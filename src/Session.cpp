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

QJsonObject Session::serialize() const {
	QJsonObject sessionJson;
	sessionJson.insert("id", m_ID);
	sessionJson.insert("start_time", m_startTime.toString(Epochpp::serialDateFormat));
	sessionJson.insert("end_time", m_endTime.toString(Epochpp::serialDateFormat));
	return sessionJson;
}

void Session::deserialize(QJsonObject& jsonObj) {

	m_startTime = QDateTime::fromString(jsonObj.value("start_time").toString(), Epochpp::serialDateFormat);
	m_endTime = QDateTime::fromString(jsonObj.value("end_time").toString(), Epochpp::serialDateFormat);
}