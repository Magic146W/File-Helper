#ifndef FOLDERUSAGE_H
#define FOLDERUSAGE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSettings>
#include <QList>

class FolderUsage : public QObject
{
    Q_OBJECT
public:
    explicit FolderUsage(QObject *parent = nullptr);

    FolderUsage(const QString &settingsFile);

    void IncrementUsage(const QString &folderPath);
    QMap<QString, int> GetUsage() const;
    int GetFolderUsage(const QString &folderPath) const;
    QSet<QString> GetMostUsedFolders() const;
    void ClearFolder(const QString &path);
    void ClearAll();
    void SetUsage(const QMap<QString, int> &map);
    void SetMostUsed(const QSet<QString> &set);

private:
    QMap<QString, int> usageMap;
    mutable QSet<QString> mostUsedFolders;

    void UpdateMostUsedFolders(const QString &folderPath);
    void UpdateMostUsedFoldersIfNeeded() const;
};

#endif // FOLDERUSAGE_H
