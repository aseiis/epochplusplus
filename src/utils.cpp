#include "utils.h"

void Epochpp::setMainWindow(QMainWindow* mainWindow)
{
    g_mainWindow = mainWindow;
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
