//
// Created by SD43247 on 03/07/2024.
//

#include <utility>
#include <QSettings>

#include "JobCreation.h++"
#include "../exception/CustomErrors.h++"

/**
 * @class JobCreation
 * @brief The JobCreation class is responsible for creating and executing jobs
 *
 * @param job_creation_widget The job creation widget
 * @param configFilePath The path to the config file
 **/
JobCreation::JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath) : job_creation_widget_(
        job_creation_widget), config_file_path_(std::move(configFilePath)) {
}

/**
 * @brief This method initializes the job creation widget and the comboboxes
 **/
void JobCreation::initialize() {
    job_creation_widget_->initialize();

    QStringList job_types = getJobTypesFromConfigFile(config_file_path_);
    job_creation_widget_->getJobTypeComboBox()->clear();
    job_creation_widget_->getJobTypeComboBox()->addItems(job_types);

    QStringList formats = getFormatsFromConfigFile(config_file_path_,
                                                   job_creation_widget_->getJobTypeComboBox()->currentText());
    job_creation_widget_->getImagesFormatComboBox()->clear();
    job_creation_widget_->getImagesFormatComboBox()->addItems(formats);
}

/**
 * @brief This method updates the format combobox depending on the job type
 **/
void JobCreation::loadUserInput() const {
    QSettings settings("Stellantis", "Mygale");
    settings.beginGroup("MainWindow");
    settings.beginGroup("JobCreationWidget");

    QStringList keys = settings.childKeys();
    for (QWidget *widget: job_creation_widget_->findChildren<QWidget *>()) {
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

/**
 * @brief This method connects the signals and slots for the job creation widget
 **/
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

    QComboBox::connect(job_creation_widget_->getJobTypeComboBox(), &QComboBox::currentTextChanged,
                       [this](const QString &jobType) {
                           updateFormatComboBox(jobType);
                       });

    QAbstractButton::connect(job_creation_widget_->getExecutionPushButton(), &QAbstractButton::clicked, [this] {
        try {
            createAndExecuteJob("50");
        } catch (const JobAlreadyExistsException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
            incrementJobNumber();
        } catch (const CustomErrors &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        }
    });

    QAbstractButton::connect(job_creation_widget_->getPriorityExecutionPushButton(), &QAbstractButton::clicked, [this] {
        try {
            createAndExecuteJob("75");
        } catch (const JobAlreadyExistsException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
            incrementJobNumber();
        } catch (const CustomErrors &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        }
    });


    //Handle elements enable/disable
    QAbstractButton::connect(job_creation_widget_->getSingleImageRadioButton(), &QRadioButton::toggled,
                             [this](bool checked) { job_creation_widget_->singleImageRadioButtonToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getMultipleImagesRadioButton(), &QRadioButton::toggled,
                             [this](bool checked) { job_creation_widget_->multipleImagesRadioButtonToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getBatchCalculationCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->batchCalculationCheckBoxToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getImagesNameCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->imagesNameCheckBoxToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getImagesFormatCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->imagesFormatCheckBoxToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getCpuCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->cpuCheckBoxToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getMemoryCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->memoryCheckBoxToggled(checked); });

    QAbstractButton::connect(job_creation_widget_->getAnalysisCheckBox(), &QCheckBox::toggled,
                             [this](bool checked) { job_creation_widget_->analysisCheckBoxToggled(checked); });
}

/**
 * @brief This method adds the job number suffix to the job name if there is none or increments the suffix if there is one
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
 * @brief This method opens a file dialog to select the scene file
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
 * @brief This method opens a file dialog to select the output folder
 **/
void JobCreation::openOutputFolderDialog() {
    QString output_folder = QFileDialog::getExistingDirectory(job_creation_widget_, "Select Output Folder", "I://");
    if (output_folder.isEmpty()) {
        return;
    }
    job_creation_widget_->getOutputLineEdit()->setText(output_folder);
}

/**
 * @brief This method returns the job types from the config file
 *
 * @param configFilePath The path to the config file
 *
 * @return The job types
 *
 * @throw FileOpenException
 * @throw XmlParseException
 **/
QStringList JobCreation::getJobTypesFromConfigFile(const QString &configFilePath) {
    QStringList job_types;
    QFile file(configFilePath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "name") {
            QString job_type = xmlReader.readElementText();
            job_types.append(job_type);
        }
    }

    if (xmlReader.hasError()) {
        throw XmlParseException("Error parsing XML: " + xmlReader.errorString());
    }

    file.close();
    return job_types;
}

/**
 * @brief This method returns the formats from the config file
 *
 * @param configFilePath The path to the config file
 * @param jobType The job type
 *
 * @return The formats
 *
 * @throw FileOpenException
 * @throw XmlParseException
 **/
QStringList JobCreation::getFormatsFromConfigFile(const QString &configFilePath, const QString &jobType) {
    QStringList formats;
    QFile file(configFilePath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);
    QString currentJobType;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "Option") {
            currentJobType.clear();  // Reset the current job type for each Option block
        }

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "name") {
            currentJobType = xmlReader.readElementText();
        }

        if (!currentJobType.isEmpty() && currentJobType == jobType) {
            if (xmlReader.isStartElement() && xmlReader.name().toString() == "Format") {
                QString format = xmlReader.readElementText();
                formats.append(format);
            }
        }

        if (xmlReader.isEndElement() && xmlReader.name().toString() == "Option") {
            currentJobType.clear();  // Reset the current job type after exiting an Option block
        }
    }

    if (xmlReader.hasError()) {
        throw XmlParseException("Error parsing XML: " + xmlReader.errorString());
    }

    file.close();
    return formats;
}

