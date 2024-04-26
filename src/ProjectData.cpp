#include "ProjectData.h"

//static
int ProjectData::currentProjectCount = 0;

ProjectData::ProjectData(const QString& name) : projectName(name), ID(currentProjectCount)
{
    currentSession = nullptr;
    currentSessionID = 0;
    sessions = QList<Session>();
    currentProjectCount++;
    projectColorQSS = newProjectColorQSS();
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

    currentSession = new Session(currentSessionID);
    currentSession->start();
}

void ProjectData::stop()
{
    if (currentSession == nullptr) {
        qDebug() << "ERROR: Tried to stop current session but project doesn't register any running session. Aborting.";
        return;
    }

    currentSession->end();
    sessions.push_back(*currentSession);
    currentSession = nullptr;

    saveToFile();
}

void ProjectData::deleteSaveFile()
{
    qDebug() << "Deleting file at " << getFilepath();
    QFile f = QFile(getFilepath());
    f.remove();
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

    Session s = Session(currentSessionID, start, end);
    sessions.push_back(s);
    saveToFile();
    return true;
}

int ProjectData::getSessionsCount()
{
    return sessions.count();
}

QDateTime ProjectData::getCurrentSessionStartTime()
{
    return currentSession->getStartDateTime();
}

quint64 ProjectData::getTotalDuration() const
{
    quint64 total = 0;
    for (const Session& session : sessions) {
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
    if (sessions.isEmpty()) {
        return 0;
    }

    int activeDays = 1;

    QDate lastDay = sessions.first().getStartDateTime().date();

    for (Session& s : sessions)
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
    QString filePath = getFilepath();
    qDebug() << "Saving to file " << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "ERROR: Couldn't save file at " << filePath << ". Error: " << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out << projectName;

    out << projectColorQSS;

    out << quint32(sessions.count());

    qDebug() << "Serializing " << quint32(sessions.count()) << " sessions...";

    for (const Session& session : sessions)
    {
        qDebug() << "Session " << session.getID() << " saved";
        session.serialize(out);
    }

    return true;
}

QString ProjectData::getFilepath()
{
    return QString(QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION + projectName + "." + Epochpp::DEF_BIN_FILE_EXTENSION);
}

bool ProjectData::loadFromFile(const QString& filePath)
{
    qDebug() << "Loading from file " << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Couldn't open file at " << filePath << Qt::endl;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    // need better handling? or not at all?
    //qDebug() << "Clearing sessions...";
    //sessions.clear();

    in >> projectName;

    in >> projectColorQSS;

    quint32 sessionCount;
    in >> sessionCount;

    if (currentSessionID != 0) {
        qDebug() << "Resetting current session ID";
        currentSessionID = 0;
    }

    // Session increment itself the int we're giving in order to keep track of the Session ID in a signel place
    while (currentSessionID < sessionCount) {
        Session session(currentSessionID);
        session.deserialize(in);
        sessions.append(session);
    }

    return true;
}

bool ProjectData::loadFromSave(const QString& projectName)
{
    QString path = QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION + projectName;
    return loadFromFile(path);
}

QString& ProjectData::getProjectName()
{
    return projectName;
}

void ProjectData::rename(const QString& newProjectName)
{
    if (newProjectName.isEmpty()) {
        qDebug() << "WARNING: Tried to rename project (data level) but new project name is empty. Aborting" << Qt::endl;
        return;
    }

    //delete soon-to-be-old file
    deleteSaveFile();

    projectName = newProjectName;

    saveToFile();
}

QString ProjectData::getProjectColorQSS()
{
    return this->projectColorQSS;
}

void ProjectData::setProjectColorQSS(QColor newColor)
{
    projectColorQSS = QString("color: rgb(%1, %2, %3)").arg(newColor.red()).arg(newColor.green()).arg(newColor.blue());
    saveToFile();
}
