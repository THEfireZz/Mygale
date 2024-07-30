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

    QAbstractButton::connect(job_creation_widget_->getExecutionPushButton(), &QAbstractButton::clicked, [this] {
        createAndExecuteJob("50");
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
    //Blender or Maya_2023/Vray or vred
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
        filter = "Scene Files (*.blend)";
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
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
Job JobCreation::createJob(QString priority) {
    QString job_name = getJobName();
    QString path = convertToUncPath("I:/");
    QString job_type = getJobType();
    QString scene_path = getScenePath();
    QString output_path = getOutputPath();
    QString name = getName();
    QString format = getFormat();
    QString raw_format = getRawFormat(job_creation_widget_->getImagesFormatComboBox()->currentText());
    QString first_image = getFirstImage();
    QString last_image = getLastImage();
    QString first_index = getFirstIndex();
    QString last_index = getLastIndex();
    QString camera_name = getCameraName();
    QString min_cpu = getMinCpu();
    QString max_cpu = getMaxCpu();
    QString submission_option = getSubmissionOption();
    QString steps = getSteps();

    job_ = Job::Builder()
            .setJobName(job_name)
            .setJobType(job_type)
            .setJobParameters(QHash<QString, QString>{
                    {"<jobName>",          job_name},
                    {"<path>",             path},
                    {"<scene>",            scene_path},
                    {"<output>",           output_path},
                    {"<name>",             name},
                    {"<format>",           format},
                    {"<rawFormat>",        raw_format},
                    {"<firstImg>",         first_image},
                    {"<lastImg>",          last_image},
                    {"<firstIndex>",       first_index},
                    {"<lastIndex>",        last_index},
                    {"<camera>",           camera_name},
                    {"<minCPU>",           min_cpu},
                    {"<maxCPU>",           max_cpu},
                    {"<submissionOption>", submission_option},
                    {"<steps>",            steps},
                    {"<priority>",         priority}
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
    return convertToUncPath(job_creation_widget_->getSceneLineEdit()->text());
}

/**
 * @class JobCreation
 * @brief This method returns the output path from the job creation widget
 *
 * @return the output path
 **/
QString JobCreation::getOutputPath() const {
    return convertToUncPath(job_creation_widget_->getOutputLineEdit()->text());
}

/**
 * @class JobCreation
 * @brief This method returns the name from the job creation widget and adds the prefix if the job type is Maya_2023/Vray and adds the prefix if needed
 *
 * @return the name
 **/
QString JobCreation::getName() {
    QString prefix = "";
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        prefix = "-im ";
    }
    if (job_creation_widget_->getImagesNameCheckBox()->isChecked()) {
        return prefix + job_creation_widget_->getJobNameLineEdit()->text();
    }
    return {};

}

/**
 * @class JobCreation
 * @brief This method returns the format from the job creation widget depending on the job type and adds the prefix if the job type is Blender or Maya_2023/Vray and adds the prefix if needed
 *
 * @return the format
 **/
QString JobCreation::getFormat() const {
    QString prefix = "";
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
        prefix = "-F ";
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        prefix = "-of ";
    }
    if (job_creation_widget_->getImagesFormatCheckBox()->isChecked()) {
        QString selection = job_creation_widget_->getImagesFormatComboBox()->currentText();
        QString result;
        if (job_creation_widget_->getJobTypeComboBox()->currentText() != "Blender") {
            result = getRawFormat(selection);
        } else {
            result = getFormatName(selection);
        }
        return prefix + result;
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
 *
 * @return the first image
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
 *
 * @return the last image
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
 *
 * @return the first index
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
 *
 * @return the last index
 **/
QString JobCreation::getLastIndex() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    } else if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        if (job_creation_widget_->getBatchCalculationSpinBox()->value() == 0) {
            throw std::invalid_argument("Batch calculation value cannot be 0");
        }
        int last_index = (job_creation_widget_->getLastImageSpinBox()->value() -
                          job_creation_widget_->getFirstImageSpinBox()->value()) /
                         job_creation_widget_->getBatchCalculationSpinBox()->value();
        return QString::number(last_index);
    }
    return job_creation_widget_->getLastImageSpinBox()->text();
}

/**
 * @class JobCreation
 * @brief This method returns the camera name from the job creation widget and adds the prefix if the job type is Maya_2023/Vray and adds the prefix if needed
 *
 * @return the camera name
 **/
QString JobCreation::getCameraName() const {
    QString prefix = "";
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        prefix = "-cam ";
    }
    if (job_creation_widget_->getCameraGroupBox()->isChecked()) {
        return job_creation_widget_->getCameraLineEdit()->text();
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the minimum cpu from the job creation widget
 *
 * @return the minimum cpu
 **/
QString JobCreation::getMinCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()) {
        return job_creation_widget_->getMinCpuSpinBox()->text();
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the maximum cpu from the job creation widget if the cpu checkbox is not checked, it returns the default value (32)
 *
 * @return the maximum cpu
 **/
QString JobCreation::getMaxCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()) {
        return job_creation_widget_->getMaxCpuSpinBox()->text();
    }
    return {"32"};
}

