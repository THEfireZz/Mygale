//
// Created by SD43247 on 03/07/2024.
//

#include "JobCreation.h++"


/**
 * @class JobCreation
 * @brief This method initializes the JobCreation class and connects the signals
 **/
void JobCreation::initialize() {
    job_creation_widget_->initialize();
    connectSignalsAndSlots();
}

void JobCreation::connectSignalsAndSlots() {
    ;
}


JobCreation::JobCreation(JobCreationWidget *job_creation_widget) : job_creation_widget_(job_creation_widget) {
}


