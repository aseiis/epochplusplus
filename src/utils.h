#pragma once

#include "QMessageBox"
#include "QMainWindow"

// GLOBAL

namespace Epochpp {
    inline const QString APP_NAME = QString("Epoch++");
    inline const char* DEF_BIN_FILE_EXTENSION = "timesheet";
    inline const char* DEF_SAVE_LOCATION = "/saves/";

    inline QMainWindow* g_mainWindow = nullptr;
    void setMainWindow(QMainWindow* mainWindow);
}

namespace MsgBoxGen {
    QMessageBox* throwNewMessageBox(QMainWindow* mainWindow, QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton, QString windowTitle = "");
}
