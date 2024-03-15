#include "../headers/TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
    initSavesDir();

    ui->setupUi(this);

    this->setWindowTitle(APP_NAME);

    connect(ui->newProjectPushButton, &QPushButton::pressed, this, &TimeTrackingApp::askNewProject);
    connect(ui->actionNew, &QAction::triggered, this, &TimeTrackingApp::askNewProject);

    //Test
    //Run below to create projects than run them for some time, stop them (triggering a save) and recomment: it should then load with loadAllProjects();
    /*
    Project* project = new Project("Test project");
    projects.push_back(project);
    Project* project2 = new Project("Very long length line height length project 2");
    projects.push_back(project2);
    Project* project3 = new Project("~~~~~~~~~~~~~~~~~~");
    projects.push_back(project3);
    Project* project4 = new Project("Test project 323");
    projects.push_back(project4);
    Project* project5 = new Project("Test project 552");
    projects.push_back(project5);
    Project* project6 = new Project("Test project 6");
    projects.push_back(project6);
    Project* project7 = new Project("Test project 7");
    projects.push_back(project7);
    */


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
    QString savesDirPath = QDir::currentPath() + DEF_SAVE_LOCATION;
    QFile file(savesDirPath);
    if (!file.open(QIODevice::WriteOnly)) {
        QDir dir;
        if (!dir.mkpath(savesDirPath))
        {
            qDebug() << "ERROR: Couldn't create directory at " << savesDirPath << endl;
            return;
        }
    }
}

void TimeTrackingApp::loadAndDisplayProjects()
{
    QDir savesDir = QDir::currentPath() + DEF_SAVE_LOCATION;
    if (!savesDir.exists()) {
        qWarning() << "Saves directory does not exist (path: " << savesDir << ")" << endl;
        return;
    }

    QString wildcardExt = "*." + QString(DEF_BIN_FILE_EXTENSION);
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
            qWarning() << "ERROR: Project name already used. Aborting new project creation" << endl;
            throwNewMessageBox("Project name already used. Please provide a new, unique name.", QMessageBox::Ok)->exec();
            return;
        }
        
        createProject(userProjectNameInput);
    }
    else {
        qWarning() << "ERROR: Couldn't get new project name. Aborting new project creation" << endl;
    }
}

void TimeTrackingApp::createProject(QString& newProjectName)
{
    QString newProjectColor = ProjectData::newProjectColorQSS();
    ProjectData* newProject = new ProjectData(newProjectName, newProjectColor);
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
}

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

QMessageBox* TimeTrackingApp::throwNewMessageBox(QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton, QString windowTitle)
{
    QMessageBox* msgBox = new QMessageBox(this->centralWidget());

    if (windowTitle == "")
        msgBox->setWindowTitle(APP_NAME);
    else
        msgBox->setWindowTitle(windowTitle);

    msgBox->setText(text);
    msgBox->setStandardButtons(buttons);

    if (defaultButton != QMessageBox::NoButton)
        msgBox->setDefaultButton(defaultButton);

    //msgBox->setIconPixmap(QPixmap(":/icon.png"));

    return msgBox;
}

void TimeTrackingApp::configureStyleSheet()
{
    QFile styleSheetFile(":/qss/default-style.qss");
    if (!styleSheetFile.open(QFile::ReadOnly))
    {
        qDebug() << "Couldn't open QSS file" << endl;
        return;
    }

    QString mainStyleSheet = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    setStyleSheet(mainStyleSheet);
}