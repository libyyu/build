import glob, os, sys

if "-luajit" in sys.argv:
	LUA_SRC = './luajit-2.1.0/src'
	LIBPATH = './Make/Mac'
	LIBS = ['luajit_x86_64']
else:
	LUA_SRC = './lua-5.1.5/src'
	LIBPATH='./Make/MacWithLua'
	LIBS=['lua']
LPEG_PATH = './lpeg'
PBC_PATH = './pbc-master'
JSON_PATH = './lua-cjson-2.1.0'
SOCKET_PATH = './luasocket-2.0.2/src'
SPROTO_PATH = './sproto-master'
PROJECT_SRC_PATH = './Project-Source'
PROJECT_SRC_COMMON = './Project-Source/Common'
PROJECT_SRC_ANYLOG = './Project-Source/AnyLog'

IOS_SDK = 'iPhoneOS8.4.sdk'

MODULE = 'FengEngine'

def clear(path, ext=".os"):
	for root, dirs, files in os.walk(path):
		for name in files:
			if name.endswith(ext):
				os.remove(os.path.join(root, name))

def build_mac_dylib(name, src):
	env = Environment(CCFLAGS="-g -O3 -Wall -arch x86_64 -D_IOS")
	env.Append(LINKFLAGS = '-Wl,-undefined -Wl,dynamic_lookup -arch x86_64')
	env.Append(SHLIBSUFFIX = '.bundle')
	env.Append(CPPPATH=[LUA_SRC,PROJECT_SRC_PATH, PBC_PATH, PROJECT_SRC_COMMON, PROJECT_SRC_ANYLOG])
	env['BUNDLEEMITTER'] = None
	env['BUNDLEPREFIX'] = ''
	env['BUNDLESUFFIX'] = ''
	env['BUNDLEDIRSUFFIX'] = '.bundle'
	#env['FRAMEWORKS'] = ['-framework Carbon', '-framework System']
	env['BUNDLE'] = '$SHLINK'
	env['BUNDLEFLAGS'] = ' -bundle'
	env['BUNDLECOM'] = '$BUNDLE $BUNDLEFLAGS -o ${TARGET} $SOURCES $_LIBDIRFLAGS $_LIBFLAGS $FRAMEWORKS'
	env.SharedLibrary(name+".bundle", src, LIBS=LIBS, LIBPATH=LIBPATH)
	
def build_ios_lib(name, src):
	env = Environment(CCFLAGS="-Wall -std=gnu99 -arch armv7 -arch armv7s -arch arm64")

	IXCODE = os.popen("xcode-select -print-path").read().replace("\n", "")
	env["PATH"] = IXCODE
	env.Append(CPPFLAGS = ["-isysroot", os.path.join(IXCODE, "Platforms/iPhoneOS.platform/Developer/SDKs/" + IOS_SDK)])
	
	env.Append(CPPPATH=[LUA_SRC])
	env.StaticLibrary(name, src)



luasocket_src = glob.glob('./luasocket-2.0.2/src/*.c')
if os.name != "nt":
	luasocket_src.remove('./luasocket-2.0.2/src/wsocket.c')

SOURCE_FILES = ['slua.c', 'pb.c', 'bit.c', 'lfs.c', './lpeg/lpeg.c']
pbc_src = glob.glob('./pbc-master/*.c')
pbc_src .append('./pbc-master/binding/lua/pbc-lua.c')
json_src = ['./lua-cjson-2.1.0/strbuf.c', './lua-cjson-2.1.0/lua_cjson.c', './lua-cjson-2.1.0/fpconv.c' ]
sproto_src = ['./sproto-master/sproto.c', './sproto-master/lsproto.c']
project_src = ['./Project-Source/AnyLog/ILog.cpp', './Project-Source/Common/FAssist.cpp', './Project-Source/FLuaExport.cpp']

SOURCE_FILES = SOURCE_FILES + luasocket_src + pbc_src + json_src + sproto_src + project_src

clear('./', '.os')
build_mac_dylib(MODULE, SOURCE_FILES)
#build_ios_lib("luasocket", luasocket_src)