#include "AskSessionDataDialog.h"

AskSessionDataDialog::AskSessionDataDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Select Date and Time");

    startDateTimeLabel = new QLabel(this);
    startDateTimeLabel->setText("Session start ");

    endDateTimeLabel = new QLabel(this);
    endDateTimeLabel->setText("Session end ");

    startDateTimeEdit = new QDateTimeEdit(this);
    startDateTimeEdit->setCalendarPopup(true);
    startDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    endDateTimeEdit = new QDateTimeEdit(this);
    endDateTimeEdit->setCalendarPopup(true);
    endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    timespanLabel = new QLabel(this);
    timespanLabel->setText("The session you add will be: 00:00m");

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AskSessionDataDialog::processDateTimeInput);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AskSessionDataDialog::reject);
    connect(startDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &AskSessionDataDialog::dateTimeInputChanged);
    connect(endDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &AskSessionDataDialog::dateTimeInputChanged);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(timespanLabel);
    layout->addWidget(startDateTimeLabel);
    layout->addWidget(startDateTimeEdit);
    layout->addWidget(endDateTimeLabel);
    layout->addWidget(endDateTimeEdit);
    layout->addWidget(buttonBox);
}

void AskSessionDataDialog::processDateTimeInput()
{
    const QDateTime start = startDateTimeEdit->dateTime();
    const QDateTime end = endDateTimeEdit->dateTime();
    if (!start.isValid() || !end.isValid())
    {
        qDebug() << "User input invalid";
        return;
    }

    if (start.toSecsSinceEpoch() >= end.toSecsSinceEpoch())
    {
        qDebug() << "User input invalid: start dateTime before end or null";
        MsgBoxGen::throwNewMessageBox(Epochpp::g_mainWindow, "Please provide a valid timespan. Start time must be earlier than the end time", QMessageBox::Ok, QMessageBox::Ok, "Error: invalid session timespan")->exec();
        return;
    }

    this->accept();
}

void AskSessionDataDialog::dateTimeInputChanged()
{
    // We don't update if the session data isn't valid yet
    if (startDateTimeEdit->dateTime().toSecsSinceEpoch() > endDateTimeEdit->dateTime().toSecsSinceEpoch()) {
        return;
    }

    quint64 sessionSecs = endDateTimeEdit->dateTime().toSecsSinceEpoch() - startDateTimeEdit->dateTime().toSecsSinceEpoch();
    timespanLabel->setText("The session you add will be: " + QString(Epochpp::secsToHM(sessionSecs)));
}