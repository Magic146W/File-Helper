#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QMainWindow>

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

private:
    Ui::FileHelper *ui;
};
#endif // FILEHELPER_H
