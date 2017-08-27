-- project
set_project("FengEngine")

-- version
set_version("2.1.5")

-- set xmake min version
set_xmakever("2.1.2")

-- set warning all as error
--set_warnings("all", "error")

-- set language: c99, c++11
set_languages("c99", "cxx11")

-- disable some compiler errors
--add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wdelete-non-virtual-dtor")

-- add defines
--add_defines("_GNU_SOURCE=1", "_FILE_OFFSET_BITS=64", "_LARGEFILE_SOURCE")

-- set the symbols visibility: hidden
--set_symbols("hidden")

-- strip all symbols
--set_strip("all")

-- fomit the frame pointer
--add_cxflags("-fomit-frame-pointer", "-Wunused-variable","-Wreserved-user-defined-literal")

-- for the windows platform (msvc)
if is_plat("windows") then 

    -- add some defines only for windows
    add_defines("NOCRYPT", "NOGDI")

    -- link libcmt.lib
    add_cxflags("-MT") 

    -- no msvcrt.lib
    add_ldflags("-nodefaultlib:\"msvcrt.lib\"")
end

-- for macosx
-- if is_plat("macosx") then
--     add_ldflags("-all_load", "-pagezero_size 10000", "-image_base 100000000")
-- end

-- for mode coverage
if is_mode("coverage") then
    add_ldflags("-coverage", "-fprofile-arcs", "-ftest-coverage")
end

-- add projects
if not is_option("luajit") then
    add_subdirs("../lua-5.1.5") 
end

add_subdirs("../luasocket-2.0.2")

target("FengEngine")
    set_kind("shared")
    if not is_option("luajit") then
	   add_deps("lua51")
       add_links("lua51")
       add_defines("_USE_NATIVE_LUA")
    else
    end
    add_deps("luasocket")
    add_links("luasocket")

	set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")
    set_targetdir("$(projectdir)/$(plat)/$(arch)")
    add_linkdirs("$(projectdir)","$(projectdir)/$(plat)/$(arch)")
	add_includedirs("$(projectdir)", "..", 
        "$(projectdir)/../lua-5.1.5/src",
        "../lpeg", 
        "../pbc-master", 
        "../pbc-master/src", 
        "../lua-cjson-2.1.0", 
        "../sproto-master",
        "../luasocket-2.0.2/src",
        "../Project-Source",
        "../Project-Source/Common",
        "../Project-Source/AnyLog")

    add_files("../slua.c","../bit.c", "../pb.c", "../lpeg/lpeg.c", "../i64lib.c", "../lfs.c")
    add_files("../pbc-master/src/*.c", "../pbc-master/binding/lua/pbc-lua.c")
    add_files("../lua-cjson-2.1.0/strbuf.c", "../lua-cjson-2.1.0/fpconv.c", "../lua-cjson-2.1.0/lua_cjson.c")
    add_files("../sproto-master/sproto.c", "../sproto-master/lsproto.c")
    add_files("../Project-Source/AnyLog/ILog.cpp","../Project-Source/Common/FAssist.cpp","../Project-Source/FLuaExport.cpp", "../Project-Source/DllMain.cpp")

    if is_plat("windows") then
        add_defines("WIN32","_WIN32", "LUA_BUILD_AS_DLL")
    elseif is_plat("android") then
        add_defines("ANDROID","_ANDROID")
    else
        add_defines("IOS","_IOS")
    end
