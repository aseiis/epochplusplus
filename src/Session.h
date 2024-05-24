#pragma once

#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
#include "utils.h"

class Session
{
// METHODS //

public:
    Session(int& ID);
    Session(int& ID, QDateTime start, QDateTime end);

    void start();
    void end();
    quint64 getDuration() const; // Calculate the duration of the session in seconds
    const QDateTime& getStartDateTime() const;
    const int& getID() const;
    QJsonObject serialize() const;
    void deserialize(QJsonObject& jsonObj);

// ATTRIBUTES //

private:
    QDateTime m_startTime;
    QDateTime m_endTime;
    int m_ID;
};

