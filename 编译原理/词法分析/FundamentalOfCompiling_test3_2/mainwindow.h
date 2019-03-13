#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QChar>

#include <iostream>
#include <string>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Initialize();
    void filterResource();
    int searchReserve(QString s);
    void Scanner();

    ~MainWindow();

private slots:
    void on_Import_file_triggered();

    void on_actionpreprocessing_triggered();

    void on_actiontokeb_sequence_triggered();

private:
    Ui::MainWindow *ui;
    QChar resourceProject[10000];//保存文件字符集合
    int pProject;
    QString exitWord[32];//保留字表
    QString operatorOrDelimiter[36];//界符运算表
    QString IDentifierTbl[1000];//标识符表
    int syn;
    QString token;
};

#endif // MAINWINDOW_H
