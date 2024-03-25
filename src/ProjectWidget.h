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
    void rename(const QString& newProjectName);
    void askDelete();
    void immediateDelete();

// SLOTS //

// SIGNALS //

signals:
    void requestProjectDeletion(int projectID);

// ATTRIBUTES //

//TODO: CHANGE VISIBILITY AND MANAGE DEPENDENCY
public:
    ProjectData* project;

private:
	Ui::ProjectWidgetClass* ui;
	quint64 currentSessionElapsedSecs;
	QTimer* currentSessionTimer;
	bool isExpanded;
};
