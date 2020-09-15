#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "ui_mainwindow.h"
#include <vector>
#include <string>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showMsg(std::vector<QString> &);
    void gameboardD(int*,int,int);
    void gameboardD2(int*,int,int);
    void gameboardL(int*,int);
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
