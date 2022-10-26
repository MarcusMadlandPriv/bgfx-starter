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
	 * Application event for Key input. This gets called every time
	 * a key receives input. Keycode can be compared to defines inside 
	 * "input/keycodes.h". 
	 */
	class KeyEvent : public Event
	{
	public:
		explicit KeyEvent(int code);
		[[nodiscard]] int GetKeyCode() const { return keyCode; }

	private:
		int keyCode;
	};

	/*
	 * Application event for Key pressed. This gets called every time
	 * a key is pressed. Keycode can be compared to defines inside
	 * "input/keycodes.h".
	 * 
	 * @param code Keycode usually typed as a defined key found inside
	 * "input/keycodes.h"
	 * @param repeatCount The amount of counts this key event has been 
	 * repeated if the user presses a key for a period of time. This is
	 * the count upon the user releases the key.
	 */
	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(const int code, const int repeatCount);

		[[nodiscard]] int GetRepeatCount() const { return repeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int repeatCount;
	};

	/*
	 * Application event for Key released. This gets called every time
	 * a key is released. Keycode can be compared to defines inside
	 * "input/keycodes.h".
	 * 
	 * @param code Keycode usually typed as a defined key found inside
	 * "input/keycodes.h"
	 */
	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const int code);

		EVENT_CLASS_TYPE(KeyReleased)
	};
	
	/*
	 * Application event for Key typed. This gets called every time
	 * the unicode character represented by this key is sent by the 
	 * keyboard to system input. Keycode can be compared to defines 
	 * inside "input/keycodes.h".
	 *
	 * @param code Keycode usually typed as a defined key found inside
	 * "input/keycodes.h"
	 */
	class KeyTypedEvent final : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const int keycode);

		EVENT_CLASS_TYPE(KeyTyped)
	};
}