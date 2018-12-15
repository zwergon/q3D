#include "plugin_dialog.h"

#include <q3D/plugins/interfaces.h>
#include <q3D/plugins/plugins.h>

#include <QPluginLoader>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>


namespace Q3D {

PluginsDialog::PluginsDialog( QWidget *parent ) :
    QDialog(parent)
{
    ui_.setupUi(this);
    ui_.tree_widget_->setAlternatingRowColors(false);
    ui_.tree_widget_->setColumnCount(1);
    ui_.tree_widget_->header()->hide();

    interface_icon_.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                              QIcon::Normal, QIcon::On);
    interface_icon_.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                              QIcon::Normal, QIcon::Off);
    feature_icon_.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    findPlugins();
}

void PluginsDialog::findPlugins()
{
    ui_.tree_widget_->clear();
    ui_.label_->setText(tr("Q3D found the following plugins\n"));

    QStringList fileNames = Plugins::instance()->get_plugins();

    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);
        QObject *plugin = loader.instance();
        if (plugin)
            populateTreeWidget(plugin, fileName);
    }
}

void PluginsDialog::populateTreeWidget( QObject *plugin, const QString &text )
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(ui_.tree_widget_);
    pluginItem->setText(0, text);
    ui_.tree_widget_->setItemExpanded(pluginItem, true);

    QFont boldFont = pluginItem->font(0);
    boldFont.setBold(true);
    pluginItem->setFont(0, boldFont);

    PluginCollectionInterface* plugin_collection =
            qobject_cast<PluginCollectionInterface*>(plugin);

    if (plugin_collection != nullptr) {
        foreach (auto p, plugin_collection->plugins()) {
            PluginActionInterface *iTool = qobject_cast<PluginActionInterface *>(p);
            if (iTool != nullptr){
                addItems(pluginItem, "Actions", iTool->tools());
            }
            DriverInterface* iDriver = qobject_cast<DriverInterface*>(p);
            if (iDriver != nullptr){
                addItems(pluginItem, "Drivers", iDriver->drivers() );
            }
        }

    }
}

void PluginsDialog::addItems(QTreeWidgetItem *pluginItem,
                             const char *interfaceName,
                             const QStringList &features)
{
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
    interfaceItem->setText(0, interfaceName);
    interfaceItem->setIcon(0, interface_icon_);

    foreach (QString feature, features) {
        if (feature.endsWith("..."))
            feature.chop(3);
        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, feature);
        featureItem->setIcon(0, feature_icon_);
    }
}

void PluginsDialog::on_add_button__clicked(){
    QStringList files = QFileDialog::getOpenFileNames(
                             this,
                             "Select one or more dll to open",
                             QDir::currentPath(),
                             "shared library (*.so *.dll)");

    foreach (QString fileName, files) {
        QFileInfo finfo( fileName );
        QPluginLoader loader( finfo.absoluteFilePath() );
        qDebug() << " try to load " << finfo.absoluteFilePath() << QLibrary::isLibrary( fileName );

        QObject *plugin = loader.instance();
        if (plugin){
         Plugins::instance()->add_plugin( finfo.absoluteFilePath() );
        }
    }

    findPlugins();
}

void PluginsDialog::on_remove_button__clicked(){
    QListIterator<QTreeWidgetItem*> itw( ui_.tree_widget_->selectedItems() );
    while( itw.hasNext() ){
        QFileInfo finfo( itw.next()->text(0) );
        if ( finfo.exists() ){
            Plugins::instance()->remove_plugin( finfo.absoluteFilePath() );
        }
    }

    findPlugins();
}

}//end namespace Q3D
