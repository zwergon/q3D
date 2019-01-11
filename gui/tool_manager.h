#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <q3D/gui/gui_global.h>

#include <q3D/gui/gl_area.h>
#include <QObject>
#include <QAction>

namespace Q3D {

class AbstractTool;

class GUISHARED_EXPORT ToolManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolManager(CGlArea* area);

    void registerTool( QAction* action, AbstractTool* tool );

    QList<QAction*> getActions() const;

    CGlArea* getGlArea() const;

signals:
    void toolSelected(AbstractTool* );

public slots:
    void onActionGroupTriggered(QAction* action);

private:
    QActionGroup* action_group_;
    QMap<QAction*, AbstractTool*> action_tool_map_;
};

inline CGlArea* ToolManager::getGlArea() const {
    return static_cast<CGlArea*>(parent());
}

}

#endif // TOOL_MANAGER_H
