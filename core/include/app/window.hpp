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

#include <memory>

#include "event/event.hpp"

struct GLFWwindow;

namespace Core
{
	class Window
	{
	private:
		struct WindowInfo
		{
			WindowInfo()
				: title("window")
				, width(1280)
				, height(720)
				, vSync(true)
			{}

			const char* title;
			uint32_t width;
			uint32_t height;
			bool vSync;

			std::function<void(Event&)> eventCallback;
		};

	public:
		Window(const char* name, uint32_t width, uint32_t height);
		~Window();

		void OnUpdate();
		void SetEventCallback(const std::function<void(Event&)>& callback);

		[[nodiscard]] GLFWwindow* GetNativeWindow();

		[[nodiscard]] uint32_t GetWidth() { return windowInfo.width; }
		[[nodiscard]] uint32_t GetHeight() { return windowInfo.height; }

	private:
		GLFWwindow* window;
		WindowInfo windowInfo;
	};
}