//
// Created by SD43247 on 01/08/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AppSettingsWidget.h" resolved

#include "AppSettingsWidget.h++"
#include "../../resources/ui/ui_AppSettingsWidget.h"


AppSettingsWidget::AppSettingsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::AppSettingsWidget) {
    ui->setupUi(this);
}

AppSettingsWidget::~AppSettingsWidget() {
    delete ui;
}

QLineEdit *AppSettingsWidget::getRemoteLocationLineEdit() {
    return ui->remoteLocationLineEdit;
}

QToolButton *AppSettingsWidget::getRemoteLocationToolButton() {
    return ui->remoteLocationToolButton;
}

QLineEdit *AppSettingsWidget::getLocalLocationLineEdit() {
    return ui->localLocationLineEdit;
}

QToolButton *AppSettingsWidget::getLocalLocationToolButton() {
    return ui->localLocationToolButton;
}

void AppSettingsWidget::saveUserInput() const {
    qDebug() << "Saving user input";
    QSettings settings("Stellantis", "Mygale");
    settings.beginGroup("MainWindow");
    settings.beginGroup("AppSettingsWidget");

    for (QWidget *widget: this->findChildren<QWidget *>()) {
        const QString objectName = widget->objectName();
        if (!objectName.isEmpty()) {

            if (auto const *lineEdit = qobject_cast<QLineEdit *>(widget)) {
                settings.setValue(objectName, lineEdit->text());
            }
            if (auto const *comboBox = qobject_cast<QComboBox *>(widget)) {
                settings.setValue(objectName, comboBox->currentIndex());
            }
            if (auto const *checkBox = qobject_cast<QCheckBox *>(widget)) {
                settings.setValue(objectName, checkBox->isChecked());
            }
            if (auto const *spinBox = qobject_cast<QSpinBox *>(widget)) {
                settings.setValue(objectName, spinBox->value());
            }
            if (auto const *radioButton = qobject_cast<QRadioButton *>(widget)) {
                settings.setValue(objectName, radioButton->isChecked());
            }
        }
        if (settings.contains("qt_spinbox_lineedit"))
            settings.remove("qt_spinbox_lineedit");

    }
}
