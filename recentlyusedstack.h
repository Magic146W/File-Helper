#ifndef RECENTLYUSEDSTACK_H
#define RECENTLYUSEDSTACK_H

#include <QObject>
#include <QString>
#include <QList>

class RecentlyUsedStack : public QObject
{
    Q_OBJECT
public:
    explicit RecentlyUsedStack(QObject *parent = nullptr);
    void Push(const QString &folderPath);
    QString Pop();
    QString Top() const;
    bool IsEmpty() const;
    int Size() const;
    QList<QString> GetStackList() const;
    void SetStackFromList(const QList<QString> &list);

private:
    static const int maxStackSize = 10;
    QList<QString> stack;

    void ClearRepetitionsInStack(const QString &folderPath);
};

#endif // RECENTLYUSEDSTACK_H
