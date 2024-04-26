#pragma once

#include <QtWidgets/QWidget>
#include <QDialog>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDebug>

#include "utils.h"


class AskSessionDataDialog : public QDialog
{
	Q_OBJECT

	// METHODS //
public:
	AskSessionDataDialog(QWidget* parent = nullptr);

	// ATTRIBUTES //
	QDateTimeEdit* startDateTimeEdit;
	QDateTimeEdit* endDateTimeEdit;
	QLabel* startDateTimeLabel;
	QLabel* endDateTimeLabel;
	QLabel* timespanLabel;

private slots:
	void processDateTimeInput();
	void dateTimeInputChanged();
};