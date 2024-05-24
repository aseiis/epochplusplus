#pragma once

#include "QMessageBox"
#include "QMainWindow"

// GLOBAL

namespace Epochpp {
    // CONSTANTS //
    inline const QString APP_NAME = QString("Epoch++");
    inline const char* DEF_BIN_FILE_EXTENSION = "timesheet";
    inline const char* DEF_SAVE_LOCATION = "/saves/";

    inline const char* CFG_FILE_PATH = "epochpp.cfg";

    // SETTINGS //
    inline Qt::DateFormat serialDateFormat = Qt::DateFormat::ISODate;

    // GLOBAL ACCESS //

    inline QMainWindow* g_mainWindow = nullptr;
    void setMainWindow(QMainWindow* mainWindow);

    // UTILS FUNCTIONS //
    QString secsToMS(quint64 totalSeconds);
    QString secsToHM(quint64 totalSeconds);
}

namespace MsgBoxGen {
    QMessageBox* throwNewMessageBox(QMainWindow* mainWindow, QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton, QString windowTitle = "");
}
