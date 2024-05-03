#include "ProjectData.h"

//static
int ProjectData::currentProjectCount = 0;

ProjectData::ProjectData(const QString& name, const QString& filepath) : projectName(name), filepath(filepath), ID(currentProjectCount)
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
    QString filepath = getFilepath();
    qDebug() << "Deleting file at " << filepath;
    QFile f = QFile(filepath);
    f.remove();

    emit deletedFile(filepath);
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
    QString filepath = getFilepath();
    qDebug() << "Saving to file " << filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "ERROR: Couldn't save file at " << filepath << ". Error: " << file.errorString();
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

    emit newOpenedFile(filepath);

    return true;
}

QString ProjectData::getFilepath()
{
    if (filepath == "")
    {
        qDebug() << "ERROR! No filepath registered for project " << projectName;
        // setting default filepath
        setFilepath(QString(QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION + projectName + "." + Epochpp::DEF_BIN_FILE_EXTENSION));
    } else if(!QFile::exists(filepath)) {
        qDebug() << "ERROR! Couldn't reach registered path at " << filepath << " for "<< projectName;
        // setting default filepath
        setFilepath(QString(QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION + projectName + "." + Epochpp::DEF_BIN_FILE_EXTENSION));
    }
    return filepath;
}

void ProjectData::setFilepath(QString& filepath)
{
    this->filepath = filepath;
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

    /*
    in >> this->filepath;

    if (this->filepath != filepath)
    {
        qDebug() << "ERROR: loaded filepath is different from the real filepath the file is being loaded from";
        qDebug() << "current filepath: " << filepath;
        qDebug() << "loaded filepath: " << this->filepath;
        qDebug() << "overwriting filepath with current filepath";
        this->filepath = filepath;
    }
    */

    in >> projectName;

    in >> projectColorQSS;

    quint32 sessionCount;
    in >> sessionCount;

    // wip?
    if (currentSessionID != 0) {
        qDebug() << "Clearing sessions...";
        sessions.clear();
        qDebug() << "Resetting current session ID";
        currentSessionID = 0;
    }

    // Session increment itself the int we're giving in order to keep track of the Session ID in a signel place
    while (currentSessionID < sessionCount) {
        Session session(currentSessionID);
        session.deserialize(in);
        sessions.append(session);
    }

    saveToFile();

    return true;
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
