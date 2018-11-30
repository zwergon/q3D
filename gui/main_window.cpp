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

#include <QString>
#include <QLayout>
#include <QFileDialog>


#include <q3D/model/model.h>
#include <q3D/model/model_driver.h>
#include <q3D/model/renderer.h>
#include <q3D/model/models_data.h>

#include <q3D/plugins/plugin_dialog.h>
#include <q3D/plugins/plugins.h>
#include <q3D/plugins/plugin_action.h>
#include <q3D/plugins/interfaces.h>

#include <q3D/gui/gl_area.h>
#include <q3D/gui/main_window.h>
#include <q3D/gui/renderer_menu.h>
#include <q3D/gui/camera_tool.h>

Q_DECLARE_METATYPE( Q3D::Model* );

namespace Q3D {

/*********************************************************/



class CTreeViewItem : public QTreeWidgetItem {
public:
    CTreeViewItem(QTreeWidget* parent) : QTreeWidgetItem(parent){}
    CTreeViewItem(QTreeWidgetItem* parentItem) : QTreeWidgetItem(parentItem){}
};

/*********************************************************/

class ModelTreeViewItem : public CTreeViewItem {
public:
    ModelTreeViewItem( Model* model, QTreeWidget* parent );

    Model* model() const { return model_; }

private:
    Model* model_;
};

/*********************************************************/

class ModelRendererTreeViewItem : public CTreeViewItem {
public:
    ModelRendererTreeViewItem( ModelTreeViewItem*, const QString& renderer_name );
    ~ModelRendererTreeViewItem() {
        delete renderer_;
    }


    ModelRenderer* renderer(){
        if ( 0 == renderer_ ){
            ModelDriver* driver = model()->driver();
            renderer_ = driver->createRenderer( model(), renderer_name_ );
        }
        return renderer_;
    }

    Model* model() const {
        return static_cast<ModelTreeViewItem*>(parent())->model();
    }

private:
    QString renderer_name_;
    ModelRenderer* renderer_;

};


ModelRendererTreeViewItem::ModelRendererTreeViewItem( ModelTreeViewItem* itemParent,
                                                      const QString& renderer_name  )
    :	CTreeViewItem(itemParent),
      renderer_name_(renderer_name),
      renderer_(0)
{
    setText( 0, renderer_name );
    setFlags( Qt::ItemIsUserCheckable|Qt::ItemIsEnabled );
    setCheckState( 0, Qt::Unchecked );
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


/*********************************************************/

CGlWindow::CGlWindow( ModelsData* data, QWidget *parent )
: QMainWindow( parent ),
  data_( data )
{

	ui_.setupUi( this );

    gl_area_ = ui_.gl_area_;

    connect( data_, SIGNAL(modelAdded(Model*)), this, SLOT( modelAddedSlot(Model*) ) );
    connect( data_, SIGNAL(modelRemoved(Model*)), this, SLOT( modelRemovedSlot(Model*) ) );

    //populateMenus();

    AbstractTool* tool = new CameraTool();
    tool->setGlArea(gl_area_);
    gl_area_->setActiveTool( tool );

}


void CGlWindow::populateMenus(){
    ui_.menuTools->clear();
    QStringList fileNames = Plugins::instance()->get_plugins();
    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);
        QObject *plugin = loader.instance();
        if (plugin){
            PluginActionInterface *iTool = qobject_cast<PluginActionInterface *>(plugin);
            if (iTool){
                addToolsMenu( iTool );
            }
        }
    }
}

void CGlWindow::addToolsMenu( PluginActionInterface* iTool ){
    foreach( QString action_name, iTool->tools() ){
        PluginAction* plugin_action = iTool->action( action_name );
        QAction* q_action = new QAction(action_name, plugin_action);
        connect( q_action, SIGNAL(triggered()), this, SLOT(pluginActionTriggered() ) );
        ui_.menuTools->addAction(q_action);
    }
}

void CGlWindow::pluginActionTriggered(){
    QAction *action = qobject_cast<QAction *>(sender());
    PluginAction *p_action =
                qobject_cast<PluginAction *>(action->parent());
    p_action->execute( data_ );
}

void
CGlWindow::on_actionOpen_triggered()
{
	QString newfilename = QFileDialog::getOpenFileName(this,
		tr("Open Topo"), QDir::currentPath(), tr("all topo files (*.*)") );
	if ( !newfilename.isEmpty() ) {
        data_->loadModel( newfilename );
	}
}

void
CGlWindow::on_actionDriversEdit_triggered(){
    PluginsDialog dialog( this );
    dialog.exec();
}


void CGlWindow::modelAddedSlot( Model* pmodel ){
    if ( 0 != pmodel ){
        connect( pmodel, SIGNAL( modelUpdated(Model*)),
                 gl_area_, SLOT( update( Model* ) ) ) ;

        new ModelTreeViewItem( pmodel, ui_.mpCoreTreeView );
    }
}


void
CGlWindow::modelRemovedSlot( Model* model ){

    for( int i = 0; i<ui_.mpCoreTreeView->topLevelItemCount(); i++ ){
        ModelTreeViewItem* model_item = dynamic_cast<ModelTreeViewItem*>( ui_.mpCoreTreeView->topLevelItem( i ) );
        if ( 0 == model_item ){
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
    if ( 0 != renderer_item ){
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
CGlWindow::menuSelection( QAction* action ){
    if ( 0 == action ){
        return;
    }

    Model* model = action->data().value<Model*>();
    if ( action->text() == "Delete" ){
        data_->removeModel( model );
    }
    else if ( action->text() == "Save" ){
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Topo"), QDir::currentPath(), "*.gmsh" );
        if ( !filename.isEmpty() ) {
            model->driver()->save( *model, filename );
        }

    }

}

void 
CGlWindow::handleContextMenuRequest( QPoint point ){

    //contextual menu for renderer
    ModelRendererTreeViewItem* renderer_item = dynamic_cast<ModelRendererTreeViewItem*>( ui_.mpCoreTreeView->itemAt( point ) );
    if ( 0 != renderer_item ){
        QMenu menu(this);
        RendererMenu::create( &menu, this, renderer_item->renderer() );
        menu.exec( treeWidget()->mapToGlobal(point) );

    }

    //contextual menu for model
    ModelTreeViewItem* model_item = dynamic_cast<ModelTreeViewItem*>( ui_.mpCoreTreeView->itemAt( point ) );
    if ( 0 != model_item ){
        QMenu menu(this);

        QAction* action = new QAction("Delete", this);
        action->setData( qVariantFromValue(model_item->model()));
        menu.addAction( action );

        action = new QAction("Save", this);
        action->setData( qVariantFromValue(model_item->model()));
        menu.addAction( action );

        connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSelection(QAction*)));
        menu.exec( treeWidget()->mapToGlobal(point) );
    }
}


}

