//
// Created by SD43247 on 03/07/2024.
//

#include "JobCreation.h++"


/**
 * @class JobCreation
 * @brief This method initializes the JobCreation class and connects the signals
 **/
void JobCreation::initialize() {
    job_creation_widget_->initialize();
    connectSignalsAndSlots();
}

void JobCreation::connectSignalsAndSlots() {
    QAbstractButton::connect(job_creation_widget_->getJobNameToolButton(), &QAbstractButton::clicked, [this] {
        incrementJobNumber();
    });

    QAbstractButton::connect(job_creation_widget_->getSceneToolButton(), &QAbstractButton::clicked, [this] {
        openSceneFileDialog();
    });

    QAbstractButton::connect(job_creation_widget_->getOutputToolButton(), &QAbstractButton::clicked, [this] {
        openOutputFolderDialog();
    });
}


JobCreation::JobCreation(JobCreationWidget *job_creation_widget) : job_creation_widget_(job_creation_widget) {
}

void JobCreation::incrementJobNumber() {
    QString job_name = job_creation_widget_->getJobNameLineEdit()->text();
    if (job_name.isEmpty()) {
        return;
    }

    int suffix = 1;
    static QRegularExpression regex("_([0-9]+)$");
    if (QRegularExpressionMatch match = regex.match(job_name); match.hasMatch()) {
        suffix = match.captured(1).toInt() + 1;
        job_name.chop(match.captured(1).size() + 1);
    }

    QString new_job_name = job_name + "_" + QString::number(suffix);
    job_creation_widget_->getJobNameLineEdit()->setText(new_job_name);
}

void JobCreation::openSceneFileDialog() {
    QString filter;
    //Blender or Maya_2020/Vray // Maya_2023/Vray or vred
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
        filter = "Scene Files (*.blend)";
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2020/Vray" ||
               job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        filter = "Scene Files (*.ma *.mb)";
    } else if(job_creation_widget_->getJobTypeComboBox()->currentText() == "Vred") {
        filter = "Scene Files (*.vpb)";
    } else {
        filter = "Scene Files (*.blend *.ma *.mb *.vpb)";
    }

    QString file_name = QFileDialog::getOpenFileName(job_creation_widget_, "Open Scene File", "I://", filter);
    if (file_name.isEmpty()) {
        return;
    }
    job_creation_widget_->getSceneLineEdit()->setText(file_name);
}

void JobCreation::openOutputFolderDialog() {
    QString output_folder = QFileDialog::getExistingDirectory(job_creation_widget_, "Select Output Folder", "I://");
    if (output_folder.isEmpty()) {
        return;
    }
    job_creation_widget_->getOutputLineEdit()->setText(output_folder);
}


