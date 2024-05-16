#include "TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
    // UI
    // ===================================
    ui->setupUi(this);

    Epochpp::setMainWindow(this);

    this->setWindowTitle(Epochpp::APP_NAME);

    // self UI connect
    connect(ui->newProjectPushButton, &QPushButton::pressed, this, &TimeTrackingApp::askNewProject);
    connect(ui->actionNew, &QAction::triggered, this, &TimeTrackingApp::askNewProject);
    connect(ui->actionLoad_Project, &QAction::triggered, this, &TimeTrackingApp::loadSingleProject);
    connect(ui->actionLoad_All_Projects_From, &QAction::triggered, this, &TimeTrackingApp::loadAllProjectsFrom);
    connect(ui->actionAbout, &QAction::triggered, this, &TimeTrackingApp::displayAbout);
    connect(ui->actionHelp, &QAction::triggered, this, &TimeTrackingApp::displayHelp);
    connect(ui->actionReport_a_Bug, &QAction::triggered, this, &TimeTrackingApp::displayReportBug);

    // UI Layout

    scrollAreaVBoxLayout = new QVBoxLayout(ui->projectsScrollAreaWidgetContents);
    scrollAreaVBoxLayout->setSpacing(10);
    ui->projectsScrollAreaWidgetContents->setLayout(scrollAreaVBoxLayout);
    
    scrollAreaVBoxLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    this->setFixedSize(size());
    this->statusBar()->setSizeGripEnabled(false);

    configureStyleSheet();

    // DATA
    // ===================================
    initSavesDir();
    initOpenedFiles();
    loadAllOpenedProjects();
}

TimeTrackingApp::~TimeTrackingApp()
{
    for (ProjectWidget* projectVm : projectsVm)
    {
        delete projectVm;
    }

    for (ProjectData* project : projects)
    {
        delete project;
    }

    delete scrollAreaVBoxLayout;
    delete ui;
}

void TimeTrackingApp::initOpenedFiles()
{
    QFile file(Epochpp::CFG_FILE_PATH);

    // Check if the file exists and has data
    if (file.exists() && file.size() > 0) {
        if (!file.open(QIODevice::ReadWrite)) {
            qDebug() << "Failed to open config file in RW mode";
            return;
        }

        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull() || !doc.isObject()) {
            qDebug() << "Config file is corrupted: can't parse";
            file.close();
            return;
        }

        QJsonObject obj = doc.object();
        if (!obj.contains("openedFiles")) {
            obj.insert("openedFiles", QJsonArray());
            doc.setObject(obj);
            file.seek(0);  // Rewind the file to the beginning
            file.write(doc.toJson(QJsonDocument::Indented));
        }
    }
    else {
        // File does not exist or is empty, create and initialize it
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open new file for writing.";
            return;
        }
        QJsonObject obj;
        obj.insert("openedFiles", QJsonArray());
        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
    }

    file.close();
}

void TimeTrackingApp::addToOpenedFiles(const QString& projectFilepath)
{
    QFile file(Epochpp::CFG_FILE_PATH);

    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open config file in RW mode";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Config file is corrupted: can't parse";
        file.close();
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray openedFilesArray;

    // Check if "openedFiles" key exists and is an array, otherwise create it
    if (obj.contains("openedFiles") && obj["openedFiles"].isArray()) {
        openedFilesArray = obj["openedFiles"].toArray();
    }
    else {
        obj.insert("openedFiles", QJsonArray());
    }

    // Append the new file path if it's not already in the array
    if (!openedFilesArray.contains(projectFilepath)) {
        openedFilesArray.append(projectFilepath);
        obj["openedFiles"] = openedFilesArray;

        // Update the JSON document with the new object
        doc.setObject(obj);
        file.seek(0); // Go back to the start of the file
        file.write(doc.toJson(QJsonDocument::Indented));
        file.resize(file.pos()); // Truncate any remaining data from previous content
    }

    file.close();
}

