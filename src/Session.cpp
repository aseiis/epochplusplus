#include "Session.h"

Session::Session(int& ID) : m_ID(ID)
{
	m_startTime = QDateTime();
	m_endTime = QDateTime();
	qDebug() << "New empty Session created (" << ID << ")";
	ID++;
}

Session::Session(int& ID, QDateTime start, QDateTime end) : m_ID(ID), m_startTime(start), m_endTime(end)
{ 
	qDebug() << "New assigned Session created (" << ID << ")";
	ID++;
}

void Session::start()
{
	if (m_startTime.isValid()) {
		qDebug() << "Tried to start session but this session already has a startTime. Aborting.";
		return;
	}

	m_startTime = QDateTime::currentDateTime();
}

void Session::end()
{
	if (m_endTime.isValid()) {
		qDebug() << "Tried to start session but this session already has an endTime. Aborting.";
		return;
	}

	m_endTime = QDateTime::currentDateTime();
}

quint64 Session::getDuration() const
{
	if (!m_startTime.isValid() || !m_endTime.isValid()) {
		qDebug() << "Missing data. Cannot return session duration";
		return 0;
	}

	return m_startTime.secsTo(m_endTime);
}

const QDateTime& Session::getStartDateTime() const
{
	return m_startTime;
}

const int& Session::getID() const
{
	return m_ID;
}

void Session::serialize(QDataStream& out) const {
	out << m_startTime;
	out << m_endTime;
}

void Session::deserialize(QDataStream& in) {
	qint64 duration;
	in >> m_startTime;
	in >> m_endTime;
}