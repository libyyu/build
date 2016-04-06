#define _F_DLL_
#include "FAssist.h"
#include "HTTPRequest.hpp"

//HttpRequest
typedef void (__stdcall* Request_Callback)(int,bool,const char*,int);

_FCFunBegin

F_LIB_API HttpRequest* exp_newHttpRequest(const char* url = NULL)
{
	HttpRequest * request = new HttpRequest();
	assert(request != NULL);
	if(url != NULL)
		request->SetRequestUrl(std::string(url));

	return request;
}

F_LIB_API void exp_releaseHttpRequest(HttpRequest* request)
{
	if(request)
	{
		delete request;
	}
	request = NULL;
}

F_LIB_API int exp_SetRequestUrl(HttpRequest * request,const char* url)
{
	assert(request != NULL && url != NULL);
	return request->SetRequestUrl(std::string(url));
}

F_LIB_API int exp_SetRequestHeader(HttpRequest * request,const char* header)
{
	assert(request != NULL && header != NULL);
	return request->SetRequestHeader(std::string(header));
}

F_LIB_API int exp_SetRequestResultCallback(HttpRequest * request,Request_Callback request_callback)
{
	class Request_InnerCallback
	{
	public:
		Request_InnerCallback(Request_Callback cb) : m_request_finished(false),m_request_cb(cb) { log_info("new Request_InnerCallback"); }
		virtual ~Request_InnerCallback() { delete this; log_info("delete Request_InnerCallback");}
		bool IsRequestFinish(void) { return m_request_finished;  }
		void RequestResultCallback(int id, bool success, const std::string& data)
		{
			log_info("RequestResultCallback,%d,%d,%d",id,success?1:0,data.size());
			if(NULL != m_request_cb)
			{
				m_request_cb(id,success,data.c_str(),data.size());
			}
			m_request_finished = true;
		}
	private:
		bool m_request_finished;
		Request_Callback m_request_cb;
	};

	assert(request != NULL && request_callback != NULL);
	return request->SetResultCallback(std::bind(&Request_InnerCallback::RequestResultCallback,new Request_InnerCallback(request_callback),
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
F_LIB_API void* exp_PerformRequest(HttpRequest * request,int type)
{
	assert(request != NULL);	
	if(type == (int)HttpRequest::REQUEST_ASYNC)
	{
		return (void*)request->PerformRequest(HttpRequest::REQUEST_ASYNC);
	}
	else
	{
		return (void*)request->PerformRequest(HttpRequest::REQUEST_SYNC);
	}
}

F_LIB_API int exp_GetRequestId(HttpRequest * request)
{
	assert(request != NULL);
	return request->GetRequestId();
}

_FCFunEnd

//HttpDownloader
typedef int (__stdcall* Download_ProgressCallback)(double, double, void*);
typedef int (__stdcall* Download_ResultCallback)(int, bool, const char*, int);
_FCFunBegin

F_LIB_API HttpDownloader* exp_newHttpDownloader(const char* url = NULL,const char* filename = NULL)
{
	HttpDownloader * download = new HttpDownloader();
	assert(download != NULL);
	if(url != NULL)
		download->SetDownloadUrl(std::string(url));

	if(filename != NULL)
		download->DownloadFile(std::string(filename));

	log_info("download:url:%s",url ? url : "NULL");
	log_info("save:filename:%s",filename ? filename : "NULL");

	return download;
}

F_LIB_API void exp_releaseDownloader(HttpDownloader* download)
{
	if(download)
	{
		delete download;
	}
	download = NULL;
}

F_LIB_API int exp_SetDownloadUrl(HttpDownloader* download, const char* url)
{
	assert(download && url);
	return download->SetDownloadUrl(url);
}

F_LIB_API int exp_DownloadFileName(HttpDownloader* download, const char* filename, int thread_count = 5)
{
	assert(download && filename);
	return download->DownloadFile(filename,thread_count);
}

F_LIB_API int exp_SetDownloadProgress(HttpDownloader* download, Download_ProgressCallback download_progress)
{
	class DownloadProgress_InnerCallback
	{
	public:
		DownloadProgress_InnerCallback(Download_ProgressCallback cb) : m_download_progress(cb) { log_info("new DownloadProgress_InnerCallback"); }
		virtual ~DownloadProgress_InnerCallback() { delete this; log_info("delete DownloadProgress_InnerCallback");}
		int down_callback(double total_size, double downloaded_size, void* userdata)
		{
			if(m_download_progress != NULL)
			{
				m_download_progress(total_size,downloaded_size,userdata);
			}
			return 0;
		}
	private:
		Download_ProgressCallback m_download_progress;
	};
	assert(download && download_progress);
	return download->SetProgressCallback(std::bind(&DownloadProgress_InnerCallback::down_callback, new DownloadProgress_InnerCallback(download_progress), 
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

F_LIB_API int exp_SetDownloadResultCallback(HttpDownloader* download,Download_ResultCallback download_result)
{
	class DownloadResult_InnerCallback
	{
	public:
		DownloadResult_InnerCallback(Download_ResultCallback cb) : m_down_finished(false), m_download_result(cb) {log_info("new DownloadResult_InnerCallback"); }
		virtual ~DownloadResult_InnerCallback() { delete this; log_info("delete DownloadResult_InnerCallback"); }
		bool IsDownFinished(void) { return m_down_finished;  }
		void DownResultCallback(int id, bool success, const std::string& data)
        {
        	if(m_download_result != NULL)
        	{
        		m_download_result(id,success,data.c_str(),data.size());
        	}
            m_down_finished = true;
        }
	private:
		Download_ResultCallback m_download_result;
		bool m_down_finished;
	};
	assert(download && download_result);
	return download->SetResultCallback(std::bind(&DownloadResult_InnerCallback::DownResultCallback, new DownloadResult_InnerCallback(download_result),
	 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

F_LIB_API void* exp_StartDownload(HttpDownloader* download, int type)
{
	assert(download != NULL);	
	if(type == (int)HttpDownloader::DOWN_ASYNC)
	{
		return (void*)download->StartDownload(HttpDownloader::DOWN_ASYNC);
	}
	else
	{
		return (void*)download->StartDownload(HttpDownloader::DOWN_SYNC);
	}
}
F_LIB_API int exp_GetHttpDownloadId(HttpDownloader * download)
{
	assert(download != NULL);
	return download->GetRequestId();
}
F_LIB_API int exp_SetDownloadUserData(HttpDownloader* download, void* userdata)
{
	assert(download != NULL);
	return download->SetUserData(userdata);
}

_FCFunEnd

//H_HANDLE
_FCFunBegin

F_LIB_API void exp_CloseRequestHandle(H_HTTPHANDLE request_handle)
{
	if(request_handle != NULL)
		HttpRequest::Close(request_handle);
}

F_LIB_API bool exp_GetHttpRequestCode(H_HTTPHANDLE request_handle, long* http_code)
{
	if(request_handle != NULL)
		return HttpRequest::GetHttpCode(request_handle,http_code);
	else
		return false;
}
F_LIB_API bool exp_GetHttpRequestReceiveHeader(H_HTTPHANDLE request_handle, char** header,int* len)
{
	if(request_handle != NULL)
	{
		std::string s_header;
		bool ret = HttpRequest::GetReceiveHeader(request_handle,&s_header);

		int size = s_header.size();
 		char* p_buffer = new char[size];
 		memset(p_buffer,0,size);
 		memcpy(p_buffer,s_header.c_str(),size);
 		*header = p_buffer;
 		*len = size;

		return ret;
	}	
	else
		return false;
}
F_LIB_API bool exp_GetHttpRequestReceiveContent(H_HTTPHANDLE request_handle, char** receive, int* len)
{
	if(request_handle != NULL)
	{
		std::string s_receive;
		bool ret = HttpRequest::GetReceiveContent(request_handle,&s_receive);

		int size = s_receive.size();
 		char* p_buffer = new char[size];
 		memset(p_buffer,0,size);
 		memcpy(p_buffer,s_receive.c_str(),size);
 		*receive = p_buffer;
 		*len = size;

		return ret;
	}	
	else
		return false;
}
F_LIB_API bool exp_GetHttpRequestErrorString(H_HTTPHANDLE request_handle, char** error_string, int* len)
{
	if(request_handle != NULL)
	{
		std::string s_error_string;
		bool ret = HttpRequest::GetReceiveContent(request_handle,&s_error_string);

		int size = s_error_string.size();
 		char* p_buffer = new char[size];
 		memset(p_buffer,0,size);
 		memcpy(p_buffer,s_error_string.c_str(),size);
 		*error_string = p_buffer;
 		*len = size;

		return ret;
	}	
	else
		return false;
}
//////////////////////////
F_LIB_API bool exp_CancelHttpDownload(H_HTTPHANDLE handle)
{
	if(handle != NULL)
		return HttpDownloader::CancelDownload(handle);
	else
		return false;
}
F_LIB_API void exp_CloseHttpDownload(H_HTTPHANDLE handle)
{
	if(handle != NULL)
		HttpDownloader::Close(handle);
}

F_LIB_API bool exp_GetHttpDownloadCode(H_HTTPHANDLE handle, long* http_code)
{
	if(handle != NULL)
		return HttpDownloader::GetHttpCode(handle,http_code);
	else
		return false;
}
F_LIB_API bool exp_GetHttpDownloadReceiveHeader(H_HTTPHANDLE handle, char** header, int* len)
{
	if(handle != NULL)
	{
		std::string s_header;
		bool ret = HttpDownloader::GetReceiveHeader(handle,&s_header);

		int size = s_header.size();
 		char* p_buffer = new char[size];
 		memset(p_buffer,0,size);
 		memcpy(p_buffer,s_header.c_str(),size);
 		*header = p_buffer;
 		*len = size;

		return ret;
	}	
	else
		return false;
}
F_LIB_API bool exp_GetHttpDownloadErrorString(H_HTTPHANDLE handle, char** error_string, int* len)
{
	if(handle != NULL)
	{
		std::string s_error_string;
		bool ret = HttpDownloader::GetReceiveHeader(handle,&s_error_string);

		int size = s_error_string.size();
 		char* p_buffer = new char[size];
 		memset(p_buffer,0,size);
 		memcpy(p_buffer,s_error_string.c_str(),size);
 		*error_string = p_buffer;
 		*len = size;

		return ret;
	}	
	else
		return false;
}
F_LIB_API void* exp_GetHttpDownloadUserData(H_HTTPHANDLE handle)
{
	if(handle != NULL)
		return HttpDownloader::GetUserData(handle);
	else
		return NULL;
}


_FCFunEnd


