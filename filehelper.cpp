#include "filehelper.h"
#include "./ui_filehelper.h"

FileHelper::FileHelper(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileHelper)
    , recentItemStack(new RecentlyUsedStack())
    , usedFolders(new FolderUsage())
    , mySplitter(new QSplitter())
{
    ui->setupUi(this);

    connect(ui->treeView_Folder, &QTreeView::activated, this, &FileHelper::OnFolderOpened);


    PerformStartupSetup();
}

FileHelper::~FileHelper() {
    delete ui;
}

void FileHelper::PerformStartupSetup() {
    PopulateFolderTreeView();
    PopulateRecentFoldersListView();
    AddSplitterWidget();
}

void FileHelper::closeEvent(QCloseEvent *event) { 
    SaveSettings();

    QMainWindow::closeEvent(event);
}

void FileHelper::SaveSettings()
{
    SettingsManager::SaveSplitterState(mySplitter->saveState());
    SettingsManager::SaveRecentFolders(recentItemStack->GetStackList());

}

void FileHelper::LoadSettings()
{

}

void FileHelper::AddSplitterWidget()
{
    QWidget *widgetLeftPanel = ui->widget_LeftPanel;
    QWidget *widgetRightPanel = ui->widget_RightPanel;

    mySplitter = new QSplitter(Qt::Horizontal, this);

    mySplitter->addWidget(widgetLeftPanel);
    mySplitter->addWidget(widgetRightPanel);

    ui->widget_Main->layout()->addWidget(mySplitter);

    connect(mySplitter, &QSplitter::splitterMoved, this, &FileHelper::HandleSplitterMoved);
}

void FileHelper::HandleSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);

    // ModifiedFileSystemModel *model = qobject_cast<ModifiedFileSystemModel*>(ui->QListView_FileViewer->model());
    // if (model)
    // {
    //     ui->QListView_FileViewer->setModel(nullptr);
    //     ui->QListView_FileViewer->setModel(model);
    // }
}


void FileHelper::SaveRecentFoldersData()
{
    QList<QString> recentFoldersList;
    recentFoldersList = recentItemStack->GetStackList();

    QSettings settings("FileHelper", "RecentFolderQListView");
    settings.setValue("RecentFoldersList", recentFoldersList);
}

void FileHelper::SaveFrequentFoldersData()
{
    QMap<QString, int> frequentFoldersMap = usedFolders->GetUsage();
    QVariant variantFrequent = QVariant::fromValue(frequentFoldersMap);

    QSettings settings("FileHelper", "UsedFolderQListView");
    settings.setValue("FrequentFoldersMap", variantFrequent);
}

void FileHelper::SaveMostUsedFoldersData()
{
    QSet<QString> mostUsedFoldersMap = usedFolders->GetMostUsedFolders();
    QVariant variantMostUsed = QVariant::fromValue(mostUsedFoldersMap);

    QSettings settings("FileHelper", "UsedFolderQListView");
    settings.setValue("FrequentFoldersMap", variantMostUsed);
}

void FileHelper::LoadSplitter(QSharedPointer<QSettings> savedModelData)
{
    QSettings splitterSize("FileHelper", "Splitter");
    QByteArray splitterState = splitterSize.value("SplitterSizes").toByteArray();

    if (!splitterState.isEmpty())
    {
        mySplitter->restoreState(splitterState);
    }
}

QList<QString> FileHelper::LoadRecentFoldersData(QSharedPointer<QSettings> savedModelData) {
    QVariant recentFolders = savedModelData->value("RecentFoldersList");

    QList<QString> recentFoldersList = recentFolders.value<QList<QString>>();
    recentItemStack->SetStackFromList(recentFoldersList);

    return recentFoldersList;
}

QSet<QString> FileHelper::LoadFrequentFoldersData(QSharedPointer<QSettings> savedModelData) {
    QVariant usageVariantMap = savedModelData->value("FrequentFoldersMap");
    QMap<QString, int> usageMap = usageVariantMap.value<QMap<QString, int>>();
    usedFolders->SetUsage(usageMap);

    return LoadMostUsedFoldersData(savedModelData);
}

QSet<QString> FileHelper::LoadMostUsedFoldersData(QSharedPointer<QSettings> savedModelData)
{
    QVariant mostUsedVariantMap = savedModelData->value("MostUsedFolders");
    usedFolders->SetMostUsed(mostUsedVariantMap.value<QSet<QString>>());
    return mostUsedVariantMap.value<QSet<QString>>();
}




void FileHelper::PopulateFolderTreeView() {
    QFileSystemModel* model = PrepareViewModel(ViewModelScope::FolderQTreeView);
    if(model == nullptr) {
        model = DefaultModel();
    }

    ui->treeView_Folder->setModel(model);
}

void FileHelper::PopulateRecentFoldersListView() {
    QFileSystemModel* model = PrepareViewModel(ViewModelScope::RecentFolderQListView);
    if(model == nullptr) {
        return;
    }

    ui->listView_RecentFolders->setModel(model);
}


