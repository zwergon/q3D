#ifndef CUBE_LOAD_MONGO_DLG_H
#define CUBE_LOAD_MONGO_DLG_H

#include <q3D/drivers/drivers_global.h>.h>

#include <mongoc.h>

#include <q3D/drivers/cube/mongo_cube_driver.h>

#include <QDialog>

namespace Ui {
class CubeLoadMongoDlg;
}

namespace Q3D {

class CubeLoadMongoDlg : public QDialog
{
public:
    CubeLoadMongoDlg(QWidget* parent = nullptr);
    virtual ~CubeLoadMongoDlg();

private:
    void clearTable();
    void populateTable( const QString& request );


private slots:
    void on_button_box_accepted();
    void on_search_button_clicked();



private:
    Ui::CubeLoadMongoDlg* ui_;
    MongoIdOpenInfo moi_;
    mongoc_client_t* client_;
    int i_col_;
    int i_row_;

};

}

#endif // CUBE_LOAD_MONGO_DLG_H
