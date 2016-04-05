#define _F_DLL_
#include "FAssist.h"
#include "curl/curl.h"
#include <string>
#include <assert.h>

/**
 * 一旦curl接收到数据，就会调用此回调函数
 * buffer:数据缓冲区指针
 * size:调试阶段总是发现为1
 * nmemb:(memory block)代表此次接受的内存块的长度
 * userp:用户自定义的一个参数
 */
static int current_index = 0;
size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
    log_info("current:%d",current_index)
    log_info((char*)buffer);
    log_info("---------------");

    int temp = *(int*)userp;    // 这里获取用户自定义参数
    return nmemb;
}
size_t download_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
    FILE* fp = (FILE*)userp;
    fwrite((char*)buffer,1,size,fp);

    return nmemb;
}

_FCFunBegin

F_LIB_API void curl_get_baidu(const char* url)
{
    log_info("get_url:%s",url);
	curl_global_init(CURL_GLOBAL_ALL); // 首先全局初始化CURL
    CURL* curl = curl_easy_init(); // 初始化CURL句柄

    if (NULL == curl)
    {
    	log_error("curl_easy_init failed.")
        return;
    }

    int my_param = 1;    // 自定义一个用户参数
    
    // 设置目标URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 设置接收到HTTP服务器的数据时调用的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    // 设置自定义参数(回调函数的第四个参数)
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &my_param);
    // 执行一次URL请求
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        log_error("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // 清理干净
    curl_easy_cleanup(curl);
}

F_LIB_API void curl_download(const char* url,const char* filename)
{
    log_info("curl_download url:%s,filename:%s",url,filename);
    curl_global_init(CURL_GLOBAL_ALL); // 首先全局初始化CURL
    CURL* curl = curl_easy_init(); // 初始化CURL句柄

    if (NULL == curl)
    {
        log_error("curl_easy_init failed.")
        return;
    }

    FILE* fp = fopen(filename,"wb");
    assert(fp != NULL);
    // 设置目标URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 设置接收到HTTP服务器的数据时调用的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_data);
    // 设置自定义参数(回调函数的第四个参数)
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    // 执行一次URL请求
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        log_error("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // 清理干净
    curl_easy_cleanup(curl);
    fclose(fp);
}

#ifdef TEST
void default_print(int type,const char* message)
{
    printf("[%d]:%s\n", type,message);
}
void print_usage()
{
    log_warning(" usage:");
    log_warning(" -url:www.baidu.com");
    log_warning(" ");

}



void handle_cmd(const char*arg,std::string& cmd,std::string& param)
{
    std::string cmdline(arg);
    std::string::size_type nbegin = cmdline.find_first_of(':');
    if(nbegin != std::string::npos)
    {
        cmd = cmdline.substr(0,nbegin);
        param = cmdline.substr(nbegin+1);
    }
    else
    {
        cmd = cmdline;
    }
}

extern void L_EstablishAnyLog(void* pfunc);

int main(int argc, char *argv[])
{
    L_EstablishAnyLog((void*)default_print);
    if (argc <= 1)
    {
        print_usage();
        return 0;
    }

    for(int i=1;i<argc;++i)
    {
        std::string cmd;
        std::string param;
        handle_cmd(argv[i],cmd,param);

        if (strcmp("-url", cmd.c_str()) == 0)
        {
            curl_get_baidu(param.c_str());
        }
        else if(strcmp("-download", cmd.c_str()) == 0)
        {
            log_warning("param:%s",param.c_str());
            std::string url;
            std::string filename;
            std::string tmp;
            std::string::size_type nbegin = param.find("-filename:",0);
            if(nbegin != -1)
            {
                filename = param.substr(nbegin);
                tmp = param.substr(0,nbegin);
                nbegin = tmp.find_first_of("-url:");
                handle_cmd(tmp.c_str(),tmp,url); 
            }
            else
            {
                handle_cmd(param.c_str(),tmp,url);
                filename = "download-tmp";
            }
            if(url.size()>0 && filename.size() >0)
            {
                curl_download(url.c_str(),filename.c_str());
            }
        }
    }

    return 0;
}
#endif

_FCFunEnd