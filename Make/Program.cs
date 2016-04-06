using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;

namespace test_agent
{
    class Program
    {
        const string AGENT_DLL = "agent";
        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool LoadVersions(string szFileName, out IntPtr err_msg, out int len);


        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void L_CleanupLuaState();

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void L_SetupLuaState(IntPtr luaState);

        public delegate void Agent_AnyLog_Delegate(int logType, string message);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void L_EstablishAnyLog(Agent_AnyLog_Delegate func);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void L_UnEstablishAnyLog();

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void curl_get_baidu(string url);

#pragma warning disable 414
        public class MonoPInvokeCallbackAttribute : System.Attribute
        {
            private Type type;
            public MonoPInvokeCallbackAttribute(Type t)
            {
                type = t;
            }
        }
#pragma warning restore 414
        [MonoPInvokeCallbackAttribute(typeof(Agent_AnyLog_Delegate))]
        private static void On_Agent_AnyLog(int logType, string message)
        {
            Console.WriteLine(string.Format("[{0}]{1}", logType, message));
        }

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr exp_newHttpRequest(string url);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void exp_releaseHttpRequest(IntPtr request);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_SetRequestUrl(IntPtr request, string url);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_SetRequestHeader(IntPtr request, string url);

        public delegate void Agent_Request_Callback_Delegate(int id, bool success, IntPtr ptr, int len);
        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_SetRequestResultCallback(IntPtr request, Agent_Request_Callback_Delegate func);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr exp_PerformRequest(IntPtr request, int type);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_GetRequestId(IntPtr request);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void exp_CloseRequestHandle(IntPtr handle);

        private static bool bFinished = false;

        [MonoPInvokeCallbackAttribute(typeof(Agent_AnyLog_Delegate))]
        private static void On_Agent_Request_Callback(int id, bool success, IntPtr ptr, int len)
        {
            byte[] result = new byte[len];
            Marshal.Copy(ptr, result, 0, len);
            string data = Encoding.UTF8.GetString(result);
            Console.WriteLine(string.Format("id:{0},success:{1},data{2}", id, success, data));
            if (success)
            {
                Console.WriteLine(data);
            }
            else
            {
                Console.WriteLine(string.Format("Agent Request Failed id:{0}", id));
            }
            bFinished = true;
        }

        /// <summary>
        /// /////////////////////
        /// </summary>
        /// <param name="url"></param>
        /// <param name="filename"></param>
        /// <returns></returns>
        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr exp_newHttpDownloader(string url, string filename);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static void exp_releaseDownloader(IntPtr download);


        public delegate void Agent_DownloadProgress_Callback_Delegate(double total, double loaded, IntPtr ptr);
        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_SetDownloadProgress(IntPtr download, Agent_DownloadProgress_Callback_Delegate progress);

        public delegate void Agent_DownloadResult_Callback_Delegate(int id, bool success, IntPtr ptr,int len);
        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static int exp_SetDownloadResultCallback(IntPtr download, Agent_DownloadResult_Callback_Delegate result);

        [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr exp_StartDownload(IntPtr download, int type);


        [MonoPInvokeCallbackAttribute(typeof(Agent_DownloadProgress_Callback_Delegate))]
        private static void On_Agent_DownloadProgress(double total_size, double downloaded_size, IntPtr ptr)
        {
            long tmp = (long)(downloaded_size / total_size * 100);
            Console.WriteLine("\rdownload progress:{0}", tmp);
        }

        static bool bLoadFinished = false;
        [MonoPInvokeCallbackAttribute(typeof(Agent_DownloadResult_Callback_Delegate))]
        private static void On_Agent_DownloadFinished(int id, bool success, IntPtr ptr, int len)
        {
            bLoadFinished = true;
            Console.WriteLine("Download Finished");
        }

        public static void Establish(IntPtr L)
        {
            L_EstablishAnyLog(On_Agent_AnyLog);
            L_SetupLuaState(L);
        }

        public static void UnEstablish()
        {
            L_UnEstablishAnyLog();
            L_CleanupLuaState();
        }
        /// <summary>
        /// Entry
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Establish(IntPtr.Zero);


            string url = "http://www.baidu.com";

            IntPtr request = exp_newHttpRequest(url);
            Console.WriteLine("request=" + request + ",id:" + exp_GetRequestId(request));

            exp_SetRequestResultCallback(request, On_Agent_Request_Callback);
            exp_SetRequestHeader(request, "User-Agent:Mozilla/4.04[en](Win95;I;Nav)");

            IntPtr handle = exp_PerformRequest(request, 1);
            while(handle != IntPtr.Zero && !bFinished)
            {
                Thread.Sleep(1);
            }
            exp_CloseRequestHandle(handle);
            Console.WriteLine("Request End.....");

            IntPtr request2 = exp_newHttpDownloader("http://dlsw.baidu.com/sw-search-sp/soft/71/10998/OfflineBaiduPlayer_151_V4.1.2.263.1432003947.exe","BaiduPlayer.exe");
            exp_SetDownloadProgress(request2, On_Agent_DownloadProgress);
            exp_SetDownloadResultCallback(request2, On_Agent_DownloadFinished);

            IntPtr download = exp_StartDownload(request2, 1);
            while (download != IntPtr.Zero && !bLoadFinished)
            {
                Thread.Sleep(300);
            }

            Console.ReadKey();
        }
    }
}
