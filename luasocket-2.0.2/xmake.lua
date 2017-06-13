
-- set warning all and disable error
set_warnings("all")

-- disable c99(/TP) for windows
-- if is_plat("windows") then
--     set_languages("c89")
-- else
--     set_languages("gnu99")
-- end

-- add target
target("luasocket")
    -- make as a static library
    set_kind("static")
    -- add the common source files
    if is_plat("windows") then
        add_files("src/*.c|serial.c|unix.c|usocket.c") 
    else
        add_files("src/*.c|wsocket.c") 
    end

    add_includedirs("../lua-5.1.5/src")
    set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")
    set_targetdir("$(projectdir)/$(plat)/$(arch)")
--[[
target("luasocket_dll")
    set_kind("binary")
    add_subdirs("../lua-5.1.5")
    add_deps("luasocket", "lua51")
    add_links("luasocket", "lua51")
    add_linkdirs("$(projectdir)/$(plat)/$(arch)")
    -- set the object files directory
    set_objectdir("$(buildir)/$(plat)/$(arch)/.objs")
    set_targetdir("$(projectdir)/$(plat)/$(arch)")
]]