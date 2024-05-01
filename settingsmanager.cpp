#include "settingsmanager.h"
#include <QSettings>

auto ToQString = [](SettingKeys key) {
    QString keyString;
    switch (key) {
    case SettingKeys::FileHelper:
        keyString = "FileHelper";
        break;
    case SettingKeys::Splitter:
        keyString = "Splitter";
        break;
    case SettingKeys::RecentFolders:
        keyString = "RecentFolderQListView";
        break;
    case SettingKeys::UsedFolders:
        keyString = "UsedFolderQListView";
        break;
    case SettingKeys::FrequentFolders:
        keyString = "FrequentFolders";
        break;
    case SettingKeys::MostUsedFolders:
        keyString = "MostUsedFolders";
        break;
    }
    return keyString;
};

#include "settingsmanager.h"
#include "settingkeys.h"

void SettingsManager::SaveSplitterState(const QByteArray &state) {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::Splitter));
    settings.setValue(ToQString(SettingKeys::Splitter), state);
}

QByteArray SettingsManager::LoadSplitterState() {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::Splitter));
    return settings.value(ToQString(SettingKeys::Splitter)).toByteArray();
}

void SettingsManager::SaveRecentFolders(const QList<QString> &folders) {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::RecentFolders));
    settings.setValue(ToQString(SettingKeys::RecentFolders), folders);
}

QList<QString> SettingsManager::LoadRecentFolders() {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::RecentFolders));
    return settings.value(ToQString(SettingKeys::RecentFolders)).value<QList<QString>>();
}

void SettingsManager::SaveFrequentFolders(const QMap<QString, int> &folders) {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::UsedFolders));
    settings.setValue(ToQString(SettingKeys::FrequentFolders), QVariant::fromValue(folders));
}

QMap<QString, int> SettingsManager::LoadFrequentFolders() {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::UsedFolders));
    return settings.value(ToQString(SettingKeys::FrequentFolders)).value<QMap<QString, int>>();
}

void SettingsManager::SaveMostUsedFolders(const QSet<QString> &folders) {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::UsedFolders));
    settings.setValue(ToQString(SettingKeys::MostUsedFolders), QVariant::fromValue(folders));
}

QSet<QString> SettingsManager::LoadMostUsedFolders() {
    QSettings settings(ToQString(SettingKeys::FileHelper), ToQString(SettingKeys::MostUsedFolders));
    return settings.value(ToQString(SettingKeys::MostUsedFolders)).value<QSet<QString>>();
}

