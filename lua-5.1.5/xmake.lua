
-- set warning all and disable error
set_warnings("all")

-- disable c99(/TP) for windows
if is_plat("windows") then
    set_languages("c89")
else
    set_languages("gnu99")
end

-- add target
target("lua51")
    -- make as a static library
    set_kind("static")
    -- add the common source files
    add_files("src/*.c|lua.c|luac.c|print.c") 

    -- set the object files directory
    set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")

    set_targetdir("$(projectdir)/$(plat)/$(arch)")

target("luac")
    add_deps("lua51")
    set_kind("binary")
    add_files("src/luac.c","src/print.c")
    add_links("lua51")
    add_linkdirs("$(projectdir)/$(plat)/$(arch)")
    -- set the object files directory
    set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")

    set_targetdir("$(projectdir)/$(plat)/$(arch)")

target("lua")
    add_deps("lua51")
    set_kind("binary")
    add_files("src/lua.c")
    add_links("lua51")
    add_linkdirs("$(projectdir)/$(plat)/$(arch)")
    -- set the object files directory
    set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")

    set_targetdir("$(projectdir)/$(plat)/$(arch)")
