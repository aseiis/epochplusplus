#pragma once

#include <QtWidgets/QWidget>

#include "../headers/Project.h"

#include "../ui/ui_ProjectWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProjectWidgetClass; };
QT_END_NAMESPACE

class ProjectWidget : public QWidget
{
	Q_OBJECT

public:
	ProjectWidget(QWidget* parent = nullptr);
	~ProjectWidget();
	void UpdateUI(Project* projectDataPtr);

private:
	Ui::ProjectWidgetClass* ui;

};