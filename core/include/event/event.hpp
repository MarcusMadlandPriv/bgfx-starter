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
 * See the License for the specific language governing permissionsand
 * limitations under the License.
 */

#pragma once

#include <functional>

namespace Core
{
	/*
	 * All event types sorted in this order
	 * - App Events
	 * - Key Events
	 * - Mouse Events
	 */
	enum class EventType
	{
		None,

		WindowClose, 
		WindowResize, 

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
	};
	
	/*
	 * Macro that overrides the GetEventType with a static EventType. Example 
	 * can be seen inside "app_event.h -> WindowResizeEvent". Contains a virtual 
	 * function that returns the static type. 
	 *
	 * GetStaticType() is used for templates. 
	 * Example can be seen inside "event.h -> EventDispatcher".
	 * 
	 * GetEventType() is used at runtime when having a reference to the event 
	 * it self.
	 * 
	 * @param type The type of the event that uses this macro.
	 */
	#define EVENT_CLASS_TYPE(type) \
		static EventType GetStaticType() { return EventType::##type; } \
		virtual EventType GetEventType() const override { return GetStaticType(); } 

	/*
	 * Base class for all events. All events are blocking, meaning when an 
	 * event occurs it immediately gets dispatched and must be dealt with 
	 * right away. 
	 */
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;
		
		Event(const Event&) = default;
		Event(Event&&) = default;
		
		Event& operator=(const Event&) = default;
		Event& operator=(Event&&) = default;
		
		/*
		 * Gets the event type. This is a pure virtual function but it is 
		 * overriden using the EVENT_CLASS_TYPE() macro with an event type 
		 * as input.
		 */
		[[nodiscard]] virtual EventType GetEventType() const = 0;

		/*
		 * Has the event be handled or not. This is used to tell the event
		 * system that this event should not be handled by any other class/layer. 
		 */
		bool handled = false;
	};

	/*
	 * A way to dispatch the events based on the type. If event is recieved and the
	 * event function is called, it would contain an event reference. To find out
	 * what event this is we use that reference and Dispatch() function multiple times
	 * until it matches the static type. It then calls that event function that we input 
	 * to the dispatch function.
	 */
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		explicit EventDispatcher(Event& event);

		/*
		 * Simply explained, a way to call functions when an event is called. Automates 
		 * the if statements and casting that would be present if this did not exist. 
		 * 
		 * @param func Function that takes event as input and bool as return value.
		 */
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{
				event.handled = func(*static_cast<T*>(&event));
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};
}