@echo off

@set target=%1

set local_module=slua
 
if "%target%" == "x64" (
	mkdir window\x86_64
	@set outmode=-m64
	@set linkmodule=libluajit-window-x64.a
	@set outmodule=window\x86_64\%local_module%.dll
	@set movepath=..\LuaGame\ProjectUnity\Assets\Plugins\x64\%local_module%.dll
) else ( 
	mkdir window\x86
	@set outmode=-m32
	@set linkmodule=libluajit-window-x86.a
	@set outmodule=window\x86\%local_module%.dll
	@set movepath=..\LuaGame\ProjectUnity\Assets\Plugins\x86\%local_module%.dll
)

@echo "Build Target Module %target%"

gcc slua.c ^
	pb.c ^
	lpeg/lpeg.c ^
	lua-cjson-2.1.0/strbuf.c ^
	lua-cjson-2.1.0/lua_cjson.c ^
	lua-cjson-2.1.0/fpconv.c ^
	pbc-win/src/alloc.c ^
	pbc-win/src/array.c ^
	pbc-win/src/bootstrap.c ^
	pbc-win/src/context.c ^
	pbc-win/src/decode.c ^
	pbc-win/src/map.c ^
	pbc-win/src/pattern.c ^
	pbc-win/src/proto.c ^
	pbc-win/src/register.c ^
	pbc-win/src/rmessage.c ^
	pbc-win/src/stringpool.c ^
	pbc-win/src/varint.c ^
	pbc-win/src/wmessage.c ^
	pbc-win/binding/lua/pbc-lua.c ^
	luasocket-2.0.2/src/auxiliar.c ^
	luasocket-2.0.2/src/buffer.c ^
	luasocket-2.0.2/src/except.c ^
	luasocket-2.0.2/src/inet.c ^
	luasocket-2.0.2/src/io.c ^
	luasocket-2.0.2/src/luasocket.c ^
	luasocket-2.0.2/src/mime.c ^
	luasocket-2.0.2/src/options.c ^
	luasocket-2.0.2/src/select.c ^
	luasocket-2.0.2/src/tcp.c ^
	luasocket-2.0.2/src/timeout.c ^
	luasocket-2.0.2/src/udp.c ^
	luasocket-2.0.2/src/wsocket.c ^
	sproto-master/lsproto.c ^
	sproto-master/sproto.c ^
	lsqlite3-master/lsqlite3.c ^
	sqlite-amalgamation-3081101/sqlite3.c ^
	-o %outmodule% %outmode% -shared ^
	-I./ ^
	-Ilpeg ^
	-Ilua-cjson-2.1.0 ^
	-Iluajit-2.0.4/src ^
	-Ipbc-win ^
	-Iluasocket-2.0.2 ^
	-Isproto-master ^
	-Ilsqlite3-master ^
	-Isqlite-amalgamation-3081101 ^
	-Wl,--whole-archive %linkmodule% ^
	-Wl,--no-whole-archive -lwsock32 -static-libgcc -static-libstdc++

@copy %outmodule% %movepath% /y

@echo "Build Successful For %target%"