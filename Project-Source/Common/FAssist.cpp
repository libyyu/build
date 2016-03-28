#define _F_DLL_

#include "FAssist.h"
#if !defined(WIN32) && !defined(_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

static AnyLog::ILog* g_theLog = NULL;
static lua_State* g_luaState = NULL;
static JNIEnv* g_JniEnv = NULL;

AnyLog::ILog* g_GetAnyLog()
{
	return g_theLog;
}
lua_State* g_GetLuaState()
{
	return g_luaState;
}

#ifdef _WIN32
#elif _ANDROID
	JNIEnv* g_GetJniEnv()
	{
		return g_JniEnv;
	}

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		LOGD("hello in c native code.\n");
		jint result = -1;

		if (vm->GetEnv((void**) &g_JniEnv, JNI_VERSION_1_4) != JNI_OK) {
			return -1;
		}
		assert(g_JniEnv != NULL);

		/* success -- return valid version number */
		result = JNI_VERSION_1_4;

		return result;
	}
#else
#endif


static void _DefaultLog(int logType, const char* message)
{
	switch(logType)
	{
		case 0:
			printf("[error][%s]%s\n", MODULE_NAME,message);
			break;
		case 1:
			printf("[assert][%s]%s\n", MODULE_NAME,message);	
			break;
		case 2:
			printf("[warning][%s]%s\n", MODULE_NAME,message);
			break;
		case 3:
			printf("[info][%s]%s\n", MODULE_NAME,message);	
			break;
		case 4:
			printf("[exception][%s]%s\n", MODULE_NAME,message);	
			break;
		default:
			printf("[unknow][%s]%s\n", MODULE_NAME,message);	
			break;

	}	
}

typedef void (STD_CALL *enum_files_cb) (int index,const char* filename,bool isdir);
void EnumAllFiles(const char* pchData, int& fileIndex, enum_files_cb cb)
{
#if defined(WIN32) || defined(_WIN32)
	_finddata_t fdata;
	long done;
	char tempdir[1024] = { 0 };
	strcat(tempdir, pchData); 
	strcat(tempdir, "/*.*"); 
	done = _findfirst(tempdir, &fdata);

	if (done != -1)
	{
		int ret = 0;
		while (ret != -1)
		{
			if (fdata.attrib != _A_SUBDIR)
			{
				if (strcmp(fdata.name, "...") != 0 &&
					strcmp(fdata.name, "..") != 0 &&
					strcmp(fdata.name, ".") != 0) //过滤
				{
					char filename[1024] = { 0 };
					strcat(filename, pchData);
					strcat(filename, "/");
					strcat(filename, fdata.name);
					cb(fileIndex++, filename, false);
				}

			}
			ret = _findnext(done, &fdata);
			if (fdata.attrib == _A_SUBDIR && ret != -1)
			{

				if (strcmp(fdata.name, "...") != 0 &&
					strcmp(fdata.name, "..") != 0 &&
					strcmp(fdata.name, ".") != 0) //过滤
				{
					char filename[1024] = { 0 }; //定义字符数组
					strcat(filename, pchData); //连接字符串
					strcat(filename, "/");
					strcat(filename, fdata.name);
					cb(fileIndex, filename, true);
					EnumAllFiles(filename,fileIndex,cb); //递归调用
				}
			}
		}
	}
#else
	struct dirent* ent = NULL;
    DIR *pDir = opendir(pchData);
    while (NULL != pDir && NULL != (ent = readdir(pDir)))
    {
        if (ent->d_reclen==24)
        {
            if (ent->d_type==8)
            {
            	char filename[1024] = { 0 };
				strcat(filename, pchData);
				strcat(filename, "/");
				strcat(filename, ent->d_name);
                cb(fileIndex++, filename, false);
            }
            else
            {
             	char filename[1024] = { 0 }; //定义字符数组
				strcat(filename, pchData); //连接字符串
				strcat(filename, "/");
				strcat(filename, ent->d_name);
				cb(fileIndex, filename, true);
                EnumAllFiles(filename,fileIndex,cb); //递归调用
            }
        }
    }
#endif
}

_FCFunBegin

void L_EstablishDefaultLog()
{
	FLog::DestroyILog(g_theLog);
	g_theLog = FLog::CreateILog((void*)_DefaultLog);
}

F_LIB_API void L_CleanupLuaState()
{
	if (g_luaState != NULL)
	{
		g_luaState = NULL;
		log_info("Cleanup luaState.");
	}
}
F_LIB_API void L_SetupLuaState(lua_State* l)
{
	g_luaState = l;
	log_info("SetupLuaState.");
}
F_LIB_API void L_EstablishAnyLog(void* pfunc)
{
	FLog::DestroyILog(g_theLog);
	g_theLog = FLog::CreateILog(pfunc);
}

F_LIB_API void L_UnEstablishAnyLog()
{
	log_info("UnEstablishAnyLog.");
	FLog::DestroyILog(g_theLog);
	g_theLog = NULL;
}

F_LIB_API void L_Exit()
{
	log_info("Exit.");
	L_CleanupLuaState();
	L_UnEstablishAnyLog();
}



_FCFunEnd
