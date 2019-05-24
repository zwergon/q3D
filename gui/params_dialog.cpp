#include "params_dialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialogButtonBox>

namespace Q3D {

ParamsDialog::ParamsDialog(const QDomDocument& document, QWidget *parent)
    : QDialog(parent), document_(document), has_param_(false)
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
            has_param_ = true;
        }

    }

    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    main_layout->addWidget(button_box);

    connect(button_box, &QDialogButtonBox::accepted, this, &ParamsDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &ParamsDialog::reject);

    setLayout(main_layout);
    setWindowTitle("Parameters");

}

QList<ParamsElement> ParamsDialog::getParams() const {

    QList<ParamsElement> param_list;
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
            ParamsElement p_elt;
            p_elt.setId(id);
            p_elt.setValue(value);
            param_list.append(p_elt);
        }
    }
    return param_list;
}


}
