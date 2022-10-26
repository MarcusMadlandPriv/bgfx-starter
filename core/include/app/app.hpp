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

#pragma once

#include "window.hpp"
#include "event/event.hpp"
#include "event/app_event.hpp"
#include "layer/layer_stack.hpp"
#include "layer/layers/imgui_layer.hpp"

namespace Core 
{
	class App
	{
	public:
		explicit App(const char* name = "Project", 
			uint32_t width = 1280, uint32_t height = 720);
		virtual ~App() = default;

		App(const App&) = delete;
		App(App&&) = delete;
		
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static App& Instance() { return *instance; }

		[[nodiscard]] Window& GetWindow() const { return *window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);

	private:
		Window* window;
		bool isRunning;
		bool isMinimized;
		
		LayerStack layerStack;
		ImGuiLayer* imguiLayer;

		float lastFrameTime;
		float deltaTime;

		static App* instance;
	};
}