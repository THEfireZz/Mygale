//
// Created by SD43247 on 01/08/2024.
//


#include "AppSettings.h++"

AppSettings::AppSettings(AppSettingsWidget *parent) : app_settings_widget_(parent) {

}

void AppSettings::connectSignalsAndSlots() {
    QAbstractButton::connect(app_settings_widget_->getRemoteLocationToolButton(), &QAbstractButton::clicked, [this] {
        openRemoteConfigFileDialog();
    });

    QAbstractButton::connect(app_settings_widget_->getLocalLocationToolButton(), &QAbstractButton::clicked, [this] {
        openLocalConfigFileDialog();
    });
}

void AppSettings::loadUserInput() const {
    QSettings settings("Stellantis", "Mygale");
    settings.beginGroup("MainWindow");
    settings.beginGroup("AppSettingsWidget");

    QStringList keys = settings.childKeys();
    for (QWidget *widget: app_settings_widget_->findChildren<QWidget *>()) {
        const QString objectName = widget->objectName();
        if (keys.contains(objectName)) {
            if (auto *lineEdit = qobject_cast<QLineEdit *>(widget)) {
                lineEdit->setText(settings.value(objectName).toString());
            } else if (auto *comboBox = qobject_cast<QComboBox *>(widget)) {
                comboBox->setCurrentIndex(settings.value(objectName).toInt());
            } else if (auto *checkBox = qobject_cast<QCheckBox *>(widget)) {
                checkBox->setChecked(settings.value(objectName).toBool());
            } else if (auto *spinBox = qobject_cast<QSpinBox *>(widget)) {
                spinBox->setValue(settings.value(objectName).toInt());
            }
        }
    }

}

QString AppSettings::getRemoteConfigLocation() const {
    return app_settings_widget_->getRemoteLocationLineEdit()->text();
}

QString AppSettings::getLocalConfigLocation() const {
    return app_settings_widget_->getLocalLocationLineEdit()->text();
}

void AppSettings::openRemoteConfigFileDialog() {
    QString remote_config_location = QFileDialog::getExistingDirectory(app_settings_widget_,
                                                                       "Select Remote Config Location",
                                                                       QDir::homePath());
    app_settings_widget_->getRemoteLocationLineEdit()->setText(remote_config_location);
}

void AppSettings::openLocalConfigFileDialog() {
    QString local_config_location = QFileDialog::getExistingDirectory(app_settings_widget_,
                                                                      "Select Local Config Location",
                                                                      QDir::homePath());
    app_settings_widget_->getLocalLocationLineEdit()->setText(local_config_location);

}