QFileSystemModel* FileHelper::PrepareViewModel(const ViewModelScope &scope) {
    QString scopeName;
    QSharedPointer<QSettings> savedModelData;

    switch (scope) {
    case ViewModelScope::FolderQTreeView:
        scopeName = "FolderQTreeView";
        return CreateCurrentViewModel(LoadSettings(scopeName));
        break;

    case ViewModelScope::FileQListView:
        scopeName = "FileQListView";
        return CreateCurrentViewModel(LoadSettings(scopeName));
        break;

    case ViewModelScope::RecentFolderQListView:
        scopeName = "RecentFolderQListView";
        return CreateRecentViewModel(LoadSettings(scopeName));
        break;

    case ViewModelScope::RecentFileQListView:
        scopeName = "RecentFileQListView";
        return CreateRecentViewModel(LoadSettings(scopeName));
        break;

    case ViewModelScope::MostUsedFolderQListView:
        scopeName = "UsedFolderQListView";
        return CreateMostUsedViewModel(LoadSettings(scopeName));
        break;

    case ViewModelScope::MostUsedFileQListView:
        scopeName = "UsedFileQListView";
        return CreateMostUsedViewModel(LoadSettings(scopeName));
        break;
    }

    return nullptr;
}

QFileSystemModel* FileHelper::CreateCurrentViewModel(QSharedPointer<QSettings> savedModelData) {
    if(savedModelData->allKeys().empty()) {
        return nullptr;
    }

    QString rootPath = savedModelData->value("RootPath").toString();
    if (IsPathEmpty(rootPath)) {
        return nullptr;
    }

    QFileSystemModel* model = new QFileSystemModel(this);
    model->setRootPath(rootPath);

    return model;
}

QFileSystemModel* FileHelper::CreateRecentViewModel(QSharedPointer<QSettings> savedModelData) {
    if(savedModelData->allKeys().empty()) {
        return nullptr;
    }

    QStringList recentlyUsedList = savedModelData->value("RecentlyUsed").toStringList();
    for (const QString &item : recentlyUsedList) {
        recentItemStack->Push(item);
    }

    QFileSystemModel* model = new QFileSystemModel(this);
    for (const QString &path : recentlyUsedList) {
        model->setRootPath(path);
    }

    return model;
}

QFileSystemModel* FileHelper::CreateMostUsedViewModel(QSharedPointer<QSettings> savedModelData) {
    if (savedModelData->allKeys().empty()) {
        return nullptr;
    }

    QSet<QString> mostUsedFolders = LoadFrequentFoldersData(savedModelData);

    QFileSystemModel* model = new QFileSystemModel(this);
    for (const QString &path : mostUsedFolders) {
        model->setRootPath(path);
    }

    //need to override double click function to load onto file list view!

    return model;
}

void FileHelper::OnFolderOpened(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    QFileSystemModel* model = qobject_cast<QFileSystemModel*>(ui->treeView_Folder->model());
    if (!model) {
        return;
    }

    QString folderPath = model->filePath(index);
    QFileInfo fileInfo(folderPath);
    if (!fileInfo.isDir()) {
        return;
    }

    UpdateOpenedFolders(folderPath);
}

void FileHelper::UpdateOpenedFolders(const QString &path) {
    qDebug() << "enter saveOpenedFolder!";
    recentItemStack->Push(path);
    usedFolders->IncrementUsage(path);
}

QSharedPointer<QSettings> FileHelper::LoadSettings(const QString &scope) {
    QSharedPointer<QSettings> settings(new QSettings("FileHelper", scope));
    return settings;
}

QFileSystemModel* FileHelper::DefaultModel() {
    QFileSystemModel* model = new QFileSystemModel(this);
    model->setRootPath(QDir::rootPath());
    return model;
}

bool FileHelper::IsPathEmpty(const QString &path) {
    if(path.isEmpty()) {
        return true;
    }

    return false;
}


void FileHelper::on_pushButton_FolderAdd_clicked() {
    // Adds folder
    // need to access current folder from treeview and create a new one within
}

void FileHelper::on_pushButton_FolderDelete_clicked() {
    // Removes folder
    // try to access pressed folder from treeview and delete it
    // dialog showing current number of files inside
}

void FileHelper::on_pushButton_FilesDelete_clicked() {
    // Removes folder
    // try to access pressed file from listview and delete it
}

void FileHelper::on_pushButton_ViewList_clicked() {
    //Changes to list view type of view
}

void FileHelper::on_pushButton_ViewGrid_clicked() {
    //Changes to grid view type of view
}

void FileHelper::on_pushButton_MoreRecentFiles_clicked() {
    //Gives more recent files... where?
}

void FileHelper::on_pushButton_MostUsed_clicked() {
    //switches view to most used (might add stared folders)
}

void FileHelper::on_pushButton_FolderRecent_clicked() {
    //switches view to recently used
}