/**
 * @class JobCreation
 * @brief This method returns the submission option from the job creation widget
 *
 * @return the submission option
 **/
QString JobCreation::getSubmissionOption() const {
    QString cpuInterval = getCpuInterval();
    QString memoryInterval = getMemoryInterval();
    QString parcStyleList = getParcStyleList();

    QStringList submissionOptions;

    if (!cpuInterval.isEmpty()) {
        submissionOptions.append(cpuInterval);
    }
    if (!memoryInterval.isEmpty()) {
        submissionOptions.append(memoryInterval);
    }
    if (!parcStyleList.isEmpty()) {
        submissionOptions.append(parcStyleList);
    } else {
        throw std::invalid_argument("No parc style selected");
    }

    return submissionOptions.join(" && ");
}

/**
 * @class JobCreation
 * @brief This method returns the cpu interval from the job creation widget
 *
 * @return the cpu interval
 **/
QString JobCreation::getCpuInterval() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()) {
        QString minCpu = getMinCpu();
        QString maxCpu = getMaxCpu();
        return QString("ncpus >= %1 && ncpus <= %2").arg(minCpu, maxCpu);
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the memory interval from the job creation widget
 *
 * @return the memory interval
 **/
QString JobCreation::getMemoryInterval() const {
    if (job_creation_widget_->getMemoryCheckBox()->isChecked()) {
        double memory = job_creation_widget_->getMemorySpinBox()->value() * 1024;
        return QString("mem > %1").arg(memory);
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method returns the parc style list from the job creation widget
 *
 * @return the parc style list
 **/
QString JobCreation::getParcStyleList() const {
    QStringList parcStyleList;

    //for each checked parc style in the pcPoolManagementGridLayout add "parc_style == <parcStyle>"
    for (int i = 0; i < job_creation_widget_->getPcPoolManagementGridLayout()->count(); ++i) {
        QCheckBox const *checkBox;
        checkBox = qobject_cast<QCheckBox *>(
                job_creation_widget_->getPcPoolManagementGridLayout()->itemAt(i)->widget());
        if (checkBox->isChecked()) {
            parcStyleList.append(QString("parc_style == %1").arg(checkBox->property("LSF").toString()));
        }
    }

    if (parcStyleList.size() == 1) {
        return parcStyleList.isEmpty() ? QString() : parcStyleList.first();
    } else {
        return parcStyleList.isEmpty() ? QString() : "(" + parcStyleList.join(" || ") + ")";
    }
}

/**
 * @class JobCreation
 * @brief This method returns the steps from the job creation widget
 *
 * @return the steps
 **/
QString JobCreation::getSteps() {
    if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        return QString::number(job_creation_widget_->getBatchCalculationSpinBox()->value());
    }
    return {};
}

/**
 * @class JobCreation
 * @brief This method creates and executes a job
 *
 * @param priority the priority of the job
 **/
void JobCreation::createAndExecuteJob(QString priority) {
    createJob(std::move(priority));
    qDebug() << "Job created : " << job_.getJobName();

    QString remote_script_path;
    if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
        if (!job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Blender\Skeleton_Blender.txt)";
        } else {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Blender\Skeleton_Blender_MultipleImage.txt)";
        }
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
        if (!job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Maya\Skeleton_Maya_2023Vray.txt)";
        } else {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Maya\Skeleton_Maya_2023Vray_MultipleImage.txt)";
        }
    } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Vred") {
        if (!job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Vred\Skeleton_Vred.txt)";
        } else {
            remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Vred\Skeleton_Vred_Resubmission.txt)";
        }
    }

    QString remote_launchers_path = R"(I:\Mygale\Config_Blender_4_V2\lance.txt)";

    QString local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";

    qDebug() << "Remote script path : " << remote_script_path << " Remote launchers path : " << remote_launchers_path
             << " Local job location : " << local_job_location;
    BaseScript script(job_, remote_script_path, remote_launchers_path, local_job_location);
    script.copyRemoteScript(job_.getJobName() + ".bat", "lance.bat");
    script.replaceScriptParameters(job_.getJobName() + ".bat", "lance.bat");
    QString output = script.executeScript("lance.bat");

    qDebug() << "Output : " << output;

    if (job_creation_widget_->getAnalysisCheckBox()->isChecked()) {
        static QRegularExpression regex(R"(\d+)");
        job_.addJobParameter("<previousJobID>", regex.match(output).captured(0));

        remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Skeleton_Analysis.txt)";
        remote_launchers_path = R"(I:\Mygale\Config_Blender_4_V2\lanceAnalysis.txt)";
        local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";

        job_.addJobParameter("<jobName>", job_.getJobName());

        qDebug() << "Remote script path : " << remote_script_path
                 << " Remote launchers path : " << remote_launchers_path
                 << " Local job location : " << local_job_location;

        BaseScript analysis_script(job_, remote_script_path, remote_launchers_path, local_job_location);
        analysis_script.copyRemoteScript(job_.getJobName() + "_Analysis.bat", "lanceAnalysis.bat");

        if (job_creation_widget_->getResubmissionCheckBox()->isChecked()) {
            if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Blender") {
                remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Blender\Skeleton_Blender_Resubmission.txt)";
            } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Maya_2023/Vray") {
                remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Maya\Skeleton_Maya_2023Vray_Resubmission.txt)";
            } else if (job_creation_widget_->getJobTypeComboBox()->currentText() == "Vred") {
                remote_script_path = R"(I:\Mygale\Config_Blender_4_V2\skeleton\Vred\Skeleton_Vred_Resubmission.txt)";
            }

            remote_launchers_path = R"(I:\Mygale\Config_Blender_4_V2\lanceResubmission.txt)";
            local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";

            job_.addJobParameter("<jobName>", job_.getJobName());

            qDebug() << "Remote script path : " << remote_script_path << " Remote launchers path : "
                     << remote_launchers_path
                     << " Local job location : " << local_job_location;

            BaseScript resubmission_script(job_, remote_script_path, remote_launchers_path, local_job_location);
            resubmission_script.copyRemoteScript(job_.getJobName() + "_Resubmission.bat", "lanceResubmission.bat");
            resubmission_script.appendResubmissionJobExecutionLine(
                    R"(I:\Mygale\Config_Blender_4_V2\resubmissionExecutionLine.txt)",
                    job_.getJobName() + "_Analysis.bat");

            resubmission_script.replaceScriptParameters(job_.getJobName() + "_Resubmission.bat", "lanceResubmission.bat");
        }

        analysis_script.replaceScriptParameters(job_.getJobName() + "_Analysis.bat", "lanceAnalysis.bat");
        analysis_script.executeScript("lanceAnalysis.bat");
    }
}

QString JobCreation::convertToUncPath(const QString &path) {
    // Path = I:/Mygale/TEMP/JobName/
    QString uncPath = path;
    if (!QFileInfo::exists(path)) {
        throw std::invalid_argument("The path does not exist");
    }
    QStorageInfo storageInfo(path);
    DWORD driveType = GetDriveType((LPCWSTR) storageInfo.rootPath().utf16());
    if (driveType == DRIVE_REMOTE) {
        return uncPath.replace("I:/", storageInfo.device()).replace("/", "\\");
    } else {
        throw std::invalid_argument("The path is not a network path");
    }
}


