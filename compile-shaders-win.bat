@echo off

cd tools

shaderc -f ../shaders/simple-vert.sc -o ../shaders/simple-vert.bin --platform windows --type vertex --verbose -i ../core/external/bgfx/src
shaderc -f ../shaders/simple-frag.sc -o ../shaders/simple-frag.bin --platform windows --type fragment --verbose -i ../core/external/bgfx/src

pause