#include "app/app.hpp"
#include "project_layer.hpp"

class Project : public Core::App
{
public:
	Project()
	{
		PushLayer(new ProjectLayer());
	}
};

int main(int argc, char** argv)
{
	std::unique_ptr<Project> app = std::make_unique<Project>();
	app->Run();
}