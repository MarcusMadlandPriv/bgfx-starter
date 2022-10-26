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

#include "event.hpp"

namespace Core
{
	/*
	 * Application event for Mouse moved. This gets called every time
	 * the mouse position changes.
	 *
	 * @param x Position of mouse cursor on the X axis.
	 * @param y Position of mouse cursor on the Y axis.
	 */
	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y);

		[[nodiscard]] float GetX() const { return mouseX; }
		[[nodiscard]] float GetY() const { return mouseY; }

		EVENT_CLASS_TYPE(MouseMoved)
		
	private:
		float mouseX, mouseY;
	};

	/*
	 * Application event for Mouse scrolled. This gets called every time
	 * a mouse wheel is scrolled either up or down.
	 *
	 * @param xOffset The offset the middle mouse button is scrolled on 
	 * the X axis.
	 * @param yOffset The offset the middle mouse button is scrolled on 
	 * the Y axis. (most common)
	 */
	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset);

		[[nodiscard]] float GetXOffset() const { return offsetX; }
		[[nodiscard]] float GetYOffset() const { return offsetY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		
	private:
		float offsetX, offsetY;
	};

	/*
	 * Base class for mouse button events.
	 *
	 * @param button Mouse button type.
	 * - 0 = Left Click
	 * - 1 = Right Click
	 * - 2 = Middle Click
	 */
	class MouseButtonEvent : public Event
	{
	public:
		explicit MouseButtonEvent(const int button);
		
		[[nodiscard]] int GetMouseButton() const { return button; }
	
	private:
		int button;
	};

	/*
	 * Application event for Mouse pressed. This gets called every time
	 * a mouse key is pressed. 
	 *
	 * @param button Mouse button type. 
	 * - 0 = Left Click
	 * - 1 = Right Click
	 * - 2 = Middle Click
	 */
	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(const int button);

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/*
	 * Application event for Mouse released. This gets called every time
	 * a mouse key is released. 
	 *
	 * @param button Mouse button type.
	 * - 0 = Left Click
	 * - 1 = Right Click
	 * - 2 = Middle Click
	 */
	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(const int button);

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}