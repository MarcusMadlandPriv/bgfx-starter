@echo off

REM compile shaders

cd tools


REM simple shader
shaderc -f ../shaders/simple-vert.sc -o ../shaders/simple-vert.bin --platform windows ./ --type vertex --verbose -i ../core/external/bgfx/src -p vs_5_0
shaderc -f ../shaders/simple-frag.sc -o ../shaders/simple-frag.bin --platform windows ./ --type fragment --verbose -i ../core/external/bgfx/src -p ps_5_0

pause