#ifndef PARAMS_DIALOG_H
#define PARAMS_DIALOG_H

#include <q3D/gui/gui_global.h>

#include <QDialog>
#include <QDomDocument>
#include <QHash>

#include <q3D/model/process_info.h>
#include <q3D/gui/widget_creator.h>


namespace Q3D {


class GUISHARED_EXPORT ParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParamsDialog( const QDomDocument& document, QWidget *parent = nullptr);

    bool hasParam() const { return has_param_; }

    QList<ParamsElement> getParams() const;

protected:
    bool has_param_;
    QDomDocument document_;
    WidgetCreator widget_creator_;
};

}

#endif // PARAMS_DIALOG_H
