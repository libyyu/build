
PATH=E:\UnityWorks\WorkHere\UnityLuaGame\MinGW\MinGW\x86\bin;%PATH%
mkdir window\x86


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
	-o window/x86/slua.dll -m32 -shared ^
	-I.\ ^
	-Ilpeg ^
	-Ilua-cjson-2.1.0 ^
	-Iluajit-2.0.4\src ^
	-Ipbc-win ^
	-Iluasocket-2.0.2 ^
	-Isproto-master ^
	-Ilsqlite3-master ^
	-Isqlite-amalgamation-3081101 ^
	-Wl,--no-whole-archive -lwsock32 -static-libgcc -static-libstdc++

copy window\x86\slua.dll ..\LuaGame\ProjectUnity\Assets\Plugins\x86\slua.dll /y
pause
