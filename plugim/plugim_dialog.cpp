#include "plugim_dialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialogButtonBox>

namespace Q3D {

PlugImDialog::PlugImDialog(const QDomDocument& document, QWidget *parent)
    : QDialog(parent), document_(document)
{

    QVBoxLayout* main_layout = new QVBoxLayout;

    QDomElement doc_elt = document.documentElement();
    for(
        QDomElement param_elt = doc_elt.firstChildElement("Param");
        !param_elt.isNull();
        param_elt = param_elt.nextSiblingElement("Param"))
    {

        QWidget* widget = widget_creator_.create(param_elt, this);
        if ( nullptr != widget ){

            main_layout->addWidget(widget);
        }

    }

    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    main_layout->addWidget(button_box);

    connect(button_box, &QDialogButtonBox::accepted, this, &PlugImDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &PlugImDialog::reject);

    setLayout(main_layout);
    setWindowTitle("Parameters");

}

QList<ProcessParam> PlugImDialog::getParams() const {

    QList<ProcessParam> param_list;
    QDomElement doc_elt = document_.documentElement();

    for(
        QDomElement param_elt = doc_elt.firstChildElement("Param");
        !param_elt.isNull();
        param_elt = param_elt.nextSiblingElement("Param"))
    {
        QString id = param_elt.attribute("Id");
        QWidget* w = findChild<QWidget*>(id);
        if ( w != nullptr ){

            QString value = widget_creator_.getValue(param_elt, w);
            param_list.append(ProcessParam(id, value));
        }
    }
    return param_list;
}


}
