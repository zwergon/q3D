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
    void listExperiences(QStringList& experiences) const;
    void listNumeros(const QString& experience, QList<int>& numeros) const;
    void listSeries(
            const QString& experience,
            int numero,
            QList<int>& series) const;
    void getHeader( QString& header ) const;

    QString getExperience() const;
    int getNumero() const;
    int getSerie() const;

private slots:
    void on_buttonBox_accepted();
    void on_experience_selection_changed(const QString&);
    void on_numero_selection_changed(const QString&);
    void on_serie_selection_changed(const QString&);

private:
    Ui::CubeLoadMongoDlg* ui_;
    MongoFoamOpenInfo moi_;
    mongoc_client_t* client_;
};

}

#endif // CUBE_LOAD_MONGO_DLG_H
