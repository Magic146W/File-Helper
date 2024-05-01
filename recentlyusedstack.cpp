#include "recentlyusedstack.h"

RecentlyUsedStack::RecentlyUsedStack(QObject *parent)
    : QObject{parent}
{}

void RecentlyUsedStack::Push(const QString &folderPath) {
    ClearRepetitionsInStack(folderPath);

    if (stack.size() >= maxStackSize) {
        stack.pop_back();
    }
    stack.push_front(folderPath);
}

QString RecentlyUsedStack::Pop() {
    return stack.takeFirst();
}

QString RecentlyUsedStack::Top() const {
    return stack.first();
}

bool RecentlyUsedStack::IsEmpty() const {
    return stack.isEmpty();
}

int RecentlyUsedStack::Size() const {
    return stack.size();
}

QList<QString> RecentlyUsedStack::GetStackList() const {
    return stack;
}

void RecentlyUsedStack::SetStackFromList(const QList<QString> &list) {
    stack = list;
}

void RecentlyUsedStack::ClearRepetitionsInStack(const QString &folderPath) {
    if (stack.contains(folderPath)) {
        stack.removeAll(folderPath);
    }
}