/**
 * @brief This method returns the job parameter value from the config file
 *
 * @param configFilePath The path to the config file
 * @param jobType The current job type
 * @param parameter The parameter that needs to be retrieved
 *
 * @return The parameter value
 *
 * @throw FileOpenException
 * @throw XmlParseException
 **/
QString JobCreation::getJobParameterValueFromConfigFile(const QString &configFilePath, const QString &jobType,
                                                        const QString &parameter) {
    QFile file(configFilePath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);
    QString currentJobType;
    QString value;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "Option") {
            currentJobType.clear();  // Reset the current job type for each Option block
        }

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "name") {
            currentJobType = xmlReader.readElementText();
        }

        if (!currentJobType.isEmpty() && currentJobType == jobType) {
            if (xmlReader.isStartElement() && xmlReader.name() == parameter) {
                value = xmlReader.readElementText();
            }
        }

        if (xmlReader.isEndElement() && xmlReader.name().toString() == "Option") {
            currentJobType.clear();  // Reset the current job type after exiting an Option block
        }
    }

    if (xmlReader.hasError()) {
        throw XmlParseException("Error parsing XML: " + xmlReader.errorString());
    }

    file.close();
    return value;
}

/**
 * @brief This method creates a job with the given priority using the user input
 *
 * @param priority The job priority
 *
 * @return The created job
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

    job_.checkRequiredParameters(config_file_path_);
    return job_;
}

/**
 * @brief This method returns the job type
 *
 * @return The job type
 **/
QString JobCreation::getJobType() const {
    return job_creation_widget_->getJobTypeComboBox()->currentText();
}

/**
 * @brief This method returns the job name
 *
 * @return The job name
 **/
QString JobCreation::getJobName() const {
    return job_creation_widget_->getJobNameLineEdit()->text();
}

/**
 * @brief This method returns the scene path
 *
 * @return The scene path
 **/
QString JobCreation::getScenePath() const {
    return convertToUncPath(job_creation_widget_->getSceneLineEdit()->text());
}

/**
 * @brief This method returns the output path
 *
 * @return The output path
 **/
QString JobCreation::getOutputPath() const {
    return convertToUncPath(job_creation_widget_->getOutputLineEdit()->text());
}

/**
 * @brief This method returns the name with the prefix of the job type
 *
 * @return The name
 **/
QString JobCreation::getName() {
    QString xmlPrefix = getJobParameterValueFromConfigFile(config_file_path_,
                                                           job_creation_widget_->getJobTypeComboBox()->currentText(),
                                                           "NameOption");
    if (job_creation_widget_->getImagesNameCheckBox()->isChecked()) {
        return xmlPrefix + job_creation_widget_->getImagesNameLineEdit()->text();
    }
    return {};

}

/**
 * @brief This method returns the format with the prefix of the job type
 *
 * @return The format
 **/
QString JobCreation::getFormat() const {
    QString xmlPrefix = getJobParameterValueFromConfigFile(config_file_path_,
                                                           job_creation_widget_->getJobTypeComboBox()->currentText(),
                                                           "FormatOption");
    if (job_creation_widget_->getImagesFormatCheckBox()->isChecked()) {
        QString selection = job_creation_widget_->getImagesFormatComboBox()->currentText();
        QString result;
        if (job_creation_widget_->getJobTypeComboBox()->currentText() != "Blender") {
            result = getRawFormat(selection);
        } else {
            result = getFormatName(selection);
        }
        return xmlPrefix + result;
    }
    return {};
}

