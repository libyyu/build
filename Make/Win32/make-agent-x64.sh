echo "build for windows x64"
echo ""
mkdir -p x64
mkdir -p build

MODULE_NAME=agent

echo "clear all"
echo ""
rm -f x64/${MODULE_NAME}.dll
rm -f build/${MODULE_NAME}_x64.def
rm -f build/${MODULE_NAME}_x64.lib
echo "clear end,build complie"
echo ""

LOCAL_PATH=../..
LUAJIT_PATH=../../luajit-2.0.4/src
PROJECT_SRC_PATH=../../Project-Source


g++ \
	-I./ \
	-I$LOCAL_PATH \
	-I$LUAJIT_PATH \
	-I$PROJECT_SRC_PATH \
	-I$PROJECT_SRC_PATH/Common \
	-I$PROJECT_SRC_PATH/AnyLog \
	-shared -m32 \
	$PROJECT_SRC_PATH/AnyLog/ILog.cpp \
	$PROJECT_SRC_PATH/Common/FAssist.cpp \
	$PROJECT_SRC_PATH/FLuaExport.cpp \
	-Wl,--output-def=build/${MODULE_NAME}_x64.def \
	-Wl,--out-implib=build/${MODULE_NAME}_x64.lib \
	-Wl,--dll \
	-o x64/${MODULE_NAME}.dll \
	-L./ \
	-DWIN32 -D_WIN32 -DLUA_VERSION_NUM=501 \
	-Wl,--no-whole-archive -lluajit-x64 -lcurldll-x64 -lws2_32 -lwldap32 -static-libgcc -static-libstdc++ \
	-fpermissive -fkeep-inline-functions

echo "build end."

#-Wl,--whole-archive libagent-x64.a \


