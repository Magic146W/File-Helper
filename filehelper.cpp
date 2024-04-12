#include "filehelper.h"
#include "./ui_filehelper.h"

FileHelper::FileHelper(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileHelper)
{
    ui->setupUi(this);
}

FileHelper::~FileHelper()
{
    delete ui;
}
