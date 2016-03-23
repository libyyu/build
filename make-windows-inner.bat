@echo off

@echo "%1"
@set target=%1
 
if "%target%" == "x64" (
	mkdir window\x86_64
	@set outmode=-m64
	@set linkmodule=window\x86_64\libluajit.a
	@set outmodule=window/x86_64/slua.dll
) else ( 
	mkdir window\x86
	@set outmode=-m32
	@set linkmodule=window\x86\libluajit.a
	@set outmodule=window/x86/slua.dll
)

@echo Build Target Module %target%

cd luajit-2.0.4
mingw32-make clean
mingw32-make BUILDMODE=static CC="gcc %outmode%"
copy src\libluajit.a ..\%linkmodule% /y
cd ..

gcc slua.c ^
	pb.c ^
	lpeg/lpeg.c ^
	lua-cjson-2.1.0\strbuf.c ^
	lua-cjson-2.1.0\lua_cjson.c ^
	lua-cjson-2.1.0\fpconv.c ^
	pbc-win\src\alloc.c ^
	pbc-win\src\array.c ^
	pbc-win\src\bootstrap.c ^
	pbc-win\src\context.c ^
	pbc-win\src\decode.c ^
	pbc-win\src\map.c ^
	pbc-win\src\pattern.c ^
	pbc-win\src\proto.c ^
	pbc-win\src\register.c ^
	pbc-win\src\rmessage.c ^
	pbc-win\src\stringpool.c ^
	pbc-win\src\varint.c ^
	pbc-win\src\wmessage.c ^
	pbc-win\binding\lua\pbc-lua.c ^
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
	-I.\ ^
	-Ilpeg ^
	-Ilua-cjson-2.1.0 ^
	-Iluajit-2.0.4\src ^
	-Ipbc-win ^
	-Iluasocket-2.0.2 ^
	-Isproto-master ^
	-Ilsqlite3-master ^
	-Isqlite-amalgamation-3081101 ^
	-Wl,--whole-archive %linkmodule% ^
	-Wl,--no-whole-archive -lwsock32 -static-libgcc -static-libstdc++

if "%target%" == "x64" (
copy %outmodule% ..\LuaGame\ProjectUnity\Assets\Plugins\x64\slua.dll /y
) else (
copy %outmodule% ..\LuaGame\ProjectUnity\Assets\Plugins\x86\slua.dll /y
)
