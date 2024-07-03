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

    //imagesFormatComboBox
    [[nodiscard]] QComboBox *getImagesFormatComboBox();

    //cpuCheckBox
    [[nodiscard]] QCheckBox *getCpuCheckBox();

    //minCpuLabel
    [[nodiscard]] QLabel *getMinCpuLabel();

    //minCpuSpinBox
    [[nodiscard]] QSpinBox *getMinCpuSpinBox();

    //maxCpuLabel
    [[nodiscard]] QLabel *getMaxCpuLabel();

    //maxCpuSpinBox
    [[nodiscard]] QSpinBox *getMaxCpuSpinBox();

    //memoryCheckBox
    [[nodiscard]] QCheckBox *getMemoryCheckBox();

    //memoryLabel
    [[nodiscard]] QLabel *getMemoryLabel();

    //memorySpinBox
    [[nodiscard]] QSpinBox *getMemorySpinBox();

    //memorySuffixLabel
    [[nodiscard]] QLabel *getMemorySuffixLabel();

    //analysisCheckBox
    [[nodiscard]] QCheckBox *getAnalysisCheckBox();

    //resubmissionCheckBox
    [[nodiscard]] QCheckBox *getResubmissionCheckBox();


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
