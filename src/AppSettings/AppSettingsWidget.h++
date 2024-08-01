//
// Created by SD43247 on 01/08/2024.
//

#ifndef MYGALE_APPSETTINGSWIDGET_H
#define MYGALE_APPSETTINGSWIDGET_H

#include <QWidget>
#include <QLineEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class AppSettingsWidget; }
QT_END_NAMESPACE

class AppSettingsWidget : public QWidget {
Q_OBJECT

public:
    explicit AppSettingsWidget(QWidget *parent = nullptr);

    ~AppSettingsWidget() override;

    //Ui getters

    [[nodiscard]] QLineEdit *getRemoteLocationLineEdit();

    [[nodiscard]] QToolButton *getRemoteLocationToolButton();

    [[nodiscard]] QLineEdit *getLocalLocationLineEdit();

    [[nodiscard]] QToolButton *getLocalLocationToolButton();

private:
    Ui::AppSettingsWidget *ui;

};


#endif //MYGALE_APPSETTINGSWIDGET_H
