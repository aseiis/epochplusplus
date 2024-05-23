#include "ProjectData.h"

//static
int ProjectData::currentProjectCount = 0;

ProjectData::ProjectData(const QString& name, const QString& filepath) : m_projectName(name), m_filepath(filepath), m_ID(currentProjectCount)
{
    currentSession = nullptr;
    m_currentSessionID = 0;
    m_sessions = QList<Session>();
    currentProjectCount++;
    m_projectColorQSS = newProjectColorQSS();
}

ProjectData::~ProjectData()
{
    // Save file on destruction?
}

void ProjectData::start()
{
    if (currentSession != nullptr) {
        qDebug() << "ERROR: Tried to start a new session but project already have a running session registered. Aborting.";
        return;
    }

    currentSession = new Session(m_currentSessionID);
    currentSession->start();
}

void ProjectData::stop()
{
    if (currentSession == nullptr) {
        qDebug() << "ERROR: Tried to stop current session but project doesn't register any running session. Aborting.";
        return;
    }

    currentSession->end();
    m_sessions.push_back(*currentSession);
    currentSession = nullptr;

    saveToFile();
}

void ProjectData::deleteSaveFile()
{
    QString filepath = getFilepath();
    qDebug() << "Deleting file at " << filepath;
    QFile f = QFile(filepath);
    f.remove();

    emit signalDeletedFile(filepath);
}

bool ProjectData::isRunning()
{
    return currentSession != nullptr;
}

QString ProjectData::newProjectColorQSS()
{
    int red = QRandomGenerator::global()->bounded(256);
    int green = QRandomGenerator::global()->bounded(256);
    int blue = QRandomGenerator::global()->bounded(256);

    return QString("color: rgb(%1, %2, %3);").arg(red).arg(green).arg(blue);
}

bool ProjectData::addCustomSessionFrom(QDateTime& start, QDateTime& end)
{
    if (!start.isValid() || !end.isValid())
    {
        return false;
    }

    Session s = Session(m_currentSessionID, start, end);
    m_sessions.push_back(s);
    saveToFile();
    return true;
}

int ProjectData::getSessionsCount()
{
    return m_sessions.count();
}

QDateTime ProjectData::getCurrentSessionStartTime()
{
    return currentSession->getStartDateTime();
}

quint64 ProjectData::getTotalDuration() const
{
    quint64 total = 0;
    for (const Session& session : m_sessions) {
        total += session.getDuration();
    }
    return total;
}

QString ProjectData::getPrettyTotalDuration() const
{
    quint64 seconds = getTotalDuration();
    return Epochpp::secsToHM(seconds);
}

QString ProjectData::getAvgSessionDuration()
{
    quint64 seconds = 0;
    if (getSessionsCount() != 0) //avoid division by zero
    {
        seconds = getTotalDuration() / getSessionsCount();
    }
    
    return Epochpp::secsToHM(seconds);
}

int ProjectData::getActiveDaysCount()
{
    if (m_sessions.isEmpty()) {
        return 0;
    }

    int activeDays = 1;

    QDate lastDay = m_sessions.first().getStartDateTime().date();

    for (Session& s : m_sessions)
    {
        QDate sDay = s.getStartDateTime().date();
        if (sDay != lastDay) {
            activeDays++;
            lastDay = sDay;
        }
    }

    return activeDays;
}

QString ProjectData::getAvgTimePerActiveDay()
{
    quint64 avgSeconds = 0;
    if (getActiveDaysCount() != 0) //avoid division by zero
    {
        avgSeconds = getTotalDuration() / getActiveDaysCount();
    }

    return Epochpp::secsToHM(avgSeconds);
}

bool ProjectData::saveToFile()
{
    QString verifiedFilepath = getFilepath();
    if (verifiedFilepath == "")
    {
        qDebug() << "ERROR: Couldn't save file for project " << m_projectName << " : no filepath";
        return false;
    }
    
    QFile file(verifiedFilepath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "ERROR: Couldn't save file at " << verifiedFilepath << ". Error: " << file.errorString();
        return false;
    }

    qDebug() << "Saving to file " << verifiedFilepath;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out << m_projectName;

    out << m_projectColorQSS;

    out << quint32(m_sessions.count());

    qDebug() << "Serializing " << quint32(m_sessions.count()) << " sessions...";

    for (const Session& session : m_sessions)
    {
        qDebug() << "Session " << session.getID() << " saved";
        session.serialize(out);
    }

    return true;
}

QString ProjectData::getFilepath(bool checkForExistantFile)
{
    if (m_filepath == "")
        qDebug() << "ERROR! No filepath registered for project " << m_projectName;
    
    if (checkForExistantFile && !QFile::exists(m_filepath))
        qDebug() << "WARNING! Project " << m_projectName << " save filepath point to a non-existant file: " << m_filepath;

    return m_filepath;
}

void ProjectData::setFilepath(QString& filepath)
{
    m_filepath = filepath;
    emit signalTrackFile(m_filepath);
}

bool ProjectData::loadFromFile(const QString& filepath)
{
    qDebug() << "Loading from file " << filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Couldn't open file at " << filepath << Qt::endl;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    in >> m_projectName;

    in >> m_projectColorQSS;

    quint32 sessionCount;
    in >> sessionCount;

    // wip?
    if (m_currentSessionID != 0) {
        qDebug() << "Clearing sessions...";
        m_sessions.clear();
        qDebug() << "Resetting current session ID";
        m_currentSessionID = 0;
    }

    // Session increment itself the int we're giving in order to keep track of the Session ID in a signel place
    while (m_currentSessionID < sessionCount) {
        Session session(m_currentSessionID);
        session.deserialize(in);
        m_sessions.append(session);
    }

    return true;
}

QString& ProjectData::getProjectName()
{
    return m_projectName;
}

void ProjectData::rename(const QString& newProjectName)
{
    if (newProjectName.isEmpty()) {
        qDebug() << "WARNING: Tried to rename project but new project name is empty. Aborting" << Qt::endl;
        return;
    }

    //delete soon-to-be-old file
    deleteSaveFile(); // this fn also takes care of the signal emitting for untracking old project filepath

    QString oldProjectName = m_projectName;
    m_projectName = newProjectName;
    QString oldFilepath = getFilepath();
    QString newFilepath;
    if (oldFilepath == "")
    {
        qDebug() << "WARNING: Renamed a project without a registered filepath. Saving it at default path";
        newFilepath = QString(QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION + newProjectName + "." + Epochpp::DEF_BIN_FILE_EXTENSION);
    }
    else
    {
        QString end(oldProjectName + ".timesheet");
        newFilepath = oldFilepath.remove(end, Qt::CaseInsensitive);
        newFilepath += QString(m_projectName + ".timesheet");
    }

    setFilepath(newFilepath);
    saveToFile();
} 

QString ProjectData::getProjectColorQSS()
{
    return m_projectColorQSS;
}

void ProjectData::setProjectColorQSS(QColor newColor)
{
    m_projectColorQSS = QString("color: rgb(%1, %2, %3)").arg(newColor.red()).arg(newColor.green()).arg(newColor.blue());
    saveToFile();
}