void TimeTrackingApp::initSavesDir()
{
    QString savesDirPath = QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION;
    QFile file(savesDirPath);
    if (!file.open(QIODevice::WriteOnly)) {
        QDir dir;
        if (!dir.mkpath(savesDirPath))
        {
            qDebug() << "ERROR: Couldn't create directory at " << savesDirPath << Qt::endl;
            return;
        }
    }
    // necessary?
    file.close();
}

// private fn for loading and displaying projects inside a directory
// not linked directly to an action
void TimeTrackingApp::loadFrom(QDir savesDir)
{
    QString wildcardExt = "*." + QString(Epochpp::DEF_BIN_FILE_EXTENSION);
    savesDir.setNameFilters(QStringList() << wildcardExt);
    savesDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QFileInfoList fileList = savesDir.entryInfoList();
    for (const QFileInfo& fileInfo : fileList) {
        QString filepath = fileInfo.absoluteFilePath();
        loadAt(filepath);
    }
}

// private fn responsible for the loading and displaying of a project
// not linked directly to an action
void TimeTrackingApp::loadAt(const QString filepath, bool trackingCheckEnabled)
{
    // Check if project isn't already loaded
    if (trackingCheckEnabled && isProjectAlreadyTracked(filepath)) {
        qDebug() << "Can't load project: " << filepath << " is already loaded";
        return;
    }

    ProjectData* loadedProject = new ProjectData("Unnamed Loaded Project");
    bool loadedSuccessfully = loadedProject->loadFromFile(filepath);
    if (loadedSuccessfully)
    {
        connect(loadedProject, &ProjectData::newOpenedFile, this, &TimeTrackingApp::addToOpenedFiles);
        connect(loadedProject, &ProjectData::deletedFile, this, &TimeTrackingApp::untrackProject);
        projects.push_back(loadedProject);
        newProjectWidget(loadedProject);
    }
    else
    {
        qDebug() << "Couldn't load project at " << filepath;
        delete loadedProject;
    }
}

// load all projects found in the config file ; used when launching the app
void TimeTrackingApp::loadAllOpenedProjects()
{
    QFile file(Epochpp::CFG_FILE_PATH);

    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open config file in RW mode";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Config file is corrupted: can't parse";
        file.close();
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray openedFilesArray;

    // Check if "openedFiles" key exists and is an array, otherwise abort process and load all projects from the default directory
    if (!(obj.contains("openedFiles") && obj["openedFiles"].isArray()))
    {
        qDebug() << "Couldn't load projects from config file! Loading projects from default save location...";
        loadProjectsFromDefaultDirectory();
    }

    openedFilesArray = obj["openedFiles"].toArray();

    for (int i = 0; i < openedFilesArray.size(); ++i) {
        QJsonValue value = openedFilesArray.at(i);
        QString filepath = value.toString();
        loadAt(filepath, false);
    }

    file.close();
}

// [! ONLY USED IN CASE THE LOADING MECHANISM DOESNT WORK !]
// THE FUNCTION ABOVE IS NOW RESPONSIBLE FOR LOADING ALL PROJECTS FOUND IN THE CONFIG FILE
// private fn for loading and displaying projects located in the default directory where .timesheet are saved
// not linked to an action
void TimeTrackingApp::loadProjectsFromDefaultDirectory()
{
    QDir savesDir = QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION;
    if (!savesDir.exists()) {
        qWarning() << "Saves directory does not exist (path: " << savesDir << ")" << Qt::endl;
        return;
    }

    loadFrom(savesDir);
}

// for action "actionLoad_All_Projects_From" (menuBar)
void TimeTrackingApp::loadAllProjectsFrom()
{
    // WIP!!!
    // -> ADD THE DIRECTORY WHERE THE PROJECTS WERE ORIGINALLY SAVED TO A LIST OF DIRECTORIES TO LOAD FROM

    QString inputPath = QFileDialog::getExistingDirectory(nullptr, QObject::tr("Select Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (inputPath.isEmpty())
    {
        qDebug() << "Can't load projects from " << inputPath;
        return;
    }

    QDir savesDir(inputPath);
    
    loadFrom(savesDir);
}

// for action "actionLoad" (menuBar)
void TimeTrackingApp::loadSingleProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Project"), "",
        tr("TIMESHEET (*.timesheet, *.TIMESHEET)"));

    if (fileName.isEmpty())
    {
        qDebug() << "Can't load project at " << fileName;
        return;
    }

    loadAt(fileName);
}

