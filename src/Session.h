#pragma once

#include <QDateTime>
#include <QDebug>

class Session
{
// METHODS //

public:
    Session(const int ID);
    Session(const int ID, QDateTime start, QDateTime end);

    void start();
    void end();
    quint64 getDuration() const; // Calculate the duration of the session in seconds
    const QDateTime& getStartDateTime() const;
    void serialize(QDataStream& out) const;
    void deserialize(QDataStream& in);

// ATTRIBUTES //

private:
    QDateTime startTime;
    QDateTime endTime;
    int ID;
};

