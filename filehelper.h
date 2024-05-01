#ifndef FILEHELPER_H
#define FILEHELPER_H

#include "recentlyusedstack.h"
#include "folderusage.h"
#include "settingkeys.h"
#include "settingsmanager.h"
#include <QMainWindow>
#include <QDir>
#include <QFileSystemModel>
#include <QSettings>
#include <QSplitter>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class FileHelper;
}
QT_END_NAMESPACE

class FileHelper : public QMainWindow
{
    Q_OBJECT

public:
    FileHelper(QWidget *parent = nullptr);
    ~FileHelper();

private slots:
    void on_pushButton_FolderAdd_clicked();
    void on_pushButton_FolderDelete_clicked();
    void on_pushButton_FilesDelete_clicked();
    void on_pushButton_ViewList_clicked();
    void on_pushButton_ViewGrid_clicked();
    void on_pushButton_MoreRecentFiles_clicked();
    void on_pushButton_MostUsed_clicked();
    void on_pushButton_FolderRecent_clicked();

    void closeEvent(QCloseEvent *event);
    void HandleSplitterMoved(int pos, int index);
    void OnFolderOpened(const QModelIndex &index);

private:
    Ui::FileHelper *ui;

    void AddSplitterWidget();
    void PerformStartupSetup();
    void SaveSettings();
    void LoadSettings();

    enum class ViewModelScope {
        FolderQTreeView,
        FileQListView,
        RecentFolderQListView,
        RecentFileQListView,
        MostUsedFolderQListView,
        MostUsedFileQListView
    };

    QFileSystemModel* PrepareViewModel(const ViewModelScope &scope);

    void PopulateFolderTreeView();
    void PopulateRecentFoldersListView();

    QFileSystemModel* CreateCurrentViewModel(QSharedPointer<QSettings> savedModelData);
    QFileSystemModel* CreateRecentViewModel(QSharedPointer<QSettings> savedModelData);
    QFileSystemModel* CreateMostUsedViewModel(QSharedPointer<QSettings> savedModelData);

    void SaveSplitter();
    void SaveFrequentFoldersData();
    void SaveRecentFoldersData();
    void SaveMostUsedFoldersData();

    void UpdateOpenedFolders(const QString &path);

    void LoadSplitter(QSharedPointer<QSettings> savedModelData);
    QList<QString> LoadRecentFoldersData(QSharedPointer<QSettings> savedModelData);
    QSet<QString> LoadFrequentFoldersData(QSharedPointer<QSettings> savedModelData);
    QSet<QString> LoadMostUsedFoldersData(QSharedPointer<QSettings> savedModelData);

    QFileSystemModel* DefaultModel();
    QSharedPointer<QSettings> LoadSettings(const QString &scope);

    bool IsPathEmpty(const QString &path);

    RecentlyUsedStack* recentItemStack;
    FolderUsage* usedFolders;
    QSplitter* mySplitter;

};
#endif // FILEHELPER_H
