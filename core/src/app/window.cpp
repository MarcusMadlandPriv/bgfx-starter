/*
 * Copyright 2022 Marcus Madland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "crpch.hpp"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "app/window.hpp"
#include "event/app_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"

namespace Core
{
	Window::Window(const char* name, uint32_t width, uint32_t height)
		: window(nullptr)
	{
		windowInfo.title = name;
		windowInfo.width = width;
		windowInfo.height = height;

		// Init GLFW
		if (!glfwInit())
		{
			// @todo Log
			return;
		}
			
		// Create GLFW Window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(static_cast<int>(windowInfo.width),
			static_cast<int>(windowInfo.height), windowInfo.title, nullptr, nullptr);
		if (!window)
		{
			// @todo Log
			return;
		}

		// Init Graphics
		bgfx::renderFrame();
		bgfx::Init init;
		#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			init.platformData.ndt = glfwGetX11Display();
			init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
		#elif BX_PLATFORM_OSX
			init.platformData.nwh = glfwGetCocoaWindow(window);
		#elif BX_PLATFORM_WINDOWS
			init.platformData.nwh = glfwGetWin32Window(window);
		#endif
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		init.resolution.width = (uint32_t)w;
		init.resolution.height = (uint32_t)h;
		init.resolution.reset = BGFX_RESET_VSYNC;
		if (!bgfx::init(init))
		{
			// @todo Log
			return;
		}

		// Set event callbacks
		glfwSetWindowUserPointer(window, &windowInfo);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window_,
			const  int width_, const int height_)
			{
				WindowInfo& inf =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(window_));
				inf.width = width_;
				inf.height = height_;

				bgfx::reset((uint32_t)width_, (uint32_t)height_, BGFX_RESET_VSYNC);
				bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

				WindowResizeEvent event(width_, height_);
				inf.eventCallback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window_)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(window_));

				WindowCloseEvent event;
				data.eventCallback(event);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* currWindow, const int key,
			int scancode, const int action, int mods)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(currWindow));

				switch (action)
				{
					default:
					{
						break;
					}
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.eventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* currWindow, const uint32_t keycode)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(currWindow));

				KeyTypedEvent event(static_cast<int>(keycode));
				data.eventCallback(event);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* currWindow, const int button,
			const int action, int mods)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(currWindow));

				switch (action)
				{
					default:
					{
						break;
					}
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* currWindow,
			const double xOffset, const double yOffset)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(currWindow));

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* currWindow,
			const double xPos, const double yPos)
			{
				const WindowInfo& data =
					*static_cast<WindowInfo*>(glfwGetWindowUserPointer(currWindow));

				MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
				data.eventCallback(event);
			});
	}

	Window::~Window()
	{
		bgfx::shutdown();

		glfwDestroyWindow(window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();

		// Swap buffers
		bgfx::frame();
	}

	void Window::SetEventCallback(const std::function<void(Event&)>& callback)
	{
		windowInfo.eventCallback = callback;
	}

	GLFWwindow* Window::GetNativeWindow()
	{
		return window;
	}
}
