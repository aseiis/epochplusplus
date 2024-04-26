#include "Session.h"

Session::Session(int& ID) : ID(ID)
{
	startTime = QDateTime();
	endTime = QDateTime();
	qDebug() << "New empty Session created (" << ID << ")";
	ID++;
}

Session::Session(int& ID, QDateTime start, QDateTime end) : ID(ID), startTime(start), endTime(end)
{ 
	qDebug() << "New assigned Session created (" << ID << ")";
	ID++;
}

void Session::start()
{
	if (startTime.isValid()) {
		qDebug() << "Tried to start session but this session already has a startTime. Aborting.";
		return;
	}

	startTime = QDateTime::currentDateTime();
}

void Session::end()
{
	if (endTime.isValid()) {
		qDebug() << "Tried to start session but this session already has an endTime. Aborting.";
		return;
	}

	endTime = QDateTime::currentDateTime();
}

quint64 Session::getDuration() const
{
	if (!startTime.isValid() || !endTime.isValid()) {
		qDebug() << "Missing data. Cannot return session duration";
		return 0;
	}

	return startTime.secsTo(endTime);
}

const QDateTime& Session::getStartDateTime() const
{
	return startTime;
}

const int& Session::getID() const
{
	return ID;
}

void Session::serialize(QDataStream& out) const {
	out << startTime;
	out << endTime;
}

void Session::deserialize(QDataStream& in) {
	qint64 duration;
	in >> startTime;
	in >> endTime;
}