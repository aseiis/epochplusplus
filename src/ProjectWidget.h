#pragma once

#include <QtWidgets/QWidget>

#include "ProjectData.h"

#include "../ui/ui_ProjectWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProjectWidgetClass; };
QT_END_NAMESPACE

class ProjectWidget : public QWidget
{
	Q_OBJECT

// METHODS //

public:
	ProjectWidget(QWidget* parent = nullptr, ProjectData* projectPtr = nullptr);
	~ProjectWidget();
	void updateUI();
	void startCurrentSessionTimer();
	void endCurrentSessionTimer();
	QString getPrettyCurrentSessionDuration();

	void updateCurrentSession(int elapsedTime = 0);
	void togglePlay();
	void toggleDetails();
	void rename(QString& newProjectName);

// ATTRIBUTES //

private:
	Ui::ProjectWidgetClass* ui;
	ProjectData* project;
	quint64 currentSessionElapsedSecs;
	QTimer* currentSessionTimer;
	bool isExpanded;
};
