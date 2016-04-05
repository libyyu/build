//  [5/11/2015 Carbon]
/*
                                              _ooOoo_
                                          o888888888o              
                                         888    " . "   888
                                         (|        -_-       |)                  
                                       O\          =         /O      
                                    ____/`     ---   '\____    
                                  .'  \\|                     |//  `.              
                              /  \\|||          :          |||//  \             
                           /  _|||||        -:-         |||||-  \           
                         |   | \\\              -               /// |   |         
                         | \_|             ''\---/''              |_/ |        
                          \  .-\__             `-`              __/-. /        
                     _____`. .'           /--.--\            `. . _____        
                  ."" '<  `.___\_        <|>          _/___.'  >' "".     
                 | | :  `- \`.;`             \ _ /              `;.`/ - ` : | |     
                  \  \ `-.              \_ __\ /__ _/              .-` /  /   
    ========`-.____`-.___\_____/___.-`____.-'========   
                                                `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                       佛祖保佑       永无BUG
*/   
#include <stdio.h>                       
#ifdef _WIN32
#else
#include <pthread.h>
#include <unistd.h>
#endif

#include "curl/curl.h"        //libcurl interface
#include "HTTPRequest.hpp"   //HttpRequest class

#include <list>
#include <regex>
#include <sstream>
#include <exception>

#ifndef _WIN32
typedef unsigned long DWORD;
#define INVALID_HANDLE_VALUE    (void*)0xffffffff
#define TRUE    1
#define FALSE   0
#endif  //#ifndef _WIN32

class HttpLock
{
public:
#ifdef _WIN32
    HttpLock() { InitializeCriticalSection(&m_cs); }
    ~HttpLock() { DeleteCriticalSection(&m_cs); }

    void Lock() { EnterCriticalSection(&m_cs); }
    void UnLock() { LeaveCriticalSection(&m_cs); }
#else
    HttpLock() { pthread_mutex_init(&m_lock, NULL); }
    ~HttpLock() { pthread_mutex_destroy(&m_lock); }

    int Lock(){ return pthread_mutex_lock(&m_lock); }
    int UnLock() { return pthread_mutex_unlock(&m_lock); }
#endif

private:
#ifdef _WIN32
    CRITICAL_SECTION m_cs;
#else
    pthread_mutex_t    m_lock;
#endif
};

class DoHttpLock
{
public:
    DoHttpLock(std::shared_ptr<HttpLock> & lock)
        : m_lock(lock)
    {
        m_lock->Lock();
    }

    ~DoHttpLock()
    {
        m_lock->UnLock();
    }

private:
    std::shared_ptr<HttpLock> m_lock;
};

class HttpHelper {
protected:
    HttpHelper()
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);

        s_share_handle = curl_share_init();
        curl_share_setopt(s_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }

public:
    ~HttpHelper()
    {
        curl_share_cleanup(s_share_handle);
        curl_global_cleanup();

        s_async_requests.clear();
        s_async_downloads.clear();
    }

    static HttpHelper& Instance()
    {
        static HttpHelper the_single_instance;
        s_id++;
        return the_single_instance;
    }

    static void set_share_handle(CURL* curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_SHARE, s_share_handle);
        curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
    }

    static std::list< std::shared_ptr<HttpRequest::RequestHelper> > s_async_requests;
    static std::list< std::shared_ptr<HttpDownloader::DownloadHelper> > s_async_downloads;

    static int s_id;
    static std::shared_ptr<HttpLock>       s_request_lock;
    static std::shared_ptr<HttpLock>       s_download_lock;
    static CURLSH*        s_share_handle;

#ifdef _WIN32
    static DWORD WINAPI RequestThread(LPVOID param)
#else
    static void* RequestThread(void* param)
