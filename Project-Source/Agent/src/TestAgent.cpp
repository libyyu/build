#define _F_DLL_
#include "FAssist.h"
#include "curl/curl.h"

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

_FCFunBegin

F_LIB_API void curl_get_baidu()
{
	curl_global_init(CURL_GLOBAL_ALL); // 首先全局初始化CURL
    CURL* curl = curl_easy_init(); // 初始化CURL句柄

    if (NULL == curl)
    {
    	log_error("curl_easy_init failed.")
        return;
    }

    int my_param = 1;    // 自定义一个用户参数
    
    // 设置目标URL
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
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

_FCFunEnd