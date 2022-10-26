#pragma once

#include "layer/layer.hpp"
#include "event/event.hpp"

class ProjectLayer : public Core::Layer
{
public:
	ProjectLayer() = default;
	virtual ~ProjectLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float& dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Core::Event& event) override;

private:

};