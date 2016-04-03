using System;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Collections;
using System.Text;
using System.Security;



public class Hello
{
    const string DLL_NAME = "slua";
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr luaL_newstate();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void SafeReleaseIntPtr(IntPtr ptr);

    public static bool MyLoadVersion(string szFileName,out string err_msg)
    {
        err_msg = null;
        IntPtr p_msg;
        int len;
        bool ret = LoadVersions(szFileName,out p_msg,out len);

        byte[] b = new byte[len];
        Marshal.Copy(p_msg,b, 0, len);
        err_msg = System.Text.Encoding.Default.GetString(b);

        SafeReleaseIntPtr(p_msg);

        return ret;
    }

    //////////////////////////////////////////////////////////
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

    const string AGENT_DLL = "agent";
    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool LoadVersions(string szFileName,out IntPtr err_msg,out int len);


    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void L_CleanupLuaState();

    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void L_SetupLuaState(IntPtr luaState);

    // enum LOG_TYPE
    // {
    //     LOG_ERROR = 0,
    //     LOG_ASSERT = 1,
    //     LOG_WARNING = 2,
    //     LOG_INFO = 3,
    //     LOG_EXCEPTION = 4,
    // };

    public delegate void Agent_AnyLog_Delegate(int logType, string message);

    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public extern static void L_EstablishAnyLog(Agent_AnyLog_Delegate func);

    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public extern static void L_UnEstablishAnyLog();

    [DllImport(AGENT_DLL, CallingConvention = CallingConvention.Cdecl)]
    public extern static void curl_get_baidu();

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
        Console.WriteLine("logType:"+logType+",msg:"+message);
    }


    public static void Main(String[] args)
    {
        try
        {
            IntPtr L = IntPtr.Zero;
            Console.WriteLine("Hello C#");
            Console.WriteLine("Test slua");

            L = luaL_newstate();

            Console.WriteLine("luaState=" + L);

            string err_msg;
            Console.WriteLine("MyLoadVersion:"+MyLoadVersion("testC++.cpp",out err_msg));
            Console.WriteLine("err_msg:"+err_msg);

            Console.WriteLine("agent");
            Establish(L);


            curl_get_baidu();
        }
        catch(Exception e)
        {
            Console.WriteLine(e.Message);
        }
        Console.ReadLine();
    }
}
