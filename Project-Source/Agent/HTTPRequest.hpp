#ifndef __HTTP_REQUEST_H
#define __HTTP_REQUEST_H


#include <string>
#include <map>
#include <memory>
#include <functional>
#include <vector>

//************************************
// Usage:    
// class MyResultClass
// {
// public:
//     MyResultClass() : m_request_finished(false) { }
//     ~MyResultClass() { }
// 
// public:
//     void MyRequestResultCallback(int id, bool success, const std::string& data)
//     {
//       if (success)
//       {
//        std::ofstream outfile;
//        outfile.open("baidu.html", std::ios_base::binary | std::ios_base::trunc);
//        if (outfile.good()) outfile.write(data.c_str(), data.size());
//       }
//       m_request_finished = true;
//     }
//     bool IsRequestFinish(void) { return m_request_finished; }
// private:
//     bool m_request_finished;
// };
//
// MyResultClass mc;
// HttpRequest request;
// request.SetRequestUrl("http://www.baidu.com");
// request.SetResultCallback(std::bind(&MyResultClass::MyRequestResultCallback, &mc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// request.SetRequestHeader("User-Agent:Mozilla/4.04[en](Win95;I;Nav)");
// HANDLE hRequest = request.PerformRequest(HttpRequest::REQUEST_ASYNC);
// if (hRequest)
// {
//     while (mc.IsRequestFinish() == false) Sleep(300);
//     long http_code;
//     if (request.GetHttpCode(hRequest, &http_code))
//       std::cout << "http code: " << http_code << std::endl;
//     std::string header;
//     if (request.GetReceiveHeader(hRequest, &header))
//       std::cout << header << std::endl;
//     HttpRequest::Close(hRequest);
// }
// /*recommended HttpRequest::Close(hRequest) while doing async request job and dont need request handle anymore*/
//************************************

class HttpLock;

#ifndef _WIN32
typedef void* HANDLE;
#endif

class HttpRequest
{
public:
    typedef enum {
        REQUEST_SYNC,
        REQUEST_ASYNC,
    }RequestType;

    typedef enum {
        REQUEST_OK,
        REQUEST_INVALID_OPT,
        REQUEST_PERFORM_ERROR,
        REQUEST_OPENFILE_ERROR,
        REQUEST_INIT_ERROR,
    }RequestResult;

    //int id, bool success, const std::string& data
    //-std=c++11
    typedef std::function<void(int, bool, const std::string&)> ResultCallback;
    //typedef void (__stdcall* ResultCallback)(int,bool,const std::string&)

    friend class HttpHelper;

    HttpRequest();
    ~HttpRequest();

    
    int SetRetryTimes(int retry_times = s_kRetryCount);
    int SetRequestId(int id);
    int SetRequestTimeout(long time_out = 0);
    int SetRequestUrl(const std::string& url);

    //************************************
    // Method:    SetMovedUrl
    // FullName:  HttpRequest::SetMovedUrl
    // Access:    public 
    // Returns:   int
    // Description: set http redirect follow location
    // Parameter: bool get_moved_url -- true means redirect http url
    //************************************
    int SetMovedUrl(bool get_moved_url);

    int SetPostData(const std::string& message);
    int SetPostData(const void* data, unsigned int size);

    //************************************
    // Method:    SetRequestHeader
    // FullName:  HttpRequest::SetRequestHeader
    // Access:    public 
    // Returns:   int
    // Description: set http request header, for example : Range:bytes=554554- 
    // Parameter: std::map<std::string, std::string>&
    // Parameter: std::string> & headers
    //************************************
    int SetRequestHeader(const std::map<std::string, std::string>& headers);
    int SetRequestHeader(const std::string& header);

    int SetRequestProxy(const std::string& proxy, long proxy_port);


    int SetResultCallback(ResultCallback rc);

    HANDLE PerformRequest(RequestType request_type);
    static void Close(HANDLE request_handle);

    static bool GetHttpCode(HANDLE request_handle, long* http_code);
    static bool GetReceiveHeader(HANDLE request_handle, std::string* header);
    static bool GetReceiveContent(HANDLE request_handle, std::string* receive);
    static bool GetErrorString(HANDLE request_handle, std::string* error_string);

protected:

    class RequestHelper {
    public:
        RequestHelper();
        ~RequestHelper();

        friend class HttpRequest;
        friend class HttpHelper;

        int      SetRetryTimes(int retry_times) { m_retry_times = retry_times; return REQUEST_OK; }

        int      SetRequestTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
        int      SetMovedUrl(bool get_moved_url);
        int      SetPostData(const void* data, unsigned int size);
        int      SetRequestHeader(const std::string& header);
        int      SetRequestProxy(const std::string& proxy, long proxy_port);

        int      SetResultCallback(ResultCallback rc);

        int      Perform();

        long     GetHttpCode() { return m_http_code; }
        bool     GetHeader(std::string* header);
        bool     GetContent(std::string* receive);
        bool     GetErrorString(std::string* error_string);

        bool     SelfClose(void) { return m_close_self; }

    protected:
        void    ReqeustResultDefault(int id, bool success, const std::string& data);

    private:
        HANDLE       m_curl_handle;
        HANDLE       m_http_headers;
#ifdef _WIN32
        HANDLE       m_perform_thread;
#else
        pthread_t    m_perform_thread;
#endif

        int         m_retry_times;
        int         m_id;
        bool        m_close_self;
        bool        m_is_running;
        long        m_http_code;

        std::string     m_receive_content;
        std::string     m_receive_header;
        std::string     m_error_string;
        char*           m_post_data;

        ResultCallback  m_result_callback;
    };

private:
    std::shared_ptr<RequestHelper>  m_request_handle;
    static const int               s_kRetryCount = 3;
};