/**
 * @brief This method returns the format extension from the selection
 *
 * @param selection The selection from the combobox
 *
 * @return The raw format
 **/
QString JobCreation::getRawFormat(const QString &selection) {
    static QRegularExpression regex(R"(\[\*\.(\w+)\])");
    if (auto match = regex.match(selection); match.hasMatch()) {
        return match.captured(1);
    }
    return {};
}

/**
 * @brief This method returns the format name from the selection
 *
 * @param selection The selection from the combobox
 *
 * @return The format name
 **/
QString JobCreation::getFormatName(const QString &selection) {
    static QRegularExpression regex(R"(\b(.*?)\s*\[\*\.)");
    if (auto match = regex.match(selection); match.hasMatch()) {
        return match.captured(1);
    }
    return {};
}

/**
 * @brief This method returns the first image
 *
 * @return The first image
 **/
QString JobCreation::getFirstImage() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    }
    return job_creation_widget_->getFirstImageSpinBox()->text();
}

/**
 * @brief This method returns the last image
 *
 * @return The last image
 **/
QString JobCreation::getLastImage() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    }
    return job_creation_widget_->getLastImageSpinBox()->text();
}

/**
 * @brief This method returns the first index
 *
 * @return The first index
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
 * @brief This method returns the last index
 *
 * @return The last index
 *
 * @throw BatchCalculationException
 **/
QString JobCreation::getLastIndex() const {
    if (job_creation_widget_->getSingleImageRadioButton()->isChecked()) {
        return job_creation_widget_->getSingleImageSpinBox()->text();
    } else if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        if (job_creation_widget_->getBatchCalculationSpinBox()->value() == 0) {
            throw BatchCalculationException("The batch calculation value cannot be 0");
        }
        int batch_size = job_creation_widget_->getBatchCalculationSpinBox()->value();
        int total_images = job_creation_widget_->getLastImageSpinBox()->value() -
                           job_creation_widget_->getFirstImageSpinBox()->value() + 1;
        int last_batch_index = (total_images + batch_size - 1) / batch_size;
        return QString::number(last_batch_index);
    }
    return job_creation_widget_->getLastImageSpinBox()->text();
}

/**
 * @brief This method returns the camera name
 *
 * @return The camera name
 **/
QString JobCreation::getCameraName() const {
    QString xmlPrefix = getJobParameterValueFromConfigFile(config_file_path_,
                                                           job_creation_widget_->getJobTypeComboBox()->currentText(),
                                                           "CameraOption");
    if (job_creation_widget_->getCameraGroupBox()->isChecked()) {
        return xmlPrefix + job_creation_widget_->getCameraLineEdit()->text();
    }
    return {};
}

/**
 * @brief This method returns the min cpu
 *
 * @return The min cpu
 **/
QString JobCreation::getMinCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()) {
        return job_creation_widget_->getMinCpuSpinBox()->text();
    }
    return {};
}

/**
 * @brief This method returns the max cpu
 *
 * @return The max cpu
 **/
QString JobCreation::getMaxCpu() const {
    if (job_creation_widget_->getCpuCheckBox()->isChecked()) {
        return job_creation_widget_->getMaxCpuSpinBox()->text();
    }
    return {"32"};
}

/**
 * @brief This method returns the submission option with the cpu interval, memory interval and parc style list
 *
 * @return The submission option
 *
 * @throw ParcStyleException
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
        throw ParcStyleException("No parc style selected");
    }

    return submissionOptions.join(" && ");
}

/**
 * @brief This method returns the cpu interval
 *
 * @return The cpu interval
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
 * @brief This method returns the memory interval
 *
 * @return The memory interval
 **/
QString JobCreation::getMemoryInterval() const {
    if (job_creation_widget_->getMemoryCheckBox()->isChecked()) {
        double memory = job_creation_widget_->getMemorySpinBox()->value() * 1024;
        return QString("mem > %1").arg(memory);
    }
    return {};
}

/**
 * @brief This method returns the parc style list
 *
 * @return The parc style list
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
 * @brief This method returns the steps
 *
 * @return The steps
 **/
QString JobCreation::getSteps() {
    if (job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        return QString::number(job_creation_widget_->getBatchCalculationSpinBox()->value());
    }
    return {};
}

/**
 * @brief This method creates and executes a job, the analysis job and the resubmission job if needed with the given priority
 *
 * @param priority The job priority
 **/