#endif
    {
#ifdef _WIN32
        Sleep(10);
#else
        usleep(10 * 1000);
#endif

        std::shared_ptr<HttpRequest::RequestHelper>* request = reinterpret_cast<std::shared_ptr<HttpRequest::RequestHelper>*>(param);

        if (request)
        {
            (*request)->Perform();
            if ((*request)->SelfClose())
            {
                DoHttpLock http_lock(s_request_lock);
                HttpHelper::s_async_requests.remove(*request);
            }

        }

#ifdef _WIN32
        return 1;
#else
        return NULL;
#endif
    }

    static size_t RetriveHeaderFunction(char *buffer, size_t size, size_t nitems, void *userdata)
    {
        std::string* receive_header = reinterpret_cast<std::string*>(userdata);
        if (receive_header && buffer)
        {
            receive_header->append(reinterpret_cast<const char*>(buffer), size * nitems);
        }

        return nitems * size;
    }

    static size_t RetriveContentFunction(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        std::string* receive_content = reinterpret_cast<std::string*>(userdata);
        if (receive_content && ptr)
        {
            receive_content->append(reinterpret_cast<const char*>(ptr), size * nmemb);
        }

        return nmemb * size;
    }

#ifdef _WIN32
    static DWORD WINAPI DownloadThread(LPVOID param)
#else
    static void* DownloadThread(void* param)
#endif
    {
#ifdef _WIN32
        Sleep(10);
#else
        usleep(10 * 1000);
#endif

        std::shared_ptr<HttpDownloader::DownloadHelper>* request = reinterpret_cast<std::shared_ptr<HttpDownloader::DownloadHelper>*>(param);

        if (request)
        {
            (*request)->Perform();

            if ((*request)->SelfClose())
            {
                DoHttpLock http_lock(s_download_lock);
                HttpHelper::s_async_downloads.remove(*request);
            }

        }

#ifdef _WIN32
        return 1;
#else
        return NULL;
#endif
    }

    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(userdata);

        if (thread_chunk->_download->m_is_cancel)
        {
            return 0;
        }

        DoHttpLock http_lock(thread_chunk->_download->m_httplock);
        size_t written = 0;
        int real_size = size * nmemb;
        if (thread_chunk->_endidx > 0)
        {
            if (thread_chunk->_startidx <= thread_chunk->_endidx)
            {
                if (thread_chunk->_startidx + real_size > thread_chunk->_endidx)
                {
                    real_size = thread_chunk->_endidx - thread_chunk->_startidx + 1;
                }
            }
        }

        int seek_error = fseek(thread_chunk->_fp, thread_chunk->_startidx, SEEK_SET);
        if (seek_error != 0)
        {
            perror("fseek");
        }
        else
        {
            written = fwrite(ptr, 1, real_size, thread_chunk->_fp);
        }
        thread_chunk->_download->m_downloaded_size += written;
        thread_chunk->_startidx += written;

        return written;
    }

    static int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(clientp);

        DoHttpLock http_lock(thread_chunk->_download->m_httplock);

        double total_size = thread_chunk->_download->m_total_size;
        double downloaded_size = thread_chunk->_download->m_downloaded_size;
        void* userdata = thread_chunk->_download->m_userdata;
        int callback_result = thread_chunk->_download->m_download_callback(total_size, downloaded_size, userdata);

        return callback_result;
    }

#ifdef _WIN32
    static DWORD WINAPI DownloadWork(LPVOID param)
#else
    static void* DownloadWork(void* param)
#endif
    {
        HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(param);

#ifdef _WIN32
        return thread_chunk->_download->DoDownload(thread_chunk);
#else
        return (void *)(thread_chunk->_download->DoDownload(thread_chunk));
#endif
    }
};

std::list< std::shared_ptr<HttpRequest::RequestHelper> > HttpHelper::s_async_requests;
std::list< std::shared_ptr<HttpDownloader::DownloadHelper> > HttpHelper::s_async_downloads;
int HttpHelper::s_id = 0;
std::shared_ptr<HttpLock> HttpHelper::s_request_lock(new HttpLock);
std::shared_ptr<HttpLock> HttpHelper::s_download_lock(new HttpLock);
CURLSH* HttpHelper::s_share_handle = nullptr;

HttpRequest::HttpRequest()
    : m_request_handle(new HttpRequest::RequestHelper)
{
    HttpHelper::Instance();
}

HttpRequest::~HttpRequest()
{
}

