@echo off

@set target=%1

set local_module=slua
 
if "%target%" == "x64" (
	mkdir Plugins\x64
	@set outmode=-m64
	@set linkluajit=libluajit-window-x64.a
	@set linkcurllib=libcurl-window-x64.a
	@set outmodule=Plugins\x64\%local_module%.dll
	@set movepath=..\LuaGame\ProjectUnity\Assets\Plugins\x64\%local_module%.dll
) else ( 
	mkdir Plugins\x86
	@set outmode=-m32
	@set linkluajit=libluajit-window-x86.a
	@set linkcurllib=libcurl-window-x86.a
	@set outmodule=Plugins\x86\%local_module%.dll
	@set movepath=..\LuaGame\ProjectUnity\Assets\Plugins\x86\%local_module%.dll
)

@set LOCAL_PATH=.
@set LUAJIT_PATH=luajit-2.0.4/src
@set PBC_PATH=pbc-win
@set JSON_PATH=lua-cjson-2.1.0
@set SOCKET_PATH=luasocket-2.0.2
@set PROJECT_SRC_PATH=Project-Source


@echo "Build Target Module %target%"

gcc slua.c ^
	pb.c ^
	lpeg/lpeg.c ^
	%JSON_PATH%/strbuf.c ^
	%JSON_PATH%/lua_cjson.c ^
	%JSON_PATH%/fpconv.c ^
	%PBC_PATH%/src/alloc.c ^
	%PBC_PATH%/src/array.c ^
	%PBC_PATH%/src/bootstrap.c ^
	%PBC_PATH%/src/context.c ^
	%PBC_PATH%/src/decode.c ^
	%PBC_PATH%/src/map.c ^
	%PBC_PATH%/src/pattern.c ^
	%PBC_PATH%/src/proto.c ^
	%PBC_PATH%/src/register.c ^
	%PBC_PATH%/src/rmessage.c ^
	%PBC_PATH%/src/stringpool.c ^
	%PBC_PATH%/src/varint.c ^
	%PBC_PATH%/src/wmessage.c ^
	%PBC_PATH%/binding/lua/pbc-lua.c ^
	%SOCKET_PATH%/src/auxiliar.c ^
	%SOCKET_PATH%/src/buffer.c ^
	%SOCKET_PATH%/src/except.c ^
	%SOCKET_PATH%/src/inet.c ^
	%SOCKET_PATH%/src/io.c ^
	%SOCKET_PATH%/src/luasocket.c ^
	%SOCKET_PATH%/src/mime.c ^
	%SOCKET_PATH%/src/options.c ^
	%SOCKET_PATH%/src/select.c ^
	%SOCKET_PATH%/src/tcp.c ^
	%SOCKET_PATH%/src/timeout.c ^
	%SOCKET_PATH%/src/udp.c ^
	%SOCKET_PATH%/src/wsocket.c ^
	sproto-master/lsproto.c ^
	sproto-master/sproto.c ^
	lsqlite3-master/lsqlite3.c ^
	sqlite-amalgamation-3081101/sqlite3.c ^
	%PROJECT_SRC_PATH%/AnyLog/ILog.cpp ^
	%PROJECT_SRC_PATH%/Common/FAssist.cpp ^
	-o %outmodule% %outmode% -shared ^
	-I./ ^
	-Ilpeg ^
	-I%JSON_PATH% ^
	-I%LUAJIT_PATH% ^
	-I%PBC_PATH% ^
	-I%SOCKET_PATH% ^
	-Isproto-master ^
	-Ilsqlite3-master ^
	-Isqlite-amalgamation-3081101 ^
	-I%PROJECT_SRC_PATH% ^
	-I%PROJECT_SRC_PATH%/Common ^
	-I%PROJECT_SRC_PATH%/AnyLog ^
	-L./ ^
	-DCURL_STATICLIB ^
	-Wl,--whole-archive ^
	%linkluajit% ^
	-Wl,--no-whole-archive -lws2_32 -lwldap32 -static-libgcc -static-libstdc++ -lsupc++ -lstdc++

@copy %outmodule% %movepath% /y

@echo "Build Successful For %target%"