void JobCreation::createAndExecuteJob(QString priority) {
    createJob(std::move(priority));
    qDebug() << "Job created : " << job_.getJobName();
    QString jobType = job_.getJobType();
    QString remote_script_path;
    if (!job_creation_widget_->getBatchCalculationCheckBox()->isChecked()) {
        remote_script_path =
                config_file_path_ + getJobParameterValueFromConfigFile(config_file_path_, jobType, "skeleton");
    } else {
        remote_script_path = config_file_path_ +
                             getJobParameterValueFromConfigFile(config_file_path_, jobType, "skeletonMultipleImage");
    }

    QString remote_launchers_path = config_file_path_ + R"(lance.txt)";

    QString local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";
    BaseScript script(job_, remote_script_path, remote_launchers_path, local_job_location);
    script.copyRemoteScript(job_.getJobName() + ".bat", "lance.bat");
    script.replaceScriptParameters(job_.getJobName() + ".bat", "lance.bat");
    QString output = script.executeScript("lance.bat");


    if (job_creation_widget_->getAnalysisCheckBox()->isChecked()) {
        static QRegularExpression regex(R"(\d+)");
        job_.addJobParameter("<previousJobID>", regex.match(output).captured(0));

        remote_script_path = config_file_path_ + R"(skeleton\Skeleton_Analysis.txt)";
        remote_launchers_path = config_file_path_ + R"(lanceAnalysis.txt)";
        local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";

        job_.addJobParameter("<jobName>", job_.getJobName());

        qDebug() << "Remote script path : " << remote_script_path
                 << " Remote launchers path : " << remote_launchers_path
                 << " Local job location : " << local_job_location;

        BaseScript analysis_script(job_, remote_script_path, remote_launchers_path, local_job_location);
        analysis_script.copyRemoteScript(job_.getJobName() + "_Analysis.bat", "lanceAnalysis.bat");

        if (job_creation_widget_->getResubmissionCheckBox()->isChecked()) {
            remote_script_path = config_file_path_ +
                                 getJobParameterValueFromConfigFile(config_file_path_, job_.getJobType(),
                                                                    "skeletonResubmission");

            remote_launchers_path = config_file_path_ + R"(lanceResubmission.txt)";
            local_job_location = R"(I:\Mygale\TEMP\)" + job_.getJobName() + R"(\)";
            job_.addJobParameter("<jobName>", job_.getJobName());
            BaseScript resubmission_script(job_, remote_script_path, remote_launchers_path, local_job_location);
            resubmission_script.copyRemoteScript(job_.getJobName() + "_Resubmission.bat", "lanceResubmission.bat");
            resubmission_script.appendResubmissionJobExecutionLine(
                    config_file_path_ + R"(resubmissionExecutionLine.txt)",
                    job_.getJobName() + "_Analysis.bat");

            resubmission_script.replaceScriptParameters(job_.getJobName() + "_Resubmission.bat",
                                                        "lanceResubmission.bat");
        }

        analysis_script.replaceScriptParameters(job_.getJobName() + "_Analysis.bat", "lanceAnalysis.bat");
        analysis_script.executeScript("lanceAnalysis.bat");
    }
    incrementJobNumber();
    job_creation_widget_->saveUserInput();
    QMessageBox::information(nullptr, "Job created", output);
}

/**
 * @brief This method converts the path to an UNC path
 *
 * @param path The path to convert
 *
 * @return The UNC path
 *
 * @throw PathNotFoundException
 * @throw NotRemoteDriveException
 **/
QString JobCreation::convertToUncPath(const QString &path) {
    // Path = I:/Mygale/TEMP/JobName/
    QString uncPath = path;
    if (!QFileInfo::exists(path)) {
        throw PathNotFoundException("The path " + path + " does not exist");
    }
    QStorageInfo storageInfo(path);
    DWORD driveType = GetDriveType((LPCWSTR) storageInfo.rootPath().utf16());
    if (driveType == DRIVE_REMOTE) {
        return uncPath.replace("I:/", storageInfo.device()).replace("/", "\\");
    } else {
        throw NotRemoteDriveException("The path " + path + " is not a remote drive");
    }
}

/**
 * @brief This method updates the format combobox depending on the job type
 *
 * @param jobType The job type
 **/
void JobCreation::updateFormatComboBox(const QString &jobType) {
    QStringList formats = getFormatsFromConfigFile(config_file_path_, jobType);
    job_creation_widget_->getImagesFormatComboBox()->clear();
    job_creation_widget_->getImagesFormatComboBox()->addItems(formats);
}