int HttpRequest::SetRetryTimes(int retry_times)
{
    if (m_request_handle)
    {
        m_request_handle->SetRetryTimes(retry_times);
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestId(int id)
{
    if (m_request_handle)
    {
        m_request_handle->m_id = id;
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestTimeout(long time_out)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestTimeout(time_out) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestUrl(const std::string& url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestUrl(url) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetMovedUrl(bool get_moved_url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetMovedUrl(get_moved_url) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetPostData(const std::string& message)
{
    return SetPostData(message.c_str(), message.size());
}

int HttpRequest::SetPostData(const void* data, unsigned int size)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetPostData(data, size) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }
    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestHeader(const std::map<std::string, std::string>& headers)
{
    if (m_request_handle)
    {
        for (auto it = headers.begin(); it != headers.end(); ++it)
        {
            std::string header = it->first;
            header += ": ";
            header += it->second;
            if (m_request_handle->SetRequestHeader(header) != CURLE_OK)
            {
                return REQUEST_INVALID_OPT;
            }
        }
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestHeader(const std::string& header)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestHeader(header) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }
    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestProxy(proxy, proxy_port) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetResultCallback(ResultCallback rc)
{
    if (m_request_handle)
    {
        m_request_handle->SetResultCallback(rc);
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}

void HttpRequest::Close(HANDLE request_handle)
{
    std::shared_ptr<RequestHelper>* request = (reinterpret_cast<std::shared_ptr<RequestHelper> *>(request_handle));
    if (request == INVALID_HANDLE_VALUE || request == nullptr)
    {
        return;
    }

    bool basync = false;

    DoHttpLock http_lock(HttpHelper::s_request_lock);
    for (auto it = HttpHelper::s_async_requests.begin(); it != HttpHelper::s_async_requests.end(); ++it)
    {
        if ((*request) == *it)
        {
#ifdef _WIN32
            if (WaitForSingleObject((*request)->m_perform_thread, 10) == WAIT_OBJECT_0)
#else
            if(pthread_kill((*request)->m_perform_thread, 0) != 0)
#endif
            {
                HttpHelper::s_async_requests.remove(*request);
            }
            else
            {
                (*request)->m_close_self = true;
            }
            basync = true;
            break;
        }
    }

    if (basync == false)
    {
        //request->reset();
    }
}

HANDLE HttpRequest::PerformRequest(RequestType request_type)
{
    if (m_request_handle)
    {
        if (m_request_handle->m_is_running)
        {
            return nullptr;
        }

        if (request_type == REQUEST_SYNC)
        {
            m_request_handle->Perform();

            return &m_request_handle;
        }
        else if (request_type == REQUEST_ASYNC)
        {
            DoHttpLock http_lock(HttpHelper::s_request_lock);

            HttpHelper::s_async_requests.push_back(m_request_handle);
            std::shared_ptr<RequestHelper>& request = HttpHelper::s_async_requests.back();

#ifdef _WIN32
            DWORD thread_id;
            HANDLE async_thread = CreateThread(NULL, 0, HttpHelper::RequestThread, &request, 0, &thread_id);
            request->m_perform_thread = async_thread;
#else
            pthread_create(&(request->m_perform_thread), NULL, HttpHelper::RequestThread, &request);
#endif

            return &request;
        }

        return nullptr;
    }

    return nullptr;
}

bool HttpRequest::GetHttpCode(HANDLE request_handle, long* http_code)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request && http_code)
    {
        *http_code = (*request)->GetHttpCode();
        return true;
    }

    return false;
}

bool HttpRequest::GetReceiveHeader(HANDLE request_handle, std::string* header)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetHeader(header);
    }

    return false;
}

bool HttpRequest::GetReceiveContent(HANDLE request_handle, std::string* receive)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetContent(receive);
    }

    return false;
}

bool HttpRequest::GetErrorString(HANDLE request_handle, std::string* error_string)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetErrorString(error_string);
    }

    return false;
}

HttpRequest::RequestHelper::RequestHelper()
    : m_curl_handle(nullptr)
#ifdef _WIN32
    , m_perform_thread(nullptr)
#else
    , m_perform_thread(-1)
