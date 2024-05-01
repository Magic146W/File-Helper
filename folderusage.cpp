#include "folderusage.h"

FolderUsage::FolderUsage(QObject *parent)
    : QObject{parent}
{}

void FolderUsage::IncrementUsage(const QString &folderPath) {
    usageMap[folderPath]++;

    if(!mostUsedFolders.contains(folderPath)) {
        UpdateMostUsedFolders(folderPath);
    }
}

void FolderUsage::UpdateMostUsedFolders(const QString &folderPath) {
    if (mostUsedFolders.isEmpty()) {
        mostUsedFolders.insert(folderPath);
        return;
    } else {
        auto smallestIt = mostUsedFolders.constBegin();
        for (auto it = mostUsedFolders.constBegin(); it != mostUsedFolders.constEnd(); ++it) {
            if (usageMap[*it] < usageMap[folderPath] && usageMap[*it] < usageMap[*smallestIt]) {
                smallestIt = it;
            }
        }

        if (usageMap[folderPath] > usageMap[*smallestIt]) {
            mostUsedFolders.erase(smallestIt);
            mostUsedFolders.insert(folderPath);
        }
    }
}

int FolderUsage::GetFolderUsage(const QString &folderPath) const {
    return usageMap.value(folderPath, 0);
}

void FolderUsage::UpdateMostUsedFoldersIfNeeded() const {
    if (mostUsedFolders.isEmpty()) {
        QList<QString> keys;
        for (auto it = usageMap.constBegin(); it != usageMap.constEnd(); ++it) {
            keys.append(it.key());
        }

        std::sort(keys.begin(), keys.end(), [&](const QString &a, const QString &b) {
            return usageMap[a] > usageMap[b];
        });

        const int numFolders = std::min(5, static_cast<int>(keys.size()));
        mostUsedFolders.clear();
        for (int i = 0; i < numFolders; ++i) {
            mostUsedFolders.insert(keys[i]);
        }
    }
}

QSet<QString> FolderUsage::GetMostUsedFolders() const {
    UpdateMostUsedFoldersIfNeeded();
    return mostUsedFolders;
}

QMap<QString, int> FolderUsage::GetUsage() const {
    return usageMap;
}

void FolderUsage::SetUsage(const QMap<QString, int> &map) {
    usageMap = map;
}

void FolderUsage::SetMostUsed(const QSet<QString> &set) {
    mostUsedFolders = set;
}

void FolderUsage::ClearAll() {
    usageMap.clear();
    mostUsedFolders.clear();
}

void FolderUsage::ClearFolder(const QString &path) {
    if (usageMap.contains(path)) {
        usageMap.remove(path);

        if(mostUsedFolders.contains(path)) {
            mostUsedFolders.remove(path);
        }
    }
}
