/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Francois
 *
 * $Id: main_window.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */
#include "main_window.h"
#include "ui_main_window.h"

#include <QString>
#include <QLayout>
#include <QFileDialog>
#include <QScopedPointer>


#include <q3D/model/model.h>
#include <q3D/model/model_driver.h>
#include <q3D/model/renderer.h>
#include <q3D/model/model_mgr.h>
#include <q3D/model/process_mgr.h>


#include <q3D/plugins/plugin_dialog.h>
#include <q3D/plugins/plugins.h>
#include <q3D/plugins/plugin_action.h>
#include <q3D/plugins/plugin_collection.h>


#include <q3D/gui/gl_area.h>

#include <q3D/gui/renderer_menu.h>
#include <q3D/gui/camera_tool.h>
#include <q3D/gui/picking_tool.h>
#include <q3D/gui/tool_manager.h>
#include <q3D/gui/terminal_log.h>
#include <q3D/gui/driver_dialog.h>

Q_DECLARE_METATYPE( Q3D::Model* );

namespace Q3D {

/*********************************************************/

class CTreeViewItem : public QTreeWidgetItem {
public:
    CTreeViewItem(QTreeWidget* parent);
    CTreeViewItem(QTreeWidgetItem* parentItem);
    virtual ~CTreeViewItem();

    virtual Model* model() const = 0;
};


CTreeViewItem::CTreeViewItem(QTreeWidget* parent) : QTreeWidgetItem(parent){}

CTreeViewItem::CTreeViewItem(QTreeWidgetItem* parentItem) : QTreeWidgetItem(parentItem){}

CTreeViewItem::~CTreeViewItem(){}

/*********************************************************/



class ModelTreeViewItem : public CTreeViewItem {
public:
    ModelTreeViewItem( Model* model, QTreeWidget* parent );

    virtual Model* model() const override;

    CTreeViewItem* defaultRenderer();

private:
    Model* model_;
};


/*********************************************************/

class ModelRendererTreeViewItem : public CTreeViewItem {
public:
    ModelRendererTreeViewItem( ModelTreeViewItem*, const QString& renderer_name );

    virtual ~ModelRendererTreeViewItem() override;

    ModelRenderer* renderer();

