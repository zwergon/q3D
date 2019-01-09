#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <q3D/gui/gui_global.h>

#include <QObject>
#include <QAction>

namespace Q3D {

class AbstractTool;

class GUISHARED_EXPORT ToolManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolManager(QObject *parent = nullptr);
    virtual ~ToolManager();

    void registerTool( QAction* action, AbstractTool* tool );

    QList<QAction*> getActions() const;

signals:
    void toolSelected(AbstractTool* );

public slots:
    void onActionGroupTriggered(QAction* action);

private:
    QActionGroup* action_group_;
    QMap<QAction*, AbstractTool*> action_tool_map_;
};

}

#endif // TOOL_MANAGER_H
