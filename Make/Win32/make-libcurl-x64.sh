echo "start build"

CUR_PATH=$(cd `dirname $0`; pwd)
echo $CUR_PATH

CUR_DIR=$CUR_PATH

rm -f libcurl-x64.a

cd ../../curl-7.48.0/lib
mingw32-make -f Makefile.m32 clean
mingw32-make -f Makefile.m32 ARCH=w64 CURL_CFLAG_EXTRAS=-DCURL_STATICLIB
cp libcurl.a $CUR_DIR/libcurl-x64.a

echo "Build End."
