#pragma once

#include <QString>
#include <QList>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "Session.h"
#include "utils.h"

class Project
{
public:
    Project(const QString& name);
    ~Project();

    void addSession(const Session& session);
    quint64 getTotalDuration() const;           // Calculate total duration of all sessions
    QString getPrettyTotalDuration() const;     // Formatted hh:mm:ss
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
    QString& getProjectName();

private:
    QString projectName;
    QList<Session> sessions;
};

