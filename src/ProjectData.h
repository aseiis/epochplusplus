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
    ProjectData(const QString& name);
    ~ProjectData();

    void start();
    void stop();
    void deleteSaveFile();
    void addSession(const Session& session);
    int getSessionsCount();
    QDateTime getCurrentSessionStartTime();
    quint64 getTotalDuration() const;                           // Calculate total duration of all sessions
    QString getPrettyTotalDuration() const;                     // Formatted hh:mm:ss
    QString getAvgSessionDuration();
    int getActiveDaysCount();
    QString getAvgTimePerActiveDay();
    bool saveToFile();
    QString getFilepath();
    bool loadFromFile(const QString& filePath);
    bool loadFromSave(const QString& projectName);
    QString& getProjectName();
    void rename(const QString& newProjectName);
    QString getProjectColorQSS();
    void setProjectColorQSS(QColor newColor);
    bool isRunning();

private:
    static QString newProjectColorQSS();

// ATTRIBUTES //

public:
    int ID;
    static int currentProjectCount;

private:
    QString projectName;
    QString projectColorQSS;
    int currentSessionID;
    Session* currentSession = nullptr;
    QList<Session> sessions;
};

