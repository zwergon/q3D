#ifndef GEOANALOG_MONGO_DLG_H
#define GEOANALOG_MONGO_DLG_H

#include <q3D/geoanalog/geoanalog_global.h>

#include <mongoc.h>

#include <q3D/geoanalog/geoanalog_driver.h>
#include <q3D/geoanalog/geoanalog_exam.h>

#include <QDialog>

namespace Ui {
class GeoAnalogMongoDlg;
}

class GeoAnalogMongoDlg : public QDialog
{
public:
    GeoAnalogMongoDlg(QWidget* parent = nullptr);
    virtual ~GeoAnalogMongoDlg();

private:
    void getExams( GeoanalogCollection* geo_collection ) const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GeoAnalogMongoDlg* ui_;
    GeoAnalogOpenInfo moi_;
    mongoc_client_t* client_;
};

#endif // GEOANALOG_MONGO_DLG_H