//************************************
// Usage:    HttpDownloader
// class DownCallbackClass
// {
// public:
//     DownCallbackClass() :m_down_finished(false) {}
//     ~DownCallbackClass() {}
// public:
//     void DownResultCallback(int id, bool success, const std::string& data)
//     {
//       m_down_finished = true;
//     }
//     int down_callback(double total_size, double downloaded_size, void* userdata)
//     {
//       long tmp = static_cast<long>(downloaded_size / total_size * 100);
//      printf("\r下载进度%d", tmp);
//      return 0;
//     }
//     bool IsDownFinished(void) { return m_down_finished; }
// private:
//     bool m_down_finished;
// };
// HttpDownloader download;
// DownCallbackClass dc;
// const char* down_url = "http://dlsw.baidu.com/sw-search-sp/soft/71/10998/OfflineBaiduPlayer_151_V4.1.2.263.1432003947.exe";
// const char* down_file = "BaiduPlayer.exe";
// 
// download.SetDownloadUrl(down_url);
// download.SetProgressCallback(std::bind(&DownCallbackClass::down_callback, &dc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// download.SetResultCallback(std::bind(&DownCallbackClass::DownResultCallback, &dc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// download.DownloadFile(down_file);
// HANDLE hDownload = download.StartDownload(HttpDownloader::DOWN_ASYNC);
// if (hDownload)
// {
//     while (dc.IsDownFinished() == false) Sleep(300); 
//     //to do download finish clean up
//     HttpDownloader::Close(hDownload);
// }
//************************************

class HttpDownloader
{
public:
    typedef enum {
        DOWN_SYNC,
        DOWN_ASYNC,
    }DownType;

    //double total_size, double downloaded_size, void* userdata
    typedef std::function<int(double, double, void*)> ProgressCallback;
    //typedef int (__stdcall* ProgressCallback)(double, double, void*);
    //int id, bool success, const std::string& data
    typedef std::function<void(int, bool, const std::string&)> ResultCallback;
    //typedef void (__stdcall* ResultCallback)(i;nt, bool, const std::string&);

    friend class HttpHelper;

    HttpDownloader();
    ~HttpDownloader();

    int         SetRequestProxy(const std::string& proxy, long proxy_port);
    int         SetRetryTimes(int retry_times = s_kRetryCount);
    int         SetTimeout(long time_out = 0);
    int         SetDownloadUrl(const std::string& url);
    int         SetUserData(void* userdata);
    int         SetRequestId(int id);
    int         SetProgressCallback(ProgressCallback pc);
    int         SetResultCallback(ResultCallback rc);

    int         DownloadFile(const std::string& file_name, int thread_count = 5);
    HANDLE      StartDownload(DownType down_type);
    static bool CancelDownload(HANDLE handle);
    static void Close(HANDLE handle);

    static bool        GetHttpCode(HANDLE handle, long* http_code);
    static bool        GetReceiveHeader(HANDLE handle, std::string* header);
    static bool        GetErrorString(HANDLE handle, std::string* error_string);
    static void*       GetUserData(HANDLE handle);

protected:

    class DownloadHelper {
    public:
        typedef struct tThreadChunk
        {
            FILE*       _fp;
            long        _startidx;
            long        _endidx;

            DownloadHelper*     _download;
        }ThreadChunk;

        DownloadHelper();
        ~DownloadHelper();

        friend class HttpDownloader;
        friend class HttpHelper;
        friend ThreadChunk;

        void     SetRetryTimes(int retry_times) { m_retry_times = retry_times; }
        void      SetRequestId(int id) { m_id = id;  }
        int      SetTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
        int      SetRequestProxy(const std::string& proxy, long proxy_port);

        void     SetUserData(void *userdata) { m_userdata = userdata; }
        int      SetProgressCallback(ProgressCallback pc);
        int      SetResultCallback(ResultCallback rc);
        int      SetDownloadFile(const std::string& file_name);
        int      SetDownloadThreadCount(int thread_count);

        int      Perform();

        int      GetHttpCode() { return m_http_code; }
        bool     GetHeader(std::string* header);
        bool     GetErrorString(std::string* error_string);
        bool     SelfClose(void) { return m_close_self; }
        void*    GetUserData(void) { return m_userdata; }

    protected:
        int      DownloadDefaultCallback(double total_size, double downloaded_size, void* userdata);
        void     ResultDefaultCallback(int id, bool success, const std::string& data);
        double   GetDownloadFileSize();
        int      DoDownload(ThreadChunk* thread_chunk);
        int      SplitDownloadCount(double down_size);
        void  Reset(void);

    private:
#ifdef _WIN32
        HANDLE        m_perform_thread;
#else
        pthread_t     m_perform_thread;
#endif

        int          m_retry_times;
        int          m_thread_count;
        int          m_id;
        long         m_time_out;

        std::string  m_file_path;
        std::string  m_url;
        std::string  m_http_proxy;
        std::string  m_receive_header;
        std::string  m_error_string;

        bool         m_close_self;
        bool         m_multi_download;
        bool         m_download_fail;
        bool         m_is_running;
        bool         m_is_cancel;
        void*        m_userdata;
        long         m_http_code;
        long         m_proxy_port;
        double       m_total_size;
        double       m_downloaded_size;

        std::shared_ptr<HttpLock> m_httplock;
        ProgressCallback  m_download_callback;
        ResultCallback    m_result_callback;
    };

private:
    std::shared_ptr<DownloadHelper>    m_request_handle;

    static const int          s_kRetryCount = 3;
    static const int          s_kThreadCount = 4;
};

#endif  /*__HTTP_REQUEST_H*/