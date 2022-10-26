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

#include "event/mouse_event.hpp"

namespace Core
{
 	MouseMovedEvent::MouseMovedEvent(const float x, const float y)
 		: mouseX(x), mouseY(y)
 	{}

	MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset)
		: offsetX(xOffset), offsetY(yOffset)
	{}

	MouseButtonEvent::MouseButtonEvent(const int button)
		: button(button)
	{}

	MouseButtonPressedEvent::MouseButtonPressedEvent(const int button)
		: MouseButtonEvent(button)
	{}
	
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button)
		: MouseButtonEvent(button)
	{}
}
