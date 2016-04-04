LOCAL_PATH=../..
LUAJIT_PATH=../../luajit-2.0.4/src
CURL_PATH=../../curl-7.48.0
PROJECT_SRC_PATH=../../Project-Source

MODULE_NAME=httptest

g++ \
	-I./ \
	-I$LOCAL_PATH \
	-I$LUAJIT_PATH \
	-I$CURL_PATH/include \
	-I$PROJECT_SRC_PATH \
	-I$PROJECT_SRC_PATH/Agent \
	-L./ \
	$PROJECT_SRC_PATH/Agent/HTTPRequest.cpp \
	$PROJECT_SRC_PATH/Agent/HTTPDemo.cpp \
	-o ${MODULE_NAME}.exe -m32 -std=c++11 \
	-DTEST -DCURL_STATICLIB \
	-Wl,--no-whole-archive -static-libgcc -static-libstdc++ -lcurl-x86 -lws2_32 -lwldap32