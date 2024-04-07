#include "TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
    initSavesDir();

    ui->setupUi(this);

    Epochpp::setMainWindow(this);

    this->setWindowTitle(Epochpp::APP_NAME);

    //self UI connect

    connect(ui->newProjectPushButton, &QPushButton::pressed, this, &TimeTrackingApp::askNewProject);
    connect(ui->actionNew, &QAction::triggered, this, &TimeTrackingApp::askNewProject);

    //TODO: test


    // UI Layout

    scrollAreaVBoxLayout = new QVBoxLayout(ui->projectsScrollAreaWidgetContents);
    scrollAreaVBoxLayout->setSpacing(10);
    ui->projectsScrollAreaWidgetContents->setLayout(scrollAreaVBoxLayout);

    loadAndDisplayProjects();
    
    scrollAreaVBoxLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    this->setFixedSize(size());
    this->statusBar()->setSizeGripEnabled(false);

    configureStyleSheet();
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
}

void TimeTrackingApp::loadAndDisplayProjects()
{
    QDir savesDir = QDir::currentPath() + Epochpp::DEF_SAVE_LOCATION;
    if (!savesDir.exists()) {
        qWarning() << "Saves directory does not exist (path: " << savesDir << ")" << Qt::endl;
        return;
    }

    QString wildcardExt = "*." + QString(Epochpp::DEF_BIN_FILE_EXTENSION);
    savesDir.setNameFilters(QStringList() << wildcardExt);
    savesDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QFileInfoList fileList = savesDir.entryInfoList();
    for(const QFileInfo & fileInfo : fileList) {
        QString filePath = fileInfo.absoluteFilePath();
        ProjectData* loadedProject = new ProjectData("Unnamed Loaded Project");
        loadedProject->loadFromFile(filePath);
        projects.push_back(loadedProject);
    }

    for (ProjectData* p : projects)
    {
        newProjectWidget(p);
    }
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
    projects.push_back(newProject);
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

void TimeTrackingApp::newProjectWidget(ProjectData* project)
{
    ProjectWidget* newProjectVm = new ProjectWidget(this, project);
    projectsVm.push_back(newProjectVm);
    scrollAreaVBoxLayout->insertWidget(scrollAreaVBoxLayout->count() - 1, newProjectVm);
    qDebug() << "Connecting" << Qt::endl;
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
                                      "Error");
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
