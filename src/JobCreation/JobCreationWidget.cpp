//
// Created by SD43247 on 03/07/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MygaleWidget.h" resolved
#include "JobCreationWidget.h++"
#include "../../resources/ui/ui_JobCreationWidget.h"

/**
 * @class JobCreationWidget
 *
 * @brief The JobCreationWidget class is responsible for initializing the job creation widget.
 **/
JobCreationWidget::JobCreationWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::JobCreationWidget) {
    ui->setupUi(this);
}

/**
 * @brief Destroys the job creation widget.
 **/
JobCreationWidget::~JobCreationWidget() {
    delete ui;
}

/**
 * @brief Initializes the job creation widget.
 **/
void JobCreationWidget::initialize() {
    loadPcPoolManagmentChoice();
}

/**
 * @brief Loads the pc pool management check box from the config file.
 *
 * @throws FileOpenException
 **/
void JobCreationWidget::loadPcPoolManagmentChoice() {
    QStringList pcPoolManagementOptions;
    int row = 0;
    int column = 0;

    QFile file(R"(I:\Mygale\Config_Blender_4_V2\mainConfig.xml)"); //TODO: change to config file path
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (!xmlReader.isStartElement() || xmlReader.name().toString() != "PcPoolManagementOption") {
            continue;
        }

        while (xmlReader.readNextStartElement()) {
            if (xmlReader.name().toString() == "Option") {
                pcPoolManagementOptions.append(xmlReader.readElementText());
            } else {
                xmlReader.skipCurrentElement();
            }
        }
    }
    file.close();

    for (const auto &option: pcPoolManagementOptions) {
        auto *checkBox = new QCheckBox(option);
        checkBox->setProperty("LSF", option.toLower());
        getPcPoolManagementGridLayout()->addWidget(checkBox, row / 3, column % 3);
        ++column;
        ++row;
    }
}

/**
 * @brief Returns the single image radio button.
 *
 * @return QRadioButton
 **/
QRadioButton *JobCreationWidget::getSingleImageRadioButton() {
    return ui->singleImageRadioButton;
}

/**
 * @brief Returns the single image label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getSingleImageLabel() {
    return ui->singleImageLabel;
}

/**
 * @brief Returns the single image spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getSingleImageSpinBox() {
    return ui->singleImageSpinBox;
}

/**
 * @brief Returns the multiple images radio button.
 *
 * @return QRadioButton
 **/
QRadioButton *JobCreationWidget::getMultipleImagesRadioButton() {
    return ui->multipleImagesRadioButton;
}

/**
 * @brief Returns the first image label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getFirstImageLabel() {
    return ui->firstImageLabel;
}

/**
 * @brief Returns the last image label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getLastImageLabel() {
    return ui->lastImageLabel;
}

/**
 * @brief Returns the first image spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getFirstImageSpinBox() {
    return ui->firstImageSpinBox;
}

/**
 * @brief Returns the last image spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getLastImageSpinBox() {
    return ui->lastImageSpinBox;
}

/**
 * @brief Returns the batch calculation check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getBatchCalculationCheckBox() {
    return ui->batchCalculationCheckBox;
}

/**
 * @brief Returns the batch calculation label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getBatchCalculationLabel() {
    return ui->batchCalculationLabel;
}

/**
 * @brief Returns the batch calculation spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getBatchCalculationSpinBox() {
    return ui->batchCalculationSpinBox;
}

/**
 * @brief Returns the images name check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getImagesNameCheckBox() {
    return ui->imagesNameCheckBox;
}

/**
 * @brief Returns the images name line edit.
 *
 * @return QLineEdit
 **/
QLineEdit *JobCreationWidget::getImagesNameLineEdit() {
    return ui->imagesNameLineEdit;
}

/**
 * @brief Returns the images format check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getImagesFormatCheckBox() {
    return ui->imagesFormatCheckBox;
}

/**
 * @brief Returns the images format combo box.
 *
 * @return QComboBox
 **/
QComboBox *JobCreationWidget::getImagesFormatComboBox() {
    return ui->imagesFormatComboBox;
}

/**
 * @brief Returns the cpu check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getCpuCheckBox() {
    return ui->cpuCheckBox;
}

/**
 * @brief Returns the min cpu label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getMinCpuLabel() {
    return ui->minCpuLabel;
}

/**
 * @brief Returns the min cpu spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getMinCpuSpinBox() {
    return ui->minCpuSpinBox;
}

/**
 * @brief Returns the max cpu label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getMaxCpuLabel() {
    return ui->maxCpuLabel;
}

/**
 * @brief Returns the max cpu spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getMaxCpuSpinBox() {
    return ui->maxCpuSpinBox;
}

/**
 * @brief Returns the memory check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getMemoryCheckBox() {
    return ui->memoryCheckBox;
}

/**
 * @brief Returns the memory label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getMemoryLabel() {
    return ui->memoryLabel;
}

/**
 * @brief Returns the memory spin box.
 *
 * @return QSpinBox
 **/
QSpinBox *JobCreationWidget::getMemorySpinBox() {
    return ui->memorySpinBox;
}

/**
 * @brief Returns the memory suffix label.
 *
 * @return QLabel
 **/
QLabel *JobCreationWidget::getMemorySuffixLabel() {
    return ui->memorySuffixLabel;
}

/**
 * @brief Returns the analysis check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getAnalysisCheckBox() {
    return ui->analysisCheckBox;
}

/**
 * @brief Returns the resubmission check box.
 *
 * @return QCheckBox
 **/
QCheckBox *JobCreationWidget::getResubmissionCheckBox() {
    return ui->resubmissionCheckBox;
}

