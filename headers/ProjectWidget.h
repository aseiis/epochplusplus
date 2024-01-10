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
	ProjectWidget(QWidget* parent = nullptr, Project* projectPtr = nullptr);
	~ProjectWidget();
	void updateUI();
	void startCurrentSessionTimer();
	void endCurrentSessionTimer();
	QString getPrettyCurrentSessionDuration();

public slots:
	void updateCurrentSession(int elapsedTime = 0);
	void togglePlay();

private:
	Ui::ProjectWidgetClass* ui;
	Project* project;
	quint64 currentSessionElapsedSecs;
	QTimer* currentSessionTimer;

};