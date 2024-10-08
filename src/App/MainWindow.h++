//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_MAINWINDOW_H
#define MYGALE_MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "../JobCreation/JobCreationWidget.h++"
#include "../AppSettings/AppSettingsWidget.h++"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    //Ui getter

    [[nodiscard]] JobCreationWidget *getJobCreationWidget() const;

    [[nodiscard]] AppSettingsWidget *getAppSettingsWidget() const;

    [[nodiscard]] QTabWidget *getTabWidget() const;

private:
    Ui::MainWindow *ui;
protected:
    void closeEvent(QCloseEvent *event) override;
};


#endif //MYGALE_MAINWINDOW_H
