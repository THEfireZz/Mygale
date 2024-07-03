//
// Created by SD43247 on 03/07/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MygaleWidget.h" resolved

#include "JobCreationWidget.h++"
#include "../../resources/ui/ui_JobCreationWidget.h"


JobCreationWidget::JobCreationWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::JobCreationWidget) {
    ui->setupUi(this);
}

JobCreationWidget::~JobCreationWidget() {
    delete ui;
}

QRadioButton *JobCreationWidget::getSingleImageRadioButton() {
    return ui->singleImageRadioButton;
}

QLabel *JobCreationWidget::getSingleImageLabel() {
    return ui->singleImageLabel;
}

QSpinBox *JobCreationWidget::getSingleImageSpinBox() {
    return ui->singleImageSpinBox;
}

QRadioButton *JobCreationWidget::getMultipleImagesRadioButton() {
    return ui->multipleImagesRadioButton;
}

QLabel *JobCreationWidget::getFirstImageLabel() {
    return ui->firstImageLabel;
}

QLabel *JobCreationWidget::getLastImageLabel() {
    return ui->lastImageLabel;
}

QSpinBox *JobCreationWidget::getFirstImageSpinBox() {
    return ui->firstImageSpinBox;
}

QSpinBox *JobCreationWidget::getLastImageSpinBox() {
    return ui->lastImageSpinBox;
}

QCheckBox *JobCreationWidget::getBatchCalculationCheckBox() {
    return ui->batchCalculationCheckBox;
}

QLabel *JobCreationWidget::getBatchCalculationLabel() {
    return ui->batchCalculationLabel;
}

QSpinBox *JobCreationWidget::getBatchCalculationSpinBox() {
    return ui->batchCalculationSpinBox;
}

void JobCreationWidget::initialize() {
    connectSignalsAndSlots();
}

void JobCreationWidget::connectSignalsAndSlots() {

    //Handle elements enable/disable
    connect(getSingleImageRadioButton(), &QRadioButton::toggled,
            [this](bool checked) { singleImageRadioButtonToggled(checked); });

    connect(getMultipleImagesRadioButton(), &QRadioButton::toggled,
            [this](bool checked) { multipleImagesRadioButtonToggled(checked); });

    connect(getBatchCalculationCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { batchCalculationCheckBoxToggled(checked); });

    connect(getImagesNameCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { imagesNameCheckBoxToggled(checked); });

    connect(getImagesFormatCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { imagesFormatCheckBoxToggled(checked); });

    connect(getCpuCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { cpuCheckBoxToggled(checked); });

    connect(getMemoryCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { memoryCheckBoxToggled(checked); });

    connect(getAnalysisCheckBox(), &QCheckBox::toggled,
            [this](bool checked) { analysisCheckBoxToggled(checked); });
}

/**
 * @class JobCreationWidget
 * @brief This method handles the toggling of the single image radio button
 *
 * @param checked The state of the radio button
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
 * @class JobCreationWidget
 * @brief This method handles the toggling of the multiple images radio button
 *
 * @param checked The state of the radio button
 **/
void JobCreationWidget::multipleImagesRadioButtonToggled(bool checked) {
    getFirstImageSpinBox()->setEnabled(checked);
    getFirstImageLabel()->setEnabled(checked);

    getLastImageSpinBox()->setEnabled(checked);
    getLastImageLabel()->setEnabled(checked);

    getBatchCalculationCheckBox()->setEnabled(checked);
}

/**
 * @class JobCreationWidget
 * @brief This method handles the toggling of the batch calculation check box
 *
 * @param checked The state of the check box
 **/
void JobCreationWidget::batchCalculationCheckBoxToggled(bool checked) {
    getBatchCalculationSpinBox()->setEnabled(checked);
    getBatchCalculationLabel()->setEnabled(checked);
}

QCheckBox *JobCreationWidget::getImagesNameCheckBox() {
    return ui->imagesNameCheckBox;
}

QLineEdit *JobCreationWidget::getImagesNameLineEdit() {
    return ui->imagesNameLineEdit;
}

QCheckBox *JobCreationWidget::getImagesFormatCheckBox() {
    return ui->imagesFormatCheckBox;
}

QComboBox *JobCreationWidget::getImagesFormatComboBox() {
    return ui->imagesFormatComboBox;
}

QCheckBox *JobCreationWidget::getCpuCheckBox() {
    return ui->cpuCheckBox;
}

QLabel *JobCreationWidget::getMinCpuLabel() {
    return ui->minCpuLabel;
}

QSpinBox *JobCreationWidget::getMinCpuSpinBox() {
    return ui->minCpuSpinBox;
}

QLabel *JobCreationWidget::getMaxCpuLabel() {
    return ui->maxCpuLabel;
}

QSpinBox *JobCreationWidget::getMaxCpuSpinBox() {
    return ui->maxCpuSpinBox;
}

QCheckBox *JobCreationWidget::getMemoryCheckBox() {
    return ui->memoryCheckBox;
}

QLabel *JobCreationWidget::getMemoryLabel() {
    return ui->memoryLabel;
}

QSpinBox *JobCreationWidget::getMemorySpinBox() {
    return ui->memorySpinBox;
}

QLabel *JobCreationWidget::getMemorySuffixLabel() {
    return ui->memorySuffixLabel;
}

QCheckBox *JobCreationWidget::getAnalysisCheckBox() {
    return ui->analysisCheckBox;
}

QCheckBox *JobCreationWidget::getResubmissionCheckBox() {
    return ui->resubmissionCheckBox;
}

void JobCreationWidget::imagesNameCheckBoxToggled(bool checked) {
    getImagesNameLineEdit()->setEnabled(checked);
    getAnalysisCheckBox()->setEnabled(checked);

    if (!checked) {
        getResubmissionCheckBox()->setChecked(false);
        getAnalysisCheckBox()->setChecked(false);
    }

}

void JobCreationWidget::imagesFormatCheckBoxToggled(bool checked) {
    getImagesFormatComboBox()->setEnabled(checked);
}

void JobCreationWidget::cpuCheckBoxToggled(bool checked) {
    getMinCpuLabel()->setEnabled(checked);
    getMinCpuSpinBox()->setEnabled(checked);

    getMaxCpuLabel()->setEnabled(checked);
    getMaxCpuSpinBox()->setEnabled(checked);
}

void JobCreationWidget::memoryCheckBoxToggled(bool checked) {
    getMemoryLabel()->setEnabled(checked);
    getMemorySpinBox()->setEnabled(checked);
    getMemorySuffixLabel()->setEnabled(checked);
}

void JobCreationWidget::analysisCheckBoxToggled(bool checked) {
    getResubmissionCheckBox()->setEnabled(checked);
}


