//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_JOBCREATIONWIDGET_H
#define MYGALE_JOBCREATIONWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QToolButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class JobCreationWidget; }
QT_END_NAMESPACE

class JobCreationWidget : public QWidget {
Q_OBJECT

public:
    explicit JobCreationWidget(QWidget *parent = nullptr);

    ~JobCreationWidget() override;

    void initialize();

    //Ui getter
    [[nodiscard]] QRadioButton *getSingleImageRadioButton();

    [[nodiscard]] QLabel *getSingleImageLabel();

    [[nodiscard]] QSpinBox *getSingleImageSpinBox();

    [[nodiscard]] QRadioButton *getMultipleImagesRadioButton();

    [[nodiscard]] QLabel *getFirstImageLabel();

    [[nodiscard]] QLabel *getLastImageLabel();

    [[nodiscard]] QSpinBox *getFirstImageSpinBox();

    [[nodiscard]] QSpinBox *getLastImageSpinBox();

    [[nodiscard]] QCheckBox *getBatchCalculationCheckBox();

    [[nodiscard]] QLabel *getBatchCalculationLabel();

    [[nodiscard]] QSpinBox *getBatchCalculationSpinBox();

    [[nodiscard]] QCheckBox *getImagesNameCheckBox();

    [[nodiscard]] QLineEdit *getImagesNameLineEdit();

    [[nodiscard]] QCheckBox *getImagesFormatCheckBox();

    [[nodiscard]] QComboBox *getImagesFormatComboBox();

    [[nodiscard]] QCheckBox *getCpuCheckBox();

    [[nodiscard]] QLabel *getMinCpuLabel();

    [[nodiscard]] QSpinBox *getMinCpuSpinBox();

    [[nodiscard]] QLabel *getMaxCpuLabel();

    [[nodiscard]] QSpinBox *getMaxCpuSpinBox();

    [[nodiscard]] QCheckBox *getMemoryCheckBox();

    [[nodiscard]] QLabel *getMemoryLabel();

    [[nodiscard]] QSpinBox *getMemorySpinBox();

    [[nodiscard]] QLabel *getMemorySuffixLabel();

    [[nodiscard]] QCheckBox *getAnalysisCheckBox();

    [[nodiscard]] QCheckBox *getResubmissionCheckBox();

    [[nodiscard]] QLineEdit *getJobNameLineEdit();

    [[nodiscard]] QToolButton *getJobNameToolButton();

    [[nodiscard]] QLineEdit *getSceneLineEdit();

    [[nodiscard]] QToolButton *getSceneToolButton();

    [[nodiscard]] QLineEdit *getOutputLineEdit();

    [[nodiscard]] QToolButton *getOutputToolButton();

    [[nodiscard]] QComboBox *getJobTypeComboBox();

    [[nodiscard]] QLineEdit *getCameraLineEdit();

    [[nodiscard]] QGroupBox *getCameraGroupBox();

    [[nodiscard]] QPushButton *getExecutionPushButton();





private:
    Ui::JobCreationWidget *ui;

    void connectSignalsAndSlots();

    void singleImageRadioButtonToggled(bool checked);

    void multipleImagesRadioButtonToggled(bool checked);

    void batchCalculationCheckBoxToggled(bool checked);

    void imagesNameCheckBoxToggled(bool checked);

    void imagesFormatCheckBoxToggled(bool checked);

    void cpuCheckBoxToggled(bool checked);

    void memoryCheckBoxToggled(bool checked);

    void analysisCheckBoxToggled(bool checked);


};


#endif //MYGALE_JOBCREATIONWIDGET_H
