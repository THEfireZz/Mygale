//
// Created by SD43247 on 03/07/2024.
//

#include <utility>

#include "JobCreation.h++"

JobCreation::JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath) : job_creation_widget_(
        job_creation_widget), config_file_path_(std::move(configFilePath)) {
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

    QStringList formats = getFormatsFromConfigFile(config_file_path_,
                                                   job_creation_widget_->getJobTypeComboBox()->currentText());
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

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
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

/**
 * @class JobCreation
 * @brief This method reads the xml file and returns a list of formats for a given job type
 *
 * @param configFilePath the path to xml file containing the formats
 * @param jobType the current job type
 *
 * @return a list of formats
 **/

QStringList JobCreation::getFormatsFromConfigFile(const QString &configFilePath, const QString &jobType) {
    QStringList formats;
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Could not open file : " + configFilePath.toStdString());
    }

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
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

/**
 * @class JobCreation
 * @brief This method creates a job object with the parameters from the job creation widget
 *
 * @return a job object
 **/
Job JobCreation::createJob() {
    QString job_name = getJobName();
    QString job_type = getJobType();
    QString scene_path = getScenePath();
    QString output_path = getOutputPath();
    QString format = getFormat();
    QString first_image = getFirstImage();
    QString last_image = getLastImage();
    QString first_index = getFirstIndex();
    QString last_index = getLastIndex();
    QString camera_name = getCameraName();
    QString min_cpu = getMinCpu();
    QString max_cpu = getMaxCpu();
    QString min_memory = getMinMemory();

    job_ = Job::Builder()
            .setJobName(job_name)
            .setJobType(job_type)
            .setJobParameters(QHash<QString, QString>{
                    {"scene_path", scene_path},
                    {"output_path", output_path},
                    {"format", format},
                    {"first_image", first_image},
                    {"last_image", last_image},
                    {"first_index", first_index},
                    {"last_index", last_index},
                    {"camera_name", camera_name},
                    {"min_cpu", min_cpu},
                    {"max_cpu", max_cpu},
                    {"min_memory", min_memory}
            })
            .build();

    return job_;
}

/**
 * @class JobCreation
 * @brief This method returns the job type from the job creation widget
 *
 * @return the job type
 **/

QString JobCreation::getJobType() const {
    return job_creation_widget_->getJobTypeComboBox()->currentText();
}

/**
 * @class JobCreation
 * @brief This method returns the job name from the job creation widget
 *
 * @return the job name
 **/

QString JobCreation::getJobName() const {
    return job_creation_widget_->getJobNameLineEdit()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the scene path from the job creation widget
 *
 * @return the scene path
 **/

QString JobCreation::getScenePath() const {
    return job_creation_widget_->getSceneLineEdit()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the output path from the job creation widget
 *
 * @return the output path
 **/

QString JobCreation::getOutputPath() const {
    return job_creation_widget_->getOutputLineEdit()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the format from the job creation widget depending on the job type
 *
 * @return the format
 **/

QString JobCreation::getFormat() const {
    if (job_creation_widget_->getImagesFormatCheckBox()->isChecked()) {
        QString selection = job_creation_widget_->getImagesFormatComboBox()->currentText();
        QString result;
        if (job_creation_widget_->getJobTypeComboBox()->currentText() != "Blender") {
            result = getRawFormat(selection);
        } else {
            result = getFormatName(selection);
        }
        return result;
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the raw format from the current format selected
 *
 * @param selection the current format selected
 *
 * @return the raw format
 **/

QString JobCreation::getRawFormat(const QString &selection) {
    static QRegularExpression regex(R"(\[\*\.(\w+)\])");
    if (auto match = regex.match(selection); match.hasMatch()) {
        return match.captured(1);
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the format name from the current format selected
 *
 * @param selection the current format selected
 *
 * @return the format name
 **/
QString JobCreation::getFormatName(const QString &selection) {
    static QRegularExpression regex(R"(\b(.*?)\s*\[\*\.)");
    if (auto match = regex.match(selection); match.hasMatch()) {
        return match.captured(1);
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the first image from the job creation widget
 **/

QString JobCreation::getFirstImage() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    }
    return job_creation_widget_->getFirstImageSpinBox()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the last image from the job creation widget
 **/
QString JobCreation::getLastImage() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    }
    return job_creation_widget_->getLastImageSpinBox()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the first index (calculated from the first image) from the job creation widget
 **/
QString JobCreation::getFirstIndex() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    } else if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        return "1";
    }
    return job_creation_widget_->getFirstImageSpinBox()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the last index (calculated from the last image and the number of batch images) from the job creation widget
 **/
QString JobCreation::getLastIndex() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    } else if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        if (job_creation_widget_->getBatchCalculationSpinBox()->value() == 0) {
            throw std::invalid_argument("Batch calculation value cannot be 0");
        }
        int last_index = job_creation_widget_->getLastImageSpinBox()->value() -
                         job_creation_widget_->getFirstImageSpinBox()->value() /
                         job_creation_widget_->getBatchCalculationSpinBox()->value();
        return QString::number(last_index);
    }
    return job_creation_widget_->getLastImageSpinBox()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the camera name from the job creation widget
 **/
QString JobCreation::getCameraName() const {
    if (job_creation_widget_->getCameraGroupBox()->isChecked()){
        return job_creation_widget_->getCameraLineEdit()->text();
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the minimum cpu from the job creation widget
 **/

QString JobCreation::getMinCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()){
        return job_creation_widget_->getMinCpuSpinBox()->text();
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the maximum cpu from the job creation widget if the cpu checkbox is not checked, it returns the default value (32)
 **/
QString JobCreation::getMaxCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()){
        return job_creation_widget_->getMaxCpuSpinBox()->text();
    }
    return {"32"};
}

/**
 * @class JobCreation
 * @brief This method returns the minimum memory from the job creation widget
 **/
QString JobCreation::getMinMemory() const {
    if (job_creation_widget_->getMemoryCheckBox()->isChecked()){
        return job_creation_widget_->getMemorySpinBox()->text();
    }
    return {};
}
