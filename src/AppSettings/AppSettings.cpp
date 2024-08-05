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
