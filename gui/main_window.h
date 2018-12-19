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
 * $Id: main_window.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_GUI_MAIN_WINDOW_H_
#define Q3D_GUI_MAIN_WINDOW_H_ 1

#include <q3D/gui/gui_global.h>

#include <QString>

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
    class MainWindow;
}

namespace Q3D {

class CGlArea;
class Model;
class ModelRenderer;
class PluginActionInterface;
class ModelManager;


class GUISHARED_EXPORT CGlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CGlWindow( QWidget* parent = nullptr );

	CGlArea* glArea() const;
	QTreeWidget* treeWidget() const;

private slots:
    void modelAddedSlot(Model* model );
    void modelRemovedSlot(Model* model );

    void on_actionOpen_triggered();
    void on_actionDriversEdit_triggered();
    void on_mpCoreTreeView_itemChanged ( QTreeWidgetItem*, int );


	void handleContextMenuRequest( QPoint point );
    void menuSelection( QAction* );

private:
    void populateMenus();

private:
    CGlArea*       gl_area_;
    Ui::MainWindow* ui_;

};

inline CGlArea* CGlWindow::glArea() const {
	return gl_area_;
}




}

#endif /* Q3D_GUI_MAIN_WINDOW_H_ */