    virtual Model* model() const override;

private:
    QString renderer_name_;
    ModelRenderer* renderer_;

};


ModelRendererTreeViewItem::ModelRendererTreeViewItem( ModelTreeViewItem* itemParent,
                                                      const QString& renderer_name  )
    :	CTreeViewItem(itemParent),
      renderer_name_(renderer_name),
      renderer_(nullptr)
{
    setText( 0, renderer_name );
    setFlags( Qt::ItemIsUserCheckable|Qt::ItemIsEnabled );
    setCheckState( 0, Qt::Unchecked );
}

ModelRendererTreeViewItem::~ModelRendererTreeViewItem() {
    delete renderer_;
}


ModelRenderer* ModelRendererTreeViewItem::renderer(){
    if ( nullptr == renderer_ ){
        ModelDriver* driver = model()->driver();
        renderer_ = driver->createRenderer( model(), renderer_name_ );
    }
    return renderer_;
}

Model* ModelRendererTreeViewItem::model() const  {
    return static_cast<ModelTreeViewItem*>(parent())->model();
}




/*********************************************************/

ModelTreeViewItem::ModelTreeViewItem( Model* model, QTreeWidget* parent )
:	CTreeViewItem(parent),
    model_(model)
{
	setText( 0, model->objectName() );
	setExpanded( true );
	
    ModelDriver* driver = model_->driver();
	
	QList<QString> keys;
	driver->rendererKeys( keys );
	for( QList<QString>::iterator itr = keys.begin();
		 itr != keys.end();
		 itr++ ){
			 new ModelRendererTreeViewItem( this, *itr );
	}
}

Model* ModelTreeViewItem::model() const {
    return model_;
}

CTreeViewItem* ModelTreeViewItem::defaultRenderer() {

    QString defaultRendererKey = model_->driver()->defaultRendererKey();
    for( int i =0; i<childCount(); i++ ){
        ModelRendererTreeViewItem* renderer_item =
                dynamic_cast<ModelRendererTreeViewItem*>(child(i));
        if ( nullptr != renderer_item ){
            if ( renderer_item->text(0) == defaultRendererKey ){
                return renderer_item;
            }
        }
    }

    return nullptr;
}

/*********************************************************/

CGlWindow::CGlWindow(QWidget *parent)
: QMainWindow( parent ),
  ui_(new Ui::MainWindow),
  tool_manager_(nullptr)
{

    ui_->setupUi( this );
    ui_->terminalDock->hide();


    gl_area_ = new CGlArea(this);
    setCentralWidget(gl_area_);

    tool_manager_ = new ToolManager(gl_area_);

    ModelManager* model_mgr = ModelManager::instance();
    connect( model_mgr, &ModelManager::modelAdded, this, &CGlWindow::modelAddedSlot );
    connect( model_mgr, &ModelManager::modelRemoved, this, &CGlWindow::modelRemovedSlot );

    connect(ui_->actionModelDelete, &QAction::triggered, this, &CGlWindow::onModelDeleteTriggered);
    connect(ui_->actionModelSave, &QAction::triggered, this, &CGlWindow::onModelSaveTriggered);
    connect(ui_->actionModelDriver, &QAction::triggered, this, &CGlWindow::onModelDriverTriggered);

    populateMenus();

    tool_manager_->registerTool(ui_->actionMoveTool, new CameraTool(tool_manager_));
    tool_manager_->registerTool(ui_->actionPick, new PickingTool(tool_manager_));

    //toolGroup->addAction(ui_->actionPick);
    ui_->toolsToolBar->addActions(tool_manager_->getActions());

    connect(tool_manager_, &ToolManager::toolSelected, gl_area_, &CGlArea::onToolSelected);
    tool_manager_->onActionGroupTriggered(ui_->actionMoveTool);

    new TerminalLog(ui_->terminal_text_, ProcessManager::instance());
}

QTreeWidget* CGlWindow::treeWidget() const {
    return ui_->mpCoreTreeView;
}

void CGlWindow::populateMenus(){
    QStringList fileNames = Plugins::instance()->get_plugins();
    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);

        PluginCollection* plugin_collection =  qobject_cast<PluginCollection*>(loader.instance());
        if (plugin_collection == nullptr) continue;

        ActionInterface* action_interface = plugin_collection->getActionPlugin();
        if ( nullptr == action_interface ) continue;

        foreach( auto action, action_interface->getActions(ui_->fileToolBar)){
            if ( action->getType() == PluginAction::IO_ACTION ){
                ui_->fileToolBar->addAction(action->getAction());
            }
        }
    }
}


void
CGlWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Topo"), QDir::currentPath(), tr("all topo files (*.*)") );
    if ( !filename.isEmpty() ) {
        ModelManager* model_mgr = ModelManager::instance();
        FileModelOpenInfo fmoi(filename);
        model_mgr->loadModel( fmoi );
	}
}

void
CGlWindow::on_actionDriversEdit_triggered(){
    PluginsDialog dialog( this );
    dialog.exec();
}


void CGlWindow::modelAddedSlot( Model* pmodel ){
    if ( nullptr != pmodel ){
        connect( pmodel, SIGNAL( modelUpdated(Model*)),
                 gl_area_, SLOT( update( Model* ) ) ) ;

       ModelTreeViewItem* model_item = new ModelTreeViewItem( pmodel, ui_->mpCoreTreeView );
       CTreeViewItem* renderer_item = model_item->defaultRenderer();
       if ( nullptr != renderer_item ){
            renderer_item->setCheckState(0, Qt::Checked);
       }
    }
}


