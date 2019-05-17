#ifndef GEOANALOG_MONGO_DLG_H
#define GEOANALOG_MONGO_DLG_H
#include <q3D/drivers/drivers_global.h>.h>

#include <mongoc.h>

#include <q3D/drivers/cube/mongo_cube_driver.h>

#include <QDialog>

namespace Ui {
class GeoAnalogMongoDlg;
}

namespace Q3D {

class GeoAnalogMongoDlg : public QDialog
{
public:
    GeoAnalogMongoDlg(QWidget* parent = nullptr);
    virtual ~GeoAnalogMongoDlg();

private:
    void listStudies(QStringList& studies) const;
    void listFOVS(const QString& study, QList<double>& fovs) const;
    void listIndices(
            const QString& study,
            int fov,
            QList<int>& indices) const;
    void getHeader( QString& header ) const;

    QString getStudy() const;
    float getFOV() const;
    int getIndex() const;

private slots:
    void on_buttonBox_accepted();
    void on_study_selection_changed(const QString&);
    void on_fov_selection_changed(const QString&);
    void on_index_selection_changed(const QString&);

private:
    Ui::GeoAnalogMongoDlg* ui_;
    GeoAnalogOpenInfo moi_;
    mongoc_client_t* client_;
};

}

#endif // GEOANALOG_MONGO_DLG_H
