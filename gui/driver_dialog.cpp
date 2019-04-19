#include "driver_dialog.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>

#include <q3D/model/model.h>
#include <q3D/model/model_driver.h>
#include <q3D/model/model_driver_mgr.h>

namespace Q3D {

QWidget* DriverDialog::createCombo(QWidget* parent){
    QGroupBox* group = new QGroupBox(parent);
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label = new QLabel(group);
    label->setText("Drivers");
    layout->addWidget(label);

    cb_ = new QComboBox(group);
    layout->addWidget(cb_);

    group->setLayout(layout);

    return group;
}

DriverDialog::DriverDialog(Model* model, QWidget* parent) : QDialog(parent)
{

    QVBoxLayout* main_layout = new QVBoxLayout;

    main_layout->addWidget(createCombo(parent));

    ModelDriverManager* mgr =  ModelDriverManager::instance();
    for(int i =0; i < mgr->getDriverCount(); ++i ){
        ModelDriver* driver = mgr->getDriver(i);
        if (driver->canHandle(model)){
            cb_->addItem(driver->description());
        }
    }

    cb_->setCurrentText(model->driver()->description());

    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    main_layout->addWidget(button_box);



    connect(button_box, &QDialogButtonBox::accepted, this, &DriverDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &DriverDialog::reject);

    setLayout(main_layout);
    setWindowTitle("Drivers");
}


ModelDriver* DriverDialog::selectedDriver() const {
   ModelDriverManager* mgr =  ModelDriverManager::instance();
   return mgr->getDriverByName(cb_->currentText());
}


}
