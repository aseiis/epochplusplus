#include "utils.h"

void Epochpp::setMainWindow(QMainWindow* mainWindow)
{
    g_mainWindow = mainWindow;
}

// Return formatted as such: Minutes::Seconds for n total seconds
QString Epochpp::secsToMS(quint64 totalSeconds)
{
    if (totalSeconds == 0) {
        return QString("00:00s");
    }
    int minutes = static_cast<int>(totalSeconds / 60);
    int seconds = static_cast<int>(totalSeconds % 60);

    // Formatting the output as mm:ss
    return QString("%1:%2s")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

// Return formatted as such: Hours::Minutes for n total seconds
QString Epochpp::secsToHM(quint64 totalSeconds)
{
    if (totalSeconds == 0) {
        return QString("00:00m");
    }
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    return QString("%1h %2m")
        .arg(hours)
        .arg(minutes, 2, 10, QChar('0'));
}

QMessageBox* MsgBoxGen::throwNewMessageBox(QMainWindow* mainWindow, QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton, QString windowTitle)
{
    QMessageBox* msgBox = new QMessageBox(mainWindow->centralWidget());

    if (windowTitle == "")
        msgBox->setWindowTitle(Epochpp::APP_NAME);
    else
        msgBox->setWindowTitle(windowTitle);

    msgBox->setText(text);
    msgBox->setStandardButtons(buttons);

    if (defaultButton != QMessageBox::NoButton)
        msgBox->setDefaultButton(defaultButton);

    //msgBox->setIconPixmap(QPixmap(":/icon.png"));

    return msgBox;
}