/**
 * @brief Returns the job name line edit.
 *
 * @return QLineEdit
 **/
QLineEdit *JobCreationWidget::getJobNameLineEdit() {
    return ui->jobNameLineEdit;
}

/**
 * @brief Returns the job name tool button.
 *
 * @return QToolButton
 **/
QToolButton *JobCreationWidget::getJobNameToolButton() {
    return ui->jobNameToolButton;
}

/**
 * @brief Returns the scene line edit.
 *
 * @return QLineEdit
 **/
QLineEdit *JobCreationWidget::getSceneLineEdit() {
    return ui->sceneLineEdit;
}

/**
 * @brief Returns the scene tool button.
 *
 * @return QToolButton
 **/
QToolButton *JobCreationWidget::getSceneToolButton() {
    return ui->sceneToolButton;
}

/**
 * @brief Returns the output line edit.
 *
 * @return QLineEdit
 **/
QLineEdit *JobCreationWidget::getOutputLineEdit() {
    return ui->outputLineEdit;
}

/**
 * @brief Returns the output tool button.
 *
 * @return QToolButton
 **/
QToolButton *JobCreationWidget::getOutputToolButton() {
    return ui->outputToolButton;
}

/**
 * @brief Returns the job type combo box.
 *
 * @return QComboBox
 **/
QComboBox *JobCreationWidget::getJobTypeComboBox() {
    return ui->jobTypeComboBox;
}

/**
 * @brief Returns the camera line edit.
 *
 * @return QLineEdit
 **/
QLineEdit *JobCreationWidget::getCameraLineEdit() {
    return ui->cameraLineEdit;
}

/**
 * @brief Returns the camera tool button.
 *
 * @return QToolButton
 **/
QGroupBox *JobCreationWidget::getCameraGroupBox() {
    return ui->cameraGroupBox;
}

/**
 * @brief Returns the execution push button.
 *
 * @return QPushButton
 **/
QPushButton *JobCreationWidget::getExecutionPushButton() {
    return ui->executionPushButton;
}

/**
 * @brief Returns the priority execution push button.
 *
 * @return QPushButton
 **/
QPushButton *JobCreationWidget::getPriorityExecutionPushButton() {
    return ui->priorityExecutionPushButton;
}

/**
 * @brief Returns the pc pool management grid layout.
 *
 * @return QGridLayout
 **/
QGridLayout *JobCreationWidget::getPcPoolManagementGridLayout() {
    return ui->pcPoolManagementGridLayout;
}

/**
 * @brief Enables or disables the single image radio button.
 * @param checked
 **/
void JobCreationWidget::singleImageRadioButtonToggled(bool checked) {
    getSingleImageLabel()->setEnabled(checked);
    getSingleImageSpinBox()->setEnabled(checked);

    if (checked) {
        getBatchCalculationCheckBox()->setChecked(false);
        getBatchCalculationCheckBox()->setEnabled(false);
    }
}

/**
 * @brief Enables or disables the multiple images radio button.
 * @param checked
 **/
void JobCreationWidget::multipleImagesRadioButtonToggled(bool checked) {
    getFirstImageSpinBox()->setEnabled(checked);
    getFirstImageLabel()->setEnabled(checked);

    getLastImageSpinBox()->setEnabled(checked);
    getLastImageLabel()->setEnabled(checked);

    getBatchCalculationCheckBox()->setEnabled(checked);
}

/**
 * @brief Enables or disables the batch calculation check box.
 * @param checked
 **/
void JobCreationWidget::batchCalculationCheckBoxToggled(bool checked) {
    getBatchCalculationSpinBox()->setEnabled(checked);
    getBatchCalculationLabel()->setEnabled(checked);
}

/**
 * @brief Enables or disables the images name check box.
 * @param checked
 **/
void JobCreationWidget::imagesNameCheckBoxToggled(bool checked) {
    getImagesNameLineEdit()->setEnabled(checked);
    getAnalysisCheckBox()->setEnabled(checked);

    if (!checked) {
        getResubmissionCheckBox()->setChecked(false);
        getAnalysisCheckBox()->setChecked(false);
    }

}

/**
 * @brief Enables or disables the images format check box.
 * @param checked
 **/
void JobCreationWidget::imagesFormatCheckBoxToggled(bool checked) {
    getImagesFormatComboBox()->setEnabled(checked);
}

/**
 * @brief Enables or disables the cpu check box.
 * @param checked
 **/
void JobCreationWidget::cpuCheckBoxToggled(bool checked) {
    getMinCpuLabel()->setEnabled(checked);
    getMinCpuSpinBox()->setEnabled(checked);

    getMaxCpuLabel()->setEnabled(checked);
    getMaxCpuSpinBox()->setEnabled(checked);
}

/**
 * @brief Enables or disables the memory check box.
 * @param checked
 **/
void JobCreationWidget::memoryCheckBoxToggled(bool checked) {
    getMemoryLabel()->setEnabled(checked);
    getMemorySpinBox()->setEnabled(checked);
    getMemorySuffixLabel()->setEnabled(checked);
}

/**
 * @brief Enables or disables the analysis check box.
 * @param checked
 **/
void JobCreationWidget::analysisCheckBoxToggled(bool checked) {
    getResubmissionCheckBox()->setEnabled(checked);
}

/**
 * @brief Saves the user input.
 */
void JobCreationWidget::saveUserInput() const {
    qDebug() << "Saving user input";
    QSettings settings("Stellantis", "Mygale");
    settings.beginGroup("MainWindow");
    settings.beginGroup("JobCreationWidget");

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






