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

#include <GLFW/glfw3.h>

#include "app/app.hpp"

namespace Core
{
	#define BIND_EVENT_FN(x) std::bind(&App::x, \
		this, std::placeholders::_1)
	App* App::instance = nullptr;

	App::App(const char* name, uint32_t width, uint32_t height)
		: isRunning(true)
		, isMinimized(false)
		, lastFrameTime(0.0f)
		, deltaTime(0.0f)
	{
		if (!instance)
		{
			// Initialize core
			
		}

		instance = this;

		// Window
		window = new Window(name, width, height);
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// @todo Renderer::Init();

		// Layers
		#ifdef _DEBUG
		{
			imguiLayer = new ImGuiLayer();
			PushOverlay(imguiLayer);
		}
		#endif
	}

	void App::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void App::PushOverlay(Layer* layer)
	{
		layerStack.PushOverlay(layer);
	}

	void App::Run()
	{
		while (isRunning)
		{
			const auto time = static_cast<float>(glfwGetTime()); 
			deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			if (!isMinimized)
			{
				for (Layer* layer : layerStack)
					layer->OnUpdate(deltaTime);


				#ifdef _DEBUG
				{
					Core::ImGuiLayer::Begin();

					for (Layer* layer : layerStack)
						layer->OnImGuiRender();

					Core::ImGuiLayer::End();
				}
				#endif
			}

			window->OnUpdate();
		}
	}
	void App::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	bool App::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}

	bool App::OnWindowResize(const WindowResizeEvent& e)
	{
		isMinimized = (e.GetWidth() == 0 || e.GetHeight() == 0);
		return false;
	}
}