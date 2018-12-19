#ifndef CUBE_LOAD_MONGO_DLG_H
#define CUBE_LOAD_MONGO_DLG_H

#include <q3D/cube/cube_global.h>

#include <QDialog>

namespace Ui {
class CubeLoadMongoDlg;
}

namespace Q3D {

class CubeLoadMongoDlg : public QDialog
{
public:
    CubeLoadMongoDlg(QWidget* parent = nullptr);

private:
    Ui::CubeLoadMongoDlg* ui_;
};

}

#endif // CUBE_LOAD_MONGO_DLG_H
