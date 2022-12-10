### BGFX Starter
Project template for applications using BGFX, GLFW, GLM and IMGUI. Built with premake.

[![Windows](https://github.com/MarcusMadland/bgfx-starter/actions/workflows/msbuild.yml/badge.svg)](https://github.com/MarcusMadland/bgfx-starter/actions/workflows/msbuild.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f37aebe756b046e4a16310cd8002d19c)](https://www.codacy.com/gh/MarcusMadland/bgfx-starter/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=MarcusMadland/bgfx-starter&amp;utm_campaign=Badge_Grade)
[![Apache License 2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://github.com/MarcusMadland/bgfx-starter/blob/main/LICENSE)

#### Getting Started

```
git clone https://github.com/MarcusMadland/bgfx-starter.git
```

Run `generate-project-win.bat` and `compile-shaders-win`. 

Open `project.sln` in Visual Studio. `project/src/projectlayer.cpp` contains the pipeline code that's running. Everything should run out of the box fine. Let me know if there are any issues.

![alt text](https://github.com/marcusmadland/bgfx-starter/blob/main/README.png?raw=true)

#### Contribute

Feel free to add build/compile support for different platforms other than windows etc. Should still be using premake.
