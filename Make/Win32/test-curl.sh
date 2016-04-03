LOCAL_PATH=../..
LUAJIT_PATH=../../luajit-2.0.4/src
CURL_PATH=../../curl-7.48.0
PROJECT_SRC_PATH=../../Project-Source

MODULE_NAME=test-curl

g++ \
	-I./ \
	-I$LOCAL_PATH \
	-I$LUAJIT_PATH \
	-I$CURL_PATH/include \
	-I$PROJECT_SRC_PATH \
	-I$PROJECT_SRC_PATH/Common \
	-I$PROJECT_SRC_PATH/AnyLog \
	-L./ \
	$PROJECT_SRC_PATH/AnyLog/ILog.cpp \
	$PROJECT_SRC_PATH/Common/FAssist.cpp \
	$PROJECT_SRC_PATH/FLuaExport.cpp \
	-Wl,--output-def=build/${MODULE_NAME}_x86.def \
	-Wl,--out-implib=build/${MODULE_NAME}_x86.lib \
	-Wl,--dll \
	-o x86/${MODULE_NAME}.dll \
	-shared -m32 \
	test-curl.c \
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++ -lcurldll-x86 -lws2_32 -lwldap32 -lluajit-x86

g++ \
	-I./ \
	-I$LOCAL_PATH \
	-I$LUAJIT_PATH \
	-I$CURL_PATH/include \
	-I$PROJECT_SRC_PATH \
	-I$PROJECT_SRC_PATH/Common \
	-I$PROJECT_SRC_PATH/AnyLog \
	-L./ \
	$PROJECT_SRC_PATH/AnyLog/ILog.cpp \
	$PROJECT_SRC_PATH/Common/FAssist.cpp \
	$PROJECT_SRC_PATH/FLuaExport.cpp \
	-o ${MODULE_NAME}.exe \
	test-curl.c \
	-DTEST \
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++ -lcurldll-x86 -lws2_32 -lwldap32 -lluajit-x86