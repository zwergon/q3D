#ifndef PLUGIM_DIALOG_H
#define PLUGIM_DIALOG_H

#include <q3D/plugim/plugim_global.h>

#include <QDialog>
#include <QDomDocument>

#include <q3D/model/process_info.h>
#include <q3D/plugim/widget_creator.h>


namespace Q3D {

class PLUGIMSHARED_EXPORT PlugImDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlugImDialog( const QDomDocument& document, QWidget *parent = nullptr);

    bool hasParam() const { return has_param_; }

    QList<ProcessParam> getParams() const;

protected:
    bool has_param_;
    QDomDocument document_;
    WidgetCreator widget_creator_;
};

}

#endif // PLUGIM_DIALOG_H
