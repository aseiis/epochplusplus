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

class ProjectData : public QObject
{

    Q_OBJECT
// METHODS //

public:
    ProjectData(const QString& name, const QString& filepath = "");
    ~ProjectData();

    void start();
    void stop();
    void deleteSaveFile();
    bool addCustomSessionFrom(QDateTime& start, QDateTime& end);
    int getSessionsCount();
    QDateTime getCurrentSessionStartTime();
    quint64 getTotalDuration() const;                           // Calculate total duration of all sessions
    QString getPrettyTotalDuration() const;                     // Formatted hh:mm:ss
    QString getAvgSessionDuration();
    int getActiveDaysCount();
    QString getAvgTimePerActiveDay();
    bool saveToFile();
    QString getFilepath();
    void setFilepath(QString& filepath);
    bool loadFromFile(const QString& filepath);
    QString& getProjectName();
    void rename(const QString& newProjectName);
    QString getProjectColorQSS();
    void setProjectColorQSS(QColor newColor);
    bool isRunning();

private:
    static QString newProjectColorQSS();

signals:
    void newOpenedFile(const QString& filepath);
    void deletedFile(QString& filepath);

// ATTRIBUTES //

public:
    int ID;
    static int currentProjectCount;

private:
    QString filepath;
    QString projectName;
    QString projectColorQSS;
    int currentSessionID;
    Session* currentSession = nullptr;
    QList<Session> sessions;
};

