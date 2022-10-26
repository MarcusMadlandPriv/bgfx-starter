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

#include "event/event.hpp"

namespace Core 
{
	class Layer
	{
	public:
		explicit Layer(const char* name = "Layer");
		virtual ~Layer() = default;
		
		Layer(const Layer&) = default;
		Layer(Layer&&) = default;
		
		Layer& operator=(const Layer&) = default;
		Layer& operator=(Layer&&) = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const float& dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		[[nodiscard]] const char* GetName() const { return layerName; }

	protected:
		const char* layerName;
	};
}