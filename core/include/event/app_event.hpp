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

#include "event.hpp"

namespace Core
{
	/*
	 * Application event for resizing the window. This gets called every time
	 * a window is changed in size.
	 * 
	 * @param width New window width
	 * @param height New window height
	 */
	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const uint32_t width, const uint32_t height);

		[[nodiscard]] uint32_t GetWidth() const { return width; }
		[[nodiscard]] uint32_t GetHeight() const { return height; }

		/*
		 * Overrides GetEventType() with WindowResize event as return value. 
		 */
		EVENT_CLASS_TYPE(WindowResize)

	private:
		uint32_t width;
		uint32_t height;
	};

	/*
	 * Application event for closing the window. This gets called every time
	 * a window closes.
	 */
	class WindowCloseEvent final : public Event
	{
	public:

		/*
		 * Overrides GetEventType() with WindowClose event as return value.
		 */
		EVENT_CLASS_TYPE(WindowClose)
	};
}
