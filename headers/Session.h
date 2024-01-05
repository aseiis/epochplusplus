#pragma once

#include <QDateTime>
#include <QDebug>

class Session
{
public:
    Session(const int ID);
    Session(const int ID, QDateTime start, QDateTime end);

    void start();
    void end();
    quint64 getDuration() const; // Calculate the duration of the session in seconds
    const QDateTime& getStartDateTime() const;
    void Session::serialize(QDataStream& out) const;
    void Session::deserialize(QDataStream& in);

private:
    QDateTime startTime;
    QDateTime endTime;
    int ID;
};

