//
// Created by SD43247 on 03/07/2024.
//

#include "AppController.h++"

AppController::AppController(QObject *parent)
        :
        QObject(parent) {

}

/**
 * @class AppController
 * @brief This method initializes application and connects signals and slots
 **/

void AppController::initialize() {

    main_window_ = std::make_unique<MainWindow>();
    main_window_->show();

    app_settings_ = std::make_unique<AppSettings>(main_window_->getAppSettingsWidget());
    job_creation_ = std::make_unique<JobCreation>(main_window_->getJobCreationWidget(),
                                                  app_settings_->getLocalConfigLocation());


    connectSignalsAndSlots();
    app_settings_->connectSignalsAndSlots();
    job_creation_->connectSignalsAndSlots();

    try {
        copyRemoteConfigFolderToLocal(app_settings_->getRemoteConfigLocation(),
                                      app_settings_->getLocalConfigLocation());
    } catch (const PathNotFoundException &e) {
        main_window_->getTabWidget()->setCurrentIndex(1);
        QMessageBox::warning(main_window_.get(), "Config folder error", e.what());
        return;
    } catch (const FileCopyException &e) {
        qCritical() << e.what();
        return;
    }

    job_creation_->initialize();
    job_creation_->loadUserInput();
}


void AppController::connectSignalsAndSlots() {
    //connect main window tab change signal to job creation widget save user input slot
    QObject::connect(main_window_->getTabWidget(), &QTabWidget::currentChanged,
                     [this](int index) { currentTabChanged(index); });
}


void AppController::copyRemoteConfigFolderToLocal(const QString &remoteConfigFolder, const QString &localConfigFolder) {
    qDebug() << "Copying remote config folder to local config folder";
    if (!QDir(remoteConfigFolder).exists() || !QDir(localConfigFolder).exists()) {
        throw PathNotFoundException("One of the config folders does not exist");
    }

    if (remoteConfigFolder.isEmpty() || localConfigFolder.isEmpty()) {
        throw PathNotFoundException("One of the config folders is empty.");
    }

    //copy all files from remote config folder to local config folder if they don't already exist, otherwise overwrite only if the remote file is newer
    //copy recursively if the file is a folder
    QDirIterator it(remoteConfigFolder, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QString relativePath = it.filePath().mid(remoteConfigFolder.length());
        QString localPath = localConfigFolder + relativePath;
        if (QFileInfo(it.filePath()).isDir()) {
            if (!QDir().mkpath(localPath)) {
                throw PathNotFoundException(QString("Failed to create local folder '%1'").arg(localPath));
            }
        } else {
            if (QFileInfo::exists(localPath)) {
                if (QFileInfo(it.filePath()).lastModified() > QFileInfo(localPath).lastModified()) {
                    if (!QFile::remove(localPath)) {
                        throw FileCopyException(QString("Failed to remove local file '%1'").arg(localPath));
                    }
                } else {
                    continue;
                }
            }
            if (!QFile::copy(it.filePath(), localPath)) {
                throw FileCopyException(
                        QString("Failed to copy remote file '%1' to local file '%2'").arg(it.filePath(), localPath));
            }
        }
    }


}

void AppController::currentTabChanged(int index) {
    if (index == 0) {
        job_creation_->initialize();
        job_creation_->loadUserInput();
    }

}


