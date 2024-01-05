#include "../headers/ProjectController.h"

ProjectController::ProjectController(Project*& model, ProjectWidget*& view)
{
	this->model = model;
	this->view = view;
}

ProjectController::~ProjectController()
{

}