#ifndef PLUGIN_DIALOG_H
#define PLUGIN_DIALOG_H

#include <q3D/plugins/plugins_global.h>

#include <QDialog>
#include <QIcon>

#include "ui_plugin_dialog.h"

namespace Q3D {

class PLUGINSSHARED_EXPORT PluginsDialog : public QDialog
{
    Q_OBJECT

public:
    PluginsDialog( QWidget *parent = 0 );

public slots:
    void on_add_button__clicked();
    void on_remove_button__clicked();

private:
    void findPlugins();
    void populateTreeWidget(QObject *plugin, const QString &text);
    void addItems(
            QTreeWidgetItem *pluginItem,
            const char *interfaceName,
            const QStringList &features );

    Ui::PluginDialogBase ui_;
    QIcon interface_icon_;
    QIcon feature_icon_;
};


}

#endif // PLUGIN_DIALOG_H
