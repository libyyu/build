@echo off

@set out_name=FengEngine

call make_android.bat %out_name%

echo build OK...
copy .\libs\armeabi-v7a\lib%out_name%.so ..\..\..\LuaGame\ProjectUnity\Assets\Plugins\Android\libs\armeabi-v7a\lib%out_name%.so /y

copy .\libs\x86\lib%out_name%.so ..\..\..\LuaGame\ProjectUnity\Assets\Plugins\Android\libs\x86\lib%out_name%.so /y

pause