echo "build for windows x64"
echo ""
mkdir -p x64
mkdir -p build

MODULE_NAME=slua

LOCAL_PATH=../..
LUAJIT_PATH=../../luajit-2.0.4/src
LPEG_PATH=../../lpeg
PBC_PATH=../../pbc-win
JSON_PATH=../../lua-cjson-2.1.0
SOCKET_PATH=../../luasocket-2.0.2
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
	$PROJECT_SRC_PATH/AnyLog/ILog.cpp \
	$PROJECT_SRC_PATH/Common/FAssist.cpp \
	-Wl,--output-def=build/${MODULE_NAME}_x64.def \
	-Wl,--out-implib=build/${MODULE_NAME}_x64.lib \
	-Wl,--dll \
	-o x64/${MODULE_NAME}.dll \
	-L./ \
	-Wl,--whole-archive libluajit-x64.a \
	-Wl,--no-whole-archive -lws2_32 -lwldap32 -static-libgcc -static-libstdc++ -lsupc++ -lstdc++ \
	-fpermissive -fkeep-inline-functions

echo "build end."

#-Wl,--whole-archive libluajit-x64.a \


