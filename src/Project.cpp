#include "../headers/Project.h"

Project::Project(int ID, const QString& name) : projectName(name), ID(ID)
{
    currentSession = nullptr;
    currentSessionID = 0;
    sessions = QList<Session>();
}

Project::~Project()
{
    // Save file on destruction?
}

void Project::start()
{
    if (currentSession != nullptr) {
        qDebug() << "ERROR: Tried to start a new session but project already have a running session registered. Aborting.";
        return;
    }

    currentSession = new Session(currentSessionID);
    currentSession->start();
}

void Project::stop()
{
    if (currentSession == nullptr) {
        qDebug() << "ERROR: Tried to stop current session but project doesn't register any running session. Aborting.";
        return;
    }

    currentSession->end();
    addSession(*currentSession);
    currentSession = nullptr;
    currentSessionID++;
}

bool Project::isRunning()
{
    return currentSession != nullptr;
}

void Project::addSession(const Session& session)
{
    sessions.append(session);
}

QDateTime Project::getCurrentSessionStartTime()
{
    return currentSession->getStartDateTime();
}

quint64 Project::getTotalDuration() const
{
    quint64 total = 0;
    for (const Session& session : sessions) {
        total += session.getDuration();
    }
    return total;
}

QString Project::getPrettyTotalDuration() const
{
    quint64 seconds = getTotalDuration();
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;

    return QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0'));
}

bool Project::saveToFile(const QString& filePath)
{
    QString finalFilePath = filePath + "." + DEF_BIN_FILE_EXTENSION;
    qDebug() << "Saving to file " << finalFilePath;
    QFile file(finalFilePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out << projectName;

    out << quint32(sessions.count());

    qDebug() << "Serializing " << quint32(sessions.count()) << " sessions...";

    for (const Session& session : sessions)
    {
        session.serialize(out);
    }

    return true;
}

bool Project::loadFromFile(const QString& filePath)
{
    qDebug() << "Loading from file " << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Couldn't open file at " << filePath;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    qDebug() << "Clearing sessions...";
    sessions.clear();

    in >> projectName;

    quint32 sessionCount;
    in >> sessionCount;

    for (int i = 0; i < sessionCount; ++i) {
        Session session(i);
        session.deserialize(in);
        sessions.append(session);
    }

    return true;
}

QString& Project::getProjectName()
{
    return projectName;
}