void TimeTrackingApp::askNewProject()
{
    bool ok;
    QString userProjectNameInput = QInputDialog::getText(this, tr("New Project"),
        tr("Project Name:"), QLineEdit::Normal,
        "Unnamed Project", &ok);

    if (ok && !userProjectNameInput.isEmpty()) {
        if (!isProjectNameUnique(userProjectNameInput)) {
            qWarning() << "ERROR: Project name already used. Aborting new project creation" << Qt::endl;
            MsgBoxGen::throwNewMessageBox(this, "Project name already used. Please provide a new, unique name.", QMessageBox::Ok)->exec();
            return;
        }
        
        createProject(userProjectNameInput);
    }
    else {
        qWarning() << "ERROR: Couldn't get new project name. Aborting new project creation" << Qt::endl;
    }
}

void TimeTrackingApp::createProject(const QString &newProjectName)
{
    ProjectData* newProject = new ProjectData(newProjectName);
    connect(newProject, &ProjectData::newOpenedFile, this, &TimeTrackingApp::addToOpenedFiles);
    projects.push_back(newProject);
    newProject->saveToFile();
    newProjectWidget(newProject);
}

bool TimeTrackingApp::isProjectNameUnique(QString& testProjectName)
{
    for (ProjectData* p : projects)
    {
        if (p->getProjectName() == testProjectName) {
            return false;
        }
    }

    return true;
}

