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

class ProjectData
{
public:
    ProjectData(const QString& name);
    ~ProjectData();

    void start();
    void stop();
    void addSession(const Session& session);
    QDateTime getCurrentSessionStartTime();
    quint64 getTotalDuration() const;                           // Calculate total duration of all sessions
    QString getPrettyTotalDuration() const;                     // Formatted hh:mm:ss
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
    bool loadFromSave(const QString& projectName);
    QString& getProjectName();
    int ID;
    bool isRunning();
    
    static int currentProjectCount;

private:
    QString projectName;
    int currentSessionID;
    Session* currentSession = nullptr;
    QList<Session> sessions;
};

