#pragma once

#include "Project.h"
#include "ProjectWidget.h"

class ProjectController
{
private:
	Project* model;
	ProjectWidget* view;

public:
	ProjectController(Project*& model, ProjectWidget*& view);
	~ProjectController();

private:

	

};