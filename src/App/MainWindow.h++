//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_MAINWINDOW_H
#define MYGALE_MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};


#endif //MYGALE_MAINWINDOW_H
