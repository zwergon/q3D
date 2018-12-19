#include "cube_load_mongo_dlg.h"
#include "ui_cube_load_mongo_dlg.h"

namespace Q3D {

CubeLoadMongoDlg::CubeLoadMongoDlg(QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::CubeLoadMongoDlg)
{
     ui_->setupUi(this);
}

}