bool TimeTrackingApp::isProjectAlreadyTracked(const QString& filepath)
{
    QFile file(Epochpp::CFG_FILE_PATH);

    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open config file in RW mode";
        return false;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Config file is corrupted: can't parse";
        file.close();
        return false;
    }

    QJsonObject obj = doc.object();
    QJsonArray openedFilesArray;

    // Check if "openedFiles" key exists and is an array, otherwise abort process and return false
    if (!(obj.contains("openedFiles") && obj["openedFiles"].isArray())) {
        qDebug() << "ERROR! Couldn't read openedFiles JSON data";
        file.close();
        return false;
    }

    openedFilesArray = obj["openedFiles"].toArray();

    for (int i = 0; i < openedFilesArray.size(); ++i) {
        QJsonValue value = openedFilesArray.at(i);
        if (value.isString()) {
            QString i_filepath = value.toString();

            if (i_filepath == filepath) {
                qDebug() << "ERROR! Project is already tracked:" << filepath;
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

void TimeTrackingApp::newProjectWidget(ProjectData* project)
{
    ProjectWidget* newProjectVm = new ProjectWidget(this, project);
    projectsVm.push_back(newProjectVm);
    scrollAreaVBoxLayout->insertWidget(scrollAreaVBoxLayout->count() - 1, newProjectVm);
    connect(newProjectVm, &ProjectWidget::requestProjectDeletion, this, &TimeTrackingApp::deleteProject);
}

void TimeTrackingApp::deleteProject(int projectID)
{
    auto widgetIt = std::find_if(projectsVm.begin(), projectsVm.end(), [projectID](ProjectWidget* widget) {
        return widget->project->ID == projectID;
    });
    if (widgetIt != projectsVm.end()) {
        ProjectWidget* widget = *widgetIt;
        scrollAreaVBoxLayout->removeWidget(widget);
        widget->hide();
        widget->deleteLater();
        projectsVm.erase(widgetIt);
    } else {
        MsgBoxGen::throwNewMessageBox(Epochpp::g_mainWindow,
                                      QString("Couldn't delete project"),
                                      QMessageBox::Ok,
                                      QMessageBox::Ok,
                                      "Error!");
        return;
    }

    auto it = std::find_if(projects.begin(), projects.end(), [projectID](ProjectData* project) {
        return project->ID == projectID;
    });
    if (it != projects.end()) {
        (*it)->deleteSaveFile();
        delete *it;
        projects.erase(it);
    }
}

// TODO: write better debug messages + msgbox
void TimeTrackingApp::untrackProject(const QString& filepath)
{
    QFile file(Epochpp::CFG_FILE_PATH);

    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open config file in RW mode";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Config file is corrupted: can't parse";
        file.close();
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray openedFilesArray;

    QJsonArray newOpenedFilesArray;
    bool found = false;
    for (int i = 0; i < openedFilesArray.size(); ++i) {
        QJsonValue value = openedFilesArray.at(i);
        if (value.isString() && value.toString() != filepath) {
            newOpenedFilesArray.append(value);
        }
        else {
            found = true;
        }
    }

    if (!found) {
        qDebug() << "File path not found in the tracking list:" << filepath;
        file.close();
        return;
    }

    // Update the JSON object with the new array
    obj["openedFiles"] = newOpenedFilesArray;

    // Rewrite the modified JSON back to the file
    file.resize(0); // Truncate the file first to ensure clean write
    QJsonDocument newDoc(obj);
    file.write(newDoc.toJson());
    file.close();
}

/*
void TimeTrackingApp::removeProjectWidgets()
{
    QLayoutItem* child;
    while ((child = scrollAreaVBoxLayout->takeAt(0)) != nullptr) {

        delete child->widget(); // delete the widget
        delete child;           // delete the layout item
    }
}

void TimeTrackingApp::refreshProjectsDisplay()
{
    removeProjectWidgets();
}
*/

void TimeTrackingApp::configureStyleSheet()
{
    QFile styleSheetFile(":/qss/default-style.qss");
    if (!styleSheetFile.open(QFile::ReadOnly))
    {
        qDebug() << "Couldn't open QSS file" <<  Qt::endl;
        return;
    }

    QString mainStyleSheet = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    setStyleSheet(mainStyleSheet);
}

void TimeTrackingApp::displayAbout()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Epoch++");
    aboutBox.setText(
        "<p>Epoch++ is a free, open-source software licensed under GPL3, developed using the Qt framework</p>"
        "<p>Epoch++ aims to help you easily and locally track your progress on projects and hobbies.</p>"
        "<p>Bug? Missing feature? Want to improve the software or report something?</p>"
        "<p>You can contribute on the GitHub page where the project is hosted: "
        "</br><a href='https://github.com/aseiis/epochplusplus'>https://github.com/aseiis/epochplusplus</a></p>"
        );

    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setStandardButtons(QMessageBox::Ok);

    aboutBox.exec();
}

void TimeTrackingApp::displayHelp()
{
    QMessageBox helpBox;
    helpBox.setWindowTitle("Help");
    helpBox.setText(
        "<h3>How to use Epoch++?</h3>"
        "<p>Epoch++ let you create new projects and track the time you've spent on them.</p>"
        "<p>To create your first project, click on the 'New Project' button."
        "Now your project has been created: you can see the project card displaying basic information about it: the time you spent on the current session, "
        "when a session is running, as well as the total time spent on that project</p>"
        "<p>You can now start the first session of your project by pressing the play button. It'll start a session that you can stop at any time.</p>"
        "<p>During the session, its duration is displayed on the project card. "
        "After you've stopped your session, the data of this session will be added to the project statistics.</p>"
        "<p>To see more informations and statistics about a specific project, click on the 'More...' button</p>"
    );

    helpBox.setTextFormat(Qt::RichText);
    helpBox.setStandardButtons(QMessageBox::Ok);

    helpBox.exec();
}

void TimeTrackingApp::displayReportBug()
{
    QMessageBox reportBugBox;
    reportBugBox.setWindowTitle("Report a bug");
    reportBugBox.setText(
        "<p>If you witnessed a bug or have any suggestion about the software, you can open an issue on the GitHub page where Epoch++ is hosted:</p>"
        "<p><a href='https://github.com/aseiis/epochplusplus'>https://github.com/aseiis/epochplusplus</a></p>"
    );

    reportBugBox.setTextFormat(Qt::RichText);
    reportBugBox.setStandardButtons(QMessageBox::Ok);

    reportBugBox.exec();
}