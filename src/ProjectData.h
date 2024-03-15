#pragma once

#include <QString>
#include <QList>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include <QRandomGenerator>
#include "Session.h"
#include "utils.h"

class ProjectData
{
// METHODS //

public:
    ProjectData(const QString& name, const QString& colorQSS = QString("color: rgb(200, 200, 200);"));
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
    static QString newProjectColorQSS();
    QString getProjectColorQSS();
    int ID;
    bool isRunning();

// ATTRIBUTES //

public:
    static int currentProjectCount;

private:
    QString projectName;
    QString projectColorQSS;
    int currentSessionID;
    Session* currentSession = nullptr;
    QList<Session> sessions;
};

