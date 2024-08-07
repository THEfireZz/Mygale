//
// Created by SD43247 on 03/07/2024.
//

#include <utility>
#include <QSettings>

#include "JobCreation.h++"
#include "../exception/CustomErrors.h++"

JobCreation::JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath) : job_creation_widget_(
        job_creation_widget), config_file_path_(std::move(configFilePath)) {
}

/**
 * @class JobCreation
 * @brief This method initializes the JobCreation class and connects the signals
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
        } catch (const FileCopyException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const FileOpenException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const ParcStyleException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const BatchCalculationException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const PathNotFoundException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const NotRemoteDriveException &e) {
            QMessageBox::critical(nullptr, "Job creation error", e.what());
        } catch (const MissingRequiredParameterException &e) {
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
    QFile file(configFilePath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
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

QString JobCreation::getJobParameterValueFromConfigFile(const QString &configFilePath, const QString &jobType,
                                                        const QString &parameter) {
    QString value;

    QFile file(configFilePath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement() && xmlReader.name().toString() == "Option") {
            QString currentJobType;
            while (xmlReader.readNextStartElement()) {
                if (xmlReader.name().toString() == "name") {
                    currentJobType = xmlReader.readElementText();
                } else if (!currentJobType.isEmpty() && currentJobType == jobType) {
                    if (xmlReader.name() == parameter) {
                        value = xmlReader.readElementText();
                        file.close();
                        return value;
                    } else {
                        xmlReader.skipCurrentElement();
                    }
                } else {
                    xmlReader.skipCurrentElement();
                }
            }
        }
    }

    file.close();
    return value;
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
    qDebug() << "Name : " << name;
    QString format = getFormat();
    qDebug() << "Format : " << format;
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
    QString xmlPrefix = getJobParameterValueFromConfigFile(config_file_path_,
                                                           job_creation_widget_->getJobTypeComboBox()->currentText(),
                                                           "NameOption");
    if (job_creation_widget_->getImagesNameCheckBox()->isChecked()) {
        return xmlPrefix + job_creation_widget_->getImagesNameLineEdit()->text();
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
 * @class JobCreation
 * @brief This method returns the camera name from the job creation widget and adds the prefix if the job type is Maya_2023/Vray and adds the prefix if needed
 *
 * @return the camera name
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
        throw ParcStyleException("No parc style selected");
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


void JobCreation::updateFormatComboBox(const QString &jobType) {
    qDebug() << "Update format combo box";
    qDebug() << "Job type parameter: " << jobType;
    QStringList formats = getFormatsFromConfigFile(config_file_path_, jobType);
    qDebug() << "Formats : " << formats;
    job_creation_widget_->getImagesFormatComboBox()->clear();
    job_creation_widget_->getImagesFormatComboBox()->addItems(formats);
}


