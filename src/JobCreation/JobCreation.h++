//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_JOBCREATION_H
#define MYGALE_JOBCREATION_H


#include "JobCreationWidget.h++"

class JobCreation {
public:
    explicit JobCreation(JobCreationWidget *job_creation_widget);

    ~JobCreation() = default;

    void initialize();
private:
    JobCreationWidget *job_creation_widget_;

    void connectSignalsAndSlots();
};


#endif //MYGALE_JOBCREATION_H