#endif
    , m_http_headers(nullptr)
    , m_close_self(false)
    , m_is_running(false)
    , m_retry_times(HttpRequest::s_kRetryCount)
    , m_http_code(0)
    , m_post_data(nullptr)
{
    m_result_callback = std::bind(&RequestHelper::ReqeustResultDefault, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_id = HttpHelper::s_id;
    m_curl_handle = curl_easy_init();
    HttpHelper::set_share_handle(m_curl_handle);
}

HttpRequest::RequestHelper::~RequestHelper()
{
    if (m_curl_handle)
    {
        curl_easy_cleanup(m_curl_handle);
    }
    if (m_http_headers)
    {
        curl_slist_free_all(reinterpret_cast<curl_slist*>(m_http_headers));
    }
    if (m_post_data)
    {
        delete m_post_data;
        m_post_data = nullptr;
    }
#ifdef _WIN32
    if (m_perform_thread)
    {
        CloseHandle(m_perform_thread);
    }
#endif
}

int HttpRequest::RequestHelper::SetRequestTimeout(long time_out)
{
    if (m_curl_handle)
    {
        return curl_easy_setopt(m_curl_handle, CURLOPT_TIMEOUT, 0);
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestUrl(const std::string& url)
{
    if (m_curl_handle)
    {
        if (url.substr(0, 5) == "https")
        {
            curl_easy_setopt(m_curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(m_curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
        }

        return curl_easy_setopt(m_curl_handle, CURLOPT_URL, url.c_str());
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetMovedUrl(bool get_moved_url)
{
    if (m_curl_handle)
    {
        if (get_moved_url)
        {
            curl_easy_setopt(m_curl_handle, CURLOPT_MAXREDIRS, 5);
            return curl_easy_setopt(m_curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        }
        else
        {
            return curl_easy_setopt(m_curl_handle, CURLOPT_FOLLOWLOCATION, 0L);
        }
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetPostData(const void* data, unsigned int size)
{
    if (m_curl_handle /*&& data && size > 0*/)
    {
        CURLcode curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POST, 1);
        if (curl_code == CURLE_OK)
        {
            if (m_post_data)
            {
                delete m_post_data;
                m_post_data = nullptr;
            }

            if (size == 0)
            {
                curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDS, "");
            }
            else
            {
                m_post_data = new char[size];
                memcpy(m_post_data, data, size);
                curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDS, m_post_data);
            }
        }

        if (curl_code == CURLE_OK)
        {
            curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDSIZE, size);
        }

        return curl_code;
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestHeader(const std::string& header)
{
    if (m_curl_handle && header.empty() == false)
    {
        m_http_headers = curl_slist_append(reinterpret_cast<curl_slist*>(m_http_headers), header.c_str());

        return m_http_headers ? CURLE_OK : CURLE_FAILED_INIT;
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    //CURLOPT_PROXY
    if (m_curl_handle)
    {
        CURLcode curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_PROXYPORT, proxy_port);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_PROXY, proxy.c_str());

        return curl_code;
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetResultCallback(ResultCallback rc)
{
    m_result_callback = rc;

    return CURLE_OK;
}

void HttpRequest::RequestHelper::ReqeustResultDefault(int id, bool success, const std::string& data)
{
    //default request callback do nothing
}

int HttpRequest::RequestHelper::Perform()
{
    if (m_curl_handle)
    {
        CURLcode curl_code;
        if (m_http_headers)
        {
            curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_HTTPHEADER, reinterpret_cast<curl_slist*>(m_http_headers));
            if (curl_code != CURLE_OK)
            {
                return curl_code;
            }
        }

        m_is_running = true;
        m_receive_header.clear();
        m_receive_content.clear();

        //set force http redirect
        SetMovedUrl(true);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_HEADERDATA, &m_receive_header);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_WRITEFUNCTION, HttpHelper::RetriveContentFunction);
        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_WRITEDATA, &m_receive_content);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_NOPROGRESS, 1);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_NOSIGNAL, 1);
        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0);

        curl_code = curl_easy_perform(m_curl_handle);
        if (curl_code == CURLE_OPERATION_TIMEDOUT)
        {
            int retry_count = m_retry_times;
            while (retry_count > 0)
            {
                curl_code = curl_easy_perform(m_curl_handle);
                if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
                retry_count--;
            }
        }

        curl_easy_getinfo(m_curl_handle, CURLINFO_RESPONSE_CODE, &m_http_code);
        if (curl_code == CURLE_OK && m_http_code == 200)
        {
            m_result_callback(m_id, true, m_receive_content);
        }
        else
        {
            const char* err_string = curl_easy_strerror(curl_code);
            m_error_string = err_string;
            curl_code = CURLE_HTTP_POST_ERROR;
            m_result_callback(m_id, false, m_receive_content);
        }

        m_is_running = false;

        if (m_http_headers)
        {
            curl_slist_free_all(reinterpret_cast<curl_slist*>(m_http_headers));
            m_http_headers = nullptr;
        }

        return curl_code;
    }

    return CURLE_FAILED_INIT;
}

bool HttpRequest::RequestHelper::GetHeader(std::string* header)
{
    if (m_receive_header.empty()) return false;
    else if (header) *header = m_receive_header;

    return true;
}

bool HttpRequest::RequestHelper::GetContent(std::string* receive)
{
    if (m_receive_content.empty()) return false;
    else if (receive) *receive = m_receive_content;

    return true;
}

bool HttpRequest::RequestHelper::GetErrorString(std::string* error_string)
{
    if (m_error_string.empty()) return false;
    else if (error_string) *error_string = m_error_string;

    return true;
}

HttpDownloader::HttpDownloader()
    :m_request_handle(new HttpDownloader::DownloadHelper)
{
    HttpHelper::Instance();
}

HttpDownloader::~HttpDownloader()
{

}

int HttpDownloader::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestProxy(proxy, proxy_port) == CURLE_OK)
        {
            return 0;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetRetryTimes(int retry_times /* = s_kRetryCount */)
{
    if (m_request_handle)
    {
        m_request_handle->SetRetryTimes(retry_times);
        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetTimeout(long time_out /* = 0 */)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetTimeout(time_out) == CURLE_OK)
        {
            return HttpRequest::REQUEST_OK;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetDownloadUrl(const std::string& url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestUrl(url) == CURLE_OK)
        {
            return HttpRequest::REQUEST_OK;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetUserData(void* userdata)
{
    if (m_request_handle)
    {
        m_request_handle->SetUserData(userdata);

        return HttpRequest::REQUEST_OK;
    }
    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetRequestId(int id)
{
    if (m_request_handle)
    {
        m_request_handle->SetRequestId(id);
        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetProgressCallback(ProgressCallback pc)
{
    if (m_request_handle)
    {
        m_request_handle->SetProgressCallback(pc);

        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetResultCallback(ResultCallback rc)
{
    if (m_request_handle)
    {
        m_request_handle->SetResultCallback(rc);

        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::DownloadFile(const std::string& file_name, int thread_count /* = 5 */)
{
    if (m_request_handle)
    {
        m_request_handle->SetDownloadFile(file_name);
        m_request_handle->SetDownloadThreadCount(thread_count);
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

HANDLE HttpDownloader::StartDownload(DownType down_type)
{
    if (m_request_handle)
    {
        if (m_request_handle->m_is_running)
        {
            return nullptr;
        }

        m_request_handle->Reset();

        if (down_type == DOWN_SYNC)
        {
            m_request_handle->Perform();

            return &m_request_handle;
        }
        else if (down_type == DOWN_ASYNC)
        {
            DoHttpLock http_lock(HttpHelper::s_download_lock);
            HttpHelper::s_async_downloads.push_back(m_request_handle);
            std::shared_ptr<DownloadHelper>& request = HttpHelper::s_async_downloads.back();

#ifdef _WIN32
            DWORD thread_id;
            HANDLE async_thread = CreateThread(NULL, 0, HttpHelper::DownloadThread, &request, 0, &thread_id);
            request->m_perform_thread = async_thread;
#else
            pthread_create(&(request->m_perform_thread), NULL, HttpHelper::DownloadThread, &request);
#endif

            return &request;
        }

        return nullptr;
    }

    return nullptr;
}

void HttpDownloader::Close(HANDLE handle)
{
    std::shared_ptr<DownloadHelper>* request = (reinterpret_cast<std::shared_ptr<DownloadHelper> *>(handle));
    if (request == INVALID_HANDLE_VALUE || request == nullptr)
    {
        return;
    }

    bool basync = false;

    DoHttpLock http_lock(HttpHelper::s_download_lock);
    for (auto it = HttpHelper::s_async_downloads.begin(); it != HttpHelper::s_async_downloads.end(); ++it)
    {
        if ((*request) == *it)
        {
#ifdef _WIN32
            if (WaitForSingleObject((*request)->m_perform_thread, 10) == WAIT_OBJECT_0)
#else
            if(pthread_kill((*request)->m_perform_thread, 0) != 0)
#endif
            {
                HttpHelper::s_async_downloads.remove(*request);
            }
            else
            {
                (*request)->m_close_self = true;
            }
            basync = true;
            break;
        }
    }

    if (basync == false)
    {
        (*request)->m_is_cancel = true;
        //request->reset();
    }
}

bool HttpDownloader::CancelDownload(HANDLE handle)
{
    std::shared_ptr<DownloadHelper>* request = (reinterpret_cast<std::shared_ptr<DownloadHelper> *>(handle));
    if (request == INVALID_HANDLE_VALUE || request == nullptr)
    {
        return false;
    }

    (*request)->m_is_cancel = true;

    return true;
}

bool HttpDownloader::GetHttpCode(HANDLE handle, long* http_code)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request && http_code)
    {
        *http_code = (*request)->GetHttpCode();
        return true;
    }

    return false;
}

bool HttpDownloader::GetErrorString(HANDLE handle, std::string* error_string)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetErrorString(error_string);
    }

    return false;
}

bool HttpDownloader::GetReceiveHeader(HANDLE handle, std::string* header)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetHeader(header);
    }

    return false;
}

void* HttpDownloader::GetUserData(HANDLE handle)
{

    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetUserData();
    }

    return nullptr;
}

HttpDownloader::DownloadHelper::DownloadHelper()
#ifdef _WIN32
    : m_perform_thread(nullptr)
#else
    : m_perform_thread(-1)
#endif
    , m_close_self(false)
    , m_retry_times(HttpDownloader::s_kRetryCount)
    , m_thread_count(HttpDownloader::s_kThreadCount)
    , m_http_code(0)
    , m_time_out(0)
    , m_proxy_port(0)
    , m_total_size(0.0)
    , m_downloaded_size(0.0)
    , m_multi_download(false)
    , m_download_fail(true)
    , m_is_running(false)
    , m_httplock(new HttpLock)
    , m_userdata(NULL)
{
    m_download_callback = std::bind(&DownloadHelper::DownloadDefaultCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_result_callback = std::bind(&DownloadHelper::ResultDefaultCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_id = HttpHelper::s_id;
}

HttpDownloader::DownloadHelper::~DownloadHelper()
{
    if (m_perform_thread)
    {
#ifdef _WIN32
        CloseHandle(m_perform_thread);
        m_perform_thread = nullptr;
#endif
    }
}

int HttpDownloader::DownloadHelper::SetTimeout(long time_out /* = 0 */)
{
    m_time_out = time_out;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetRequestUrl(const std::string& url)
{
    m_url = url;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    m_http_proxy = proxy;
    m_proxy_port = proxy_port;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetProgressCallback(ProgressCallback pc)
{
    m_download_callback = pc;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetResultCallback(ResultCallback rc)
{
    m_result_callback = rc;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetDownloadFile(const std::string& file_name)
{
    m_file_path = file_name;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetDownloadThreadCount(int thread_count)
{
    m_thread_count = thread_count;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::Perform()
{
    m_total_size = GetDownloadFileSize();
    if (m_total_size < 0)
    {
        return HttpRequest::REQUEST_PERFORM_ERROR;
    }

    std::string out_file_name = m_file_path;
    std::string src_file_name = out_file_name;
    out_file_name += ".dl";

    FILE *fp = nullptr;
#ifdef _WIN32
    //DeleteFileA(out_file_name.c_str());
    //fopen_s(&fp, out_file_name.c_str(), "wb");
    remove(out_file_name.c_str());
    fp = fopen(out_file_name.c_str(), "wb");
#else
    unlink(out_file_name.c_str());
    fp = fopen(out_file_name.c_str(), "wb");
#endif
    if (!fp)
    {
        return HttpRequest::REQUEST_OPENFILE_ERROR;
    }

    int down_code = HttpRequest::REQUEST_PERFORM_ERROR;
    int thread_count = SplitDownloadCount(m_total_size);

    m_thread_count = thread_count > m_thread_count ? m_thread_count : thread_count;
    //文件大小有分开下载的必要并且服务器支持多线程下载时，启用多线程下载
    if (m_multi_download && m_thread_count > 1)
    {
        long gap = static_cast<long>(m_total_size) / m_thread_count;
#ifdef _WIN32
        std::vector<HANDLE> threads;
#else
        std::vector<pthread_t> threads;
#endif

        for (int i = 0; i < m_thread_count; i++)
        {
            ThreadChunk* thread_chunk = new ThreadChunk;
            thread_chunk->_fp = fp;
            thread_chunk->_download = this;

            if (i < m_thread_count - 1)
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = thread_chunk->_startidx + gap - 1;
            }
            else
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = -1;
            }

#ifdef _WIN32
            DWORD thread_id;
            HANDLE hThread = CreateThread(NULL, 0, HttpHelper::DownloadWork, thread_chunk, 0, &(thread_id));
#else
            pthread_t hThread;
            pthread_create(&hThread, NULL, HttpHelper::DownloadWork, thread_chunk);
#endif
            threads.push_back(hThread);
        }

#ifdef _WIN32
        WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);
        for (HANDLE handle : threads)
        {
            CloseHandle(handle);
        }
#else
        for(pthread_t thread : threads)
        {
            pthread_join(thread, NULL);
        }
#endif
    }
    else
    {
        ThreadChunk* thread_chunk = new ThreadChunk;
        thread_chunk->_fp = fp;
        thread_chunk->_download = this;
        thread_chunk->_startidx = 0;
        thread_chunk->_endidx = 0;
        down_code = DoDownload(thread_chunk);
    }

    fclose(fp);

    if (m_download_fail == false)
    {
#ifdef _WIN32
        MoveFileExA(out_file_name.c_str(), src_file_name.c_str(), MOVEFILE_REPLACE_EXISTING);
#else
        unlink(src_file_name.c_str());
        rename(out_file_name.c_str(), src_file_name.c_str());
#endif
    }
    else
    {
#ifdef _WIN32
        //DeleteFileA(out_file_name.c_str());
        remove(out_file_name.c_str());
#else
        unlink(out_file_name.c_str());
#endif
    }

    m_result_callback(m_id, m_download_fail ? false : true, m_error_string);

    m_is_running = false;

    return down_code;
}

bool HttpDownloader::DownloadHelper::GetHeader(std::string* header)
{
    if (m_receive_header.empty()) return false;
    else if (header) *header = m_receive_header;

    return true;
}

bool HttpDownloader::DownloadHelper::GetErrorString(std::string* error_string)
{
    if (m_error_string.empty()) return false;
    else if (error_string) *error_string = m_error_string;

    return true;
}

int HttpDownloader::DownloadHelper::DownloadDefaultCallback(double total_size, double downloaded_size, void* userdata)
{
    return 0;
}

void HttpDownloader::DownloadHelper::ResultDefaultCallback(int id, bool success, const std::string& data)
{
}

double HttpDownloader::DownloadHelper::GetDownloadFileSize()
{
    if (m_url.empty())
    {
        return -1.0;
    }
    else
    {
        double down_file_length = -1.0;
        CURL *handle = curl_easy_init();
        HttpHelper::set_share_handle(handle);

        if (handle)
        {
            curl_easy_setopt(handle, CURLOPT_URL, m_url.c_str());
            curl_easy_setopt(handle, CURLOPT_HEADER, 1);
            curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
            curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 5);
            curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
            curl_easy_setopt(handle, CURLOPT_HEADERDATA, &m_receive_header);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HttpHelper::RetriveContentFunction);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(handle, CURLOPT_RANGE, "2-");

            CURLcode curl_code = curl_easy_perform(handle);

            if (curl_code == CURLE_OPERATION_TIMEDOUT)
            {
                int retry_count = m_retry_times;
                while (retry_count > 0)
                {
                    curl_code = curl_easy_perform(handle);
                    if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
                    retry_count--;
                }
            }

            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &m_http_code);

            if (curl_code == CURLE_OK)
            {
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &down_file_length);

                try
                {
                    //匹配"Content-Range: bytes 2-1449/26620" 则证明支持多线程下载
                    std::regex pattern("CONTENT-RANGE\\s*:\\s*\\w+\\s*(\\d+)-(\\d*)/(\\d+)", std::regex::icase);
                    m_multi_download = std::regex_search(m_receive_header, pattern);
                }
                catch(std::exception e)
                {
                    printf("[exception]%s\n", e.what());
                }
            }
            else
            {
               const char* err_string = curl_easy_strerror(curl_code);
               m_error_string = err_string;
            }            

            curl_easy_cleanup(handle);
        }

        return down_file_length;
    }
}

int HttpDownloader::DownloadHelper::DoDownload(ThreadChunk* thread_chunk)
{
    CURL* curl_handle = curl_easy_init();
    HttpHelper::set_share_handle(curl_handle);

    if (thread_chunk->_download->m_url.substr(0, 5) == "https")
    {
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, thread_chunk->_download->m_url.c_str());

    const char* user_agent = ("Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0");
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, thread_chunk->_download->m_time_out);   //0 means block always

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, HttpHelper::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, thread_chunk);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, NULL);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, HttpHelper::progress_callback);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, thread_chunk);

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);

    if (thread_chunk->_endidx != 0)
    {
        std::string down_range;
        std::ostringstream ostr;
        if (thread_chunk->_endidx > 0)
        {
            ostr << thread_chunk->_startidx << "-" << thread_chunk->_endidx;
        }
        else
        {
            ostr << thread_chunk->_startidx << "-";
        }
        
        down_range = ostr.str();
        curl_easy_setopt(curl_handle, CURLOPT_RANGE, down_range.c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        int retry_count = m_retry_times;
        while (retry_count > 0)
        {
            curl_code = curl_easy_perform(curl_handle);
            if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
            retry_count--;
        }
    }

    long http_code;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    if (curl_code == CURLE_OK && (http_code >= 200 && http_code <= 300))
    {
            m_http_code = http_code;
            thread_chunk->_download->m_download_fail = false;
    }
    else
    {
            const char* err_string = curl_easy_strerror(curl_code);
            m_error_string = err_string;
            thread_chunk->_download->m_download_fail = true;
            m_http_code = http_code;
    }

    curl_easy_cleanup(curl_handle);

    delete thread_chunk;

    return curl_code;
}

int HttpDownloader::DownloadHelper::SplitDownloadCount(double down_size)
{
    const double size_2mb = 2.0 * 1024 * 1024;
    const double size_10mb = 10.0 * 1024 * 1024;
    const double size_50mb = 50.0 * 1024 * 1024;

    if (down_size <= size_2mb)
    {
        return 1;
    }
    else if (down_size > size_2mb && down_size <= size_10mb)
    {
        return static_cast<int>(down_size / (size_2mb));
    }
    else if (down_size > size_10mb && down_size <= size_50mb)
    {
        return HttpDownloader::s_kThreadCount + 1;
    }
    else
    {
        int down_count = static_cast<int>(down_size / size_10mb);
        return down_count > 10 ? 10 : down_count;
    }

    return 1;
}

void HttpDownloader::DownloadHelper::Reset()
{
    if (m_is_running)
    {
        return;
    }

    if (m_perform_thread)   //thread run over because if m_is_running set true, Reset wont be invoke
    {
#ifdef _WIN32
        CloseHandle(m_perform_thread);
        m_perform_thread = nullptr;
#endif
    }

    m_close_self = false;
    m_multi_download = false;
    m_download_fail = true;
    m_is_running = false;
    m_is_cancel = false;
    m_http_code = 0;
    m_total_size = 0.0;
    m_downloaded_size = 0.0;

    m_receive_header = "";
    m_error_string = "";
}
