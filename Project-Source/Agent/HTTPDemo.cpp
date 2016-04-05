// http_request.cpp : 定义控制台应用程序的入口点。
//

#include "HTTPRequest.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#ifdef _WIN32
#include <Windows.h>
#endif

class DownCallbackClass
{
public:
        DownCallbackClass() :m_down_finished(false) {}
        ~DownCallbackClass() {}
public:
        void DownResultCallback(int id, bool success, const std::string& data)
        {
                m_down_finished = true;
        }
        int down_callback(double total_size, double downloaded_size, void* userdata)
        {
                long tmp = static_cast<long>(downloaded_size / total_size * 100);
                printf("\rdownload progress:%d", tmp);
                return 0;
        }
        bool IsDownFinished(void) { return m_down_finished;  }
private:
        bool m_down_finished;
};

class MyResultClass
{
public:
        MyResultClass() : m_request_finished(false) { }
        ~MyResultClass() { }

public:
        void MyRequestResultCallback(int id, bool success, const std::string& data)
        {
                if (success)
                {
                        std::ofstream outfile;
                        outfile.open("baidu.html", std::ios_base::binary | std::ios_base::trunc);
                        if (outfile.good()) outfile.write(data.c_str(), data.size());
                }
                m_request_finished = true;
        }
        bool IsRequestFinish(void) { return m_request_finished;  }
private:
        bool m_request_finished;
};

int main(int argc, char* argv[])
{
        MyResultClass mc;

        HttpRequest request;
        request.SetRequestUrl("http://www.baidu.com");
        request.SetResultCallback(std::bind(&MyResultClass::MyRequestResultCallback, &mc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        request.SetRequestHeader("User-Agent:Mozilla/4.04[en](Win95;I;Nav)");

        HANDLE hRequest = request.PerformRequest(HttpRequest::REQUEST_ASYNC);
        if (hRequest)
        {
                while (mc.IsRequestFinish() == false) Sleep(300);
                long http_code;
                if (request.GetHttpCode(hRequest, &http_code))
                        std::cout << "http code: " << http_code << std::endl;

                std::string header;
                if (request.GetReceiveHeader(hRequest, &header))
                {
                        std::cout << header << std::endl;
                }

                HttpRequest::Close(hRequest);
        }

        HttpDownloader download;
        DownCallbackClass dc;
        const char* down_url = "http://dlsw.baidu.com/sw-search-sp/soft/71/10998/OfflineBaiduPlayer_151_V4.1.2.263.1432003947.exe";
        const char* down_file = "BaiduPlayer.exe";

        download.SetDownloadUrl(down_url);
        download.SetProgressCallback(std::bind(&DownCallbackClass::down_callback, &dc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        download.SetResultCallback(std::bind(&DownCallbackClass::DownResultCallback, &dc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        download.DownloadFile(down_file);
        HANDLE hDownload = download.StartDownload(HttpDownloader::DOWN_ASYNC);
        if (hDownload)
        {
                while (dc.IsDownFinished() == false)
                {
                        Sleep(300);
                }
                //to do download finish clean up
                HttpDownloader::Close(hDownload);
        }

        return 0;
}