//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_JOBCREATIONWIDGET_H
#define MYGALE_JOBCREATIONWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class JobCreationWidget; }
QT_END_NAMESPACE

class JobCreationWidget : public QWidget {
Q_OBJECT

public:
    explicit JobCreationWidget(QWidget *parent = nullptr);

    ~JobCreationWidget() override;

private:
    Ui::JobCreationWidget *ui;
};


#endif //MYGALE_JOBCREATIONWIDGET_H
