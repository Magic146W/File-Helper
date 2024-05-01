#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settingkeys.h"
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QSet>

class SettingsManager {
public:
    static void SaveSplitterState(const QByteArray &state);
    static QByteArray LoadSplitterState();

    static void SaveRecentFolders(const QList<QString> &folders);
    static QList<QString> LoadRecentFolders();

    static void SaveFrequentFolders(const QMap<QString, int> &folders);
    static QMap<QString, int> LoadFrequentFolders();

    static void SaveMostUsedFolders(const QSet<QString> &folders);
    static QSet<QString> LoadMostUsedFolders();
};

#endif // SETTINGSMANAGER_H
