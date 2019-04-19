#ifndef DRIVER_DIALOG_H
#define DRIVER_DIALOG_H

#include <q3D/gui/gui_global.h>

#include <QDialog>
#include <QComboBox>

namespace Q3D {

class Model;
class ModelDriver;

class DriverDialog : public QDialog
{
    Q_OBJECT
public:
    DriverDialog(Model* model, QWidget* parent = nullptr);

    ModelDriver* selectedDriver() const;

private:
    QWidget* createCombo(QWidget* parent);

private:
    QComboBox* cb_;
};

}

#endif // DRIVER_DIALOG_H
