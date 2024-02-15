#pragma once

#include <QString>
#include <QList>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include "Session.h"
#include "utils.h"

class Project
{
public:
    Project(int ID, const QString& name);
    ~Project();

    void start();
    void stop();
    void addSession(const Session& session);
    QDateTime getCurrentSessionStartTime();
    quint64 getTotalDuration() const;                           // Calculate total duration of all sessions
    QString getPrettyTotalDuration() const;                     // Formatted hh:mm:ss
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
    QString& getProjectName();
    int ID;
    bool isRunning();

private:
    QString projectName;
    int currentSessionID;
    Session* currentSession = nullptr;
    QList<Session> sessions;
};

