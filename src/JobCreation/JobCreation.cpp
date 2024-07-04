//
// Created by SD43247 on 03/07/2024.
//

#include <utility>

#include "JobCreation.h++"

JobCreation::JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath) : job_creation_widget_(job_creation_widget), config_file_path_(std::move(configFilePath)){
}

/**
 * @class JobCreation
 * @brief This method initializes the JobCreation class and connects the signals
 **/
void JobCreation::initialize() {
    job_creation_widget_->initialize();
    connectSignalsAndSlots();

    QStringList job_types = getJobTypesFromConfigFile(config_file_path_);
    job_creation_widget_->getJobTypeComboBox()->addItems(job_types);

    QStringList formats = getFormatsFromConfigFile(config_file_path_, job_creation_widget_->getJobTypeComboBox()->currentText());
    job_creation_widget_->getImagesFormatComboBox()->addItems(formats);
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

/**
 * @class JobCreation
 * @brief This method increments the job number in the job name line edit
 **/
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

/**
 * @class JobCreation
 * @brief This method opens a file dialog to select a scene file and sets the path in the scene line edit
 **/
void JobCreation::openSceneFileDialog() {
    QString filter;
    //Blender or Maya_2020/Vray // Maya_2023/Vray or vred
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
        filter = "Scene Files (*.blend)";
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2020/Vray" ||
               job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        filter = "Scene Files (*.ma *.mb)";
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Vred") {
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

/**
 * @class JobCreation
 * @brief This method opens a file dialog to select an output folder and sets the path in the output line edit
 **/
void JobCreation::openOutputFolderDialog() {
    QString output_folder = QFileDialog::getExistingDirectory(job_creation_widget_, "Select Output Folder", "I://");
    if (output_folder.isEmpty()) {
        return;
    }
    job_creation_widget_->getOutputLineEdit()->setText(output_folder);
}

/**
 * @class JobCreation
 * @brief This method reads the xml file and returns a list of job types
 *
 * @param configFilePath the path to xml file containing the job types
 *
 * @return a list of job types
 *
 * @throw runtime_error if the file could not be opened
 **/

QStringList JobCreation::getJobTypesFromConfigFile(const QString &configFilePath) {
    QStringList job_types;
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Could not open file : " + configFilePath.toStdString());
    }

    QXmlStreamReader xmlReader(&file);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (!xmlReader.isStartElement() || xmlReader.name().toString() != "Option") {
            continue;
        }

        while (xmlReader.readNextStartElement()) {
            if (xmlReader.name().toString() == "name") {
                job_types.append(xmlReader.readElementText());
            } else {
                xmlReader.skipCurrentElement();
            }
        }
    }

    file.close();
    return job_types;

}

QStringList JobCreation::getFormatsFromConfigFile(const QString &configFilePath, const QString &jobType) {
    /*		<Option>
			<name>Maya_2020/Vray</name>
			<skeleton>squeletteMaya_2020Vray.txt</skeleton>
			<Format>Microsoft Windows Bitmap [*.bmp]</Format>
			<Format>JPEG [*.jpg]</Format>
			<Format>PNG [*.png]</Format>
			<Format>Silicon Graphics [*.sgi]</Format>
			<Format>Softimage [*.pic]</Format>
			<Format>Targa [*.tga]</Format>
			<Format>TIFF [*.tif]</Format>
			<NameOption>-im </NameOption>
			<FormatOption>-of </FormatOption>
			<CameraOption>-cam </CameraOption>
		</Option>
     */
    QStringList formats;
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Could not open file : " + configFilePath.toStdString());
    }

    QXmlStreamReader xmlReader(&file);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (!xmlReader.isStartElement() || xmlReader.name().toString() != "Option") {
            continue;
        }

        while (xmlReader.readNextStartElement()) {
            if (xmlReader.name().toString() == "name" && xmlReader.readElementText() == jobType) {
                while (xmlReader.readNextStartElement()) {
                    if (xmlReader.name().toString() == "Format") {
                        formats.append(xmlReader.readElementText());
                    } else {
                        xmlReader.skipCurrentElement();
                    }
                }
            } else {
                xmlReader.skipCurrentElement();
            }
        }
    }

    file.close();
    return formats;
}
