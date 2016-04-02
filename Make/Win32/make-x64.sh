echo "build for windows x64"
echo ""
mkdir -p x64
mkdir -p build

MODULE_NAME=slua

echo "clear all"
echo ""
rm -f x64/${MODULE_NAME}.dll
rm -f build/${MODULE_NAME}_x64.def
rm -f build/${MODULE_NAME}_x64.lib
echo "clear end,begin complie"
echo ""

LOCAL_PATH=../..
LUAJIT_PATH=../../luajit-2.0.4/src
LPEG_PATH=../../lpeg
PBC_PATH=../../pbc-win
JSON_PATH=../../lua-cjson-2.1.0
SOCKET_PATH=../../luasocket-2.0.2
SPROTO_PATH=../../sproto-master
PROJECT_SRC_PATH=../../Project-Source


g++ \
	-I./ \
	-I$LOCAL_PATH \
	-I$LUAJIT_PATH \
	-I$LPEG_PATH \
	-I$PBC_PATH \
	-I$PBC_PATH/src \
	-I$JSON_PATH \
	-I$SOCKET_PATH/src \
	-I$PROJECT_SRC_PATH \
	-I$PROJECT_SRC_PATH/Common \
	-I$PROJECT_SRC_PATH/AnyLog \
	-I$PROJECT_SRC_PATH/VersionMan \
	-I$SPROTO_PATH \
	-shared -m64 \
	$LOCAL_PATH/slua.c \
	$LOCAL_PATH/pb.c \
	$LPEG_PATH/lpeg.c \
	$PBC_PATH/src/alloc.c \
	$PBC_PATH/src/array.c \
	$PBC_PATH/src/bootstrap.c \
	$PBC_PATH/src/context.c \
	$PBC_PATH/src/decode.c \
	$PBC_PATH/src/map.c \
	$PBC_PATH/src/pattern.c \
	$PBC_PATH/src/proto.c \
	$PBC_PATH/src/register.c \
	$PBC_PATH/src/rmessage.c \
	$PBC_PATH/src/stringpool.c \
	$PBC_PATH/src/varint.c \
	$PBC_PATH/src/wmessage.c \
	$PBC_PATH/binding/lua/pbc-lua.c \
	$JSON_PATH/strbuf.c \
	$JSON_PATH/lua_cjson.c \
	$JSON_PATH/fpconv.c \
	$SPROTO_PATH/sproto.c \
	$SPROTO_PATH/lsproto.c \
	$PROJECT_SRC_PATH/AnyLog/ILog.cpp \
	$PROJECT_SRC_PATH/Common/FAssist.cpp \
	$PROJECT_SRC_PATH/VersionMan/VersionMan.cpp \
	$PROJECT_SRC_PATH/FLuaExport.cpp \
	-Wl,--output-def=build/${MODULE_NAME}_x64.def \
	-Wl,--out-implib=build/${MODULE_NAME}_x64.lib \
	-Wl,--dll \
	-o x64/${MODULE_NAME}.dll \
	-L./ \
	-Wl,--whole-archive libluajit-x64.a \
	-DWIN32 -D_WIN32 -DLUA_VERSION_NUM=501 -DNOUSE_LUASOCKET \
	-Wl,--no-whole-archive -lws2_32 -lwldap32 -static-libgcc -static-libstdc++ \
	-fpermissive -fkeep-inline-functions

echo "build end."

#-Wl,--whole-archive libluajit-x64.a \