void
CGlWindow::modelRemovedSlot( Model* model ){

    for( int i = 0; i<ui_->mpCoreTreeView->topLevelItemCount(); i++ ){
        ModelTreeViewItem* model_item = dynamic_cast<ModelTreeViewItem*>( ui_->mpCoreTreeView->topLevelItem( i ) );
        if ( nullptr == model_item ){
            continue;
        }
        if ( model_item->model() == model ){

            QListIterator<ModelRenderer*> it_r( model->findChildren<ModelRenderer*>() );
            while( it_r.hasNext() ){
                ModelRenderer* renderer = it_r.next();
                gl_area_->removeCoreRenderer( renderer );
            }

            delete model_item;
        }
    }
}

void 
CGlWindow::on_mpCoreTreeView_itemChanged ( QTreeWidgetItem* item, int )
{
    ModelRendererTreeViewItem* renderer_item = dynamic_cast<ModelRendererTreeViewItem*>( item );
    if ( nullptr != renderer_item ){
        ModelRenderer* renderer = renderer_item->renderer();

        if (  renderer_item->checkState(0) == Qt::Checked  ){
            gl_area_->addCoreRenderer( renderer );
        }
        else {
            gl_area_->removeCoreRenderer( renderer );
        }
	}

}

void
CGlWindow::onModelSaveTriggered( ){

    Model* model = ui_->actionModelSave->data().value<Model*>();

    ModelDriver* driver = model->driver();
    QScopedPointer<ModelOpenInfo> moi(driver->openInfo());

    FileModelOpenInfo* fmoi = dynamic_cast<FileModelOpenInfo*>(moi.get());
    if (fmoi == nullptr){
        qDebug() << "can only save model on file, not for " << driver->description();
        return;
    }

    QString fileType = QString("%1 (*.%2)").arg(driver->description()).arg(fmoi->extension());
    QString filename = QFileDialog::getSaveFileName(this, tr("Save..."), QDir::currentPath(), fileType );
    if ( !filename.isEmpty() ) {
        fmoi->setFilename(filename);
        driver->save( *model, *moi );
    }


}

void
CGlWindow::onModelDeleteTriggered(){

    Model* model = ui_->actionModelDelete->data().value<Model*>();

    ModelManager* model_mgr = ModelManager::instance();
    model_mgr->removeModel( model );

}


void
CGlWindow::onModelDriverTriggered(){

    Model* model = ui_->actionModelDelete->data().value<Model*>();

    DriverDialog dlg(model, this);
    if (dlg.exec() == QDialog::Accepted){
        if ( model->driver() != dlg.selectedDriver() ){
            model->setDriver(dlg.selectedDriver());
        }
    }


}

void 
CGlWindow::handleContextMenuRequest( QPoint point ){

    //contextual menu for renderer
    ModelRendererTreeViewItem* renderer_item = dynamic_cast<ModelRendererTreeViewItem*>( ui_->mpCoreTreeView->itemAt( point ) );
    if ( nullptr != renderer_item ){
        QMenu menu(this);
        RendererMenu::create( &menu, this, renderer_item->renderer() );
        menu.exec( treeWidget()->mapToGlobal(point) );

    }

    //contextual menu for model
    ModelTreeViewItem* model_item = dynamic_cast<ModelTreeViewItem*>( ui_->mpCoreTreeView->itemAt( point ) );
    if ( nullptr != model_item ){
        QMenu menu(this);

        QVariant modelVariant = qVariantFromValue(model_item->model());

        ui_->actionModelDelete->setData(modelVariant);
        menu.addAction( ui_->actionModelDelete );

        ui_->actionModelDriver->setData(modelVariant);
        menu.addAction(ui_->actionModelDriver);

        ui_->actionModelSave->setData(modelVariant);
        menu.addAction( ui_->actionModelSave );

        menu.exec( treeWidget()->mapToGlobal(point) );
    }
}


}

