#include "cube_load_mongo_dlg.h"
#include "ui_cube_load_mongo_dlg.h"

#include <QDebug>
#include <q3D/drivers/cube/mongo_cube_driver.h>
#include <q3D/model/model_mgr.h>

namespace Q3D {

CubeLoadMongoDlg::CubeLoadMongoDlg(QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::CubeLoadMongoDlg),
    moi_(),
    client_(nullptr)
{
     ui_->setupUi(this);
     connect(ui_->buttonBox, &QDialogButtonBox::accepted,
             this, &CubeLoadMongoDlg::on_buttonBox_accepted);
     connect(ui_->experience_cb_, &QComboBox::currentTextChanged,
             this, &CubeLoadMongoDlg::on_experience_selection_changed);
     connect(ui_->numero_cb_, &QComboBox::currentTextChanged,
             this, &CubeLoadMongoDlg::on_numero_selection_changed);
     connect(ui_->serie_cb_, &QComboBox::currentTextChanged,
             this, &CubeLoadMongoDlg::on_serie_selection_changed);

     client_ = moi_.createClient();
     QStringList experiences;
     listExperiences(experiences);
     ui_->experience_cb_->addItems(experiences);
     ui_->experience_cb_->setCurrentIndex(0);

}

CubeLoadMongoDlg::~CubeLoadMongoDlg(){
    if (nullptr != client_){
        mongoc_client_destroy(client_);
    }
}

QString CubeLoadMongoDlg::getExperience() const {
    return ui_->experience_cb_->currentText();
}

int CubeLoadMongoDlg::getNumero() const {
    return ui_->numero_cb_->currentText().toInt();
}

int CubeLoadMongoDlg::getSerie() const {
    return ui_->serie_cb_->currentText().toInt();
}

void CubeLoadMongoDlg::getHeader(QString &header) const {
    bson_t* query = BCON_NEW(
                "experience", BCON_UTF8(getExperience().toUtf8()),
                "numero", BCON_INT32(getNumero()),
                "serie", BCON_INT32(getSerie()));

    mongoc_collection_t *collection =
            mongoc_client_get_collection (client_, moi_.getDatabase().toUtf8(), "headers");
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (
                collection,
                query,
                nullptr,  /* additional options */
                nullptr); /* read prefs, NULL for default */
    bson_destroy (query);

    const bson_t *doc;
    while (mongoc_cursor_next (cursor, &doc)) {
        char* str = bson_as_json (doc, NULL);
        header += QString(str);
        bson_free (str);
    }

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
}

void CubeLoadMongoDlg::listExperiences(QStringList &experiences) const{
    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("experience"));

    bson_t reply;
    bson_error_t error;
    mongoc_client_command_simple(
                client_, moi_.getDatabase().toUtf8(),
                command, nullptr,
                &reply, &error);

    bson_iter_t iter;
    bson_iter_t array_iter;
    if ( (bson_iter_init_find (&iter, &reply, "values") &&
          BSON_ITER_HOLDS_ARRAY (&iter) &&
          bson_iter_recurse (&iter, &array_iter))) {
        while (bson_iter_next (&array_iter)) {
            if (BSON_ITER_HOLDS_UTF8 (&array_iter)) {
                uint32_t length = 0;
                auto val = bson_iter_utf8(&array_iter, &length);
                experiences.append(val);
            }
        }
    }

    experiences.sort();

    bson_destroy (command);
    bson_destroy (&reply);

}

void CubeLoadMongoDlg::listNumeros(const QString &experience, QList<int> &numeros) const {

    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("numero"),
                "query", "{", "experience", BCON_UTF8(experience.toUtf8()), "}");

    bson_t reply;
    bson_error_t error;
    mongoc_client_command_simple(
                client_, moi_.getDatabase().toUtf8(),
                command, nullptr,
                &reply, &error);

    bson_iter_t iter;
    bson_iter_t array_iter;
    if ( (bson_iter_init_find (&iter, &reply, "values") &&
          BSON_ITER_HOLDS_ARRAY (&iter) &&
          bson_iter_recurse (&iter, &array_iter))) {
        while (bson_iter_next (&array_iter)) {
            if (BSON_ITER_HOLDS_INT32 (&array_iter)) {
                auto val = bson_iter_int32(&array_iter);
                numeros.append(val);
            }
        }
    }

    qSort(numeros);

    bson_destroy (command);
    bson_destroy (&reply);

}

void CubeLoadMongoDlg::listSeries(
        const QString &experience,
        int numero,
        QList<int> &series) const {

    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("serie"),
                "query", "{",
                    "experience", BCON_UTF8(experience.toUtf8()),
                    "numero", BCON_INT32(numero),
                "}");

    bson_t reply;
    bson_error_t error;
    mongoc_client_command_simple(
                client_, moi_.getDatabase().toUtf8(),
                command, nullptr,
                &reply, &error);

    bson_iter_t iter;
    bson_iter_t array_iter;
    if ( (bson_iter_init_find (&iter, &reply, "values") &&
          BSON_ITER_HOLDS_ARRAY (&iter) &&
          bson_iter_recurse (&iter, &array_iter))) {
        while (bson_iter_next (&array_iter)) {
            if (BSON_ITER_HOLDS_INT32 (&array_iter)) {
                auto val = bson_iter_int32(&array_iter);
                series.append(val);
            }
        }
    }

    qSort(series);

    bson_destroy (command);
    bson_destroy (&reply);

}

void CubeLoadMongoDlg::on_buttonBox_accepted(){
    moi_.setExperience(getExperience());
    moi_.setNumero(getNumero());
    moi_.setSerie(getSerie());
    ModelManager::instance()->loadModel(moi_);
}

void CubeLoadMongoDlg::on_experience_selection_changed(const QString& experience){
    QList<int> numeros;
    listNumeros(experience, numeros);
    ui_->numero_cb_->blockSignals(true);
    ui_->numero_cb_->clear();
    for( auto n : numeros ){
        ui_->numero_cb_->addItem(QString::number(n));
    }
    ui_->numero_cb_->blockSignals(false);
    if ( !numeros.isEmpty()){
        on_numero_selection_changed(QString::number(numeros.at(0)));
    }
}

void CubeLoadMongoDlg::on_numero_selection_changed(const QString& numero_str){
    QList<int> series;
    listSeries(getExperience(), numero_str.toInt(), series);
    ui_->serie_cb_->blockSignals(true);
    ui_->serie_cb_->clear();
    for( auto n : series ){
        ui_->serie_cb_->addItem(QString::number(n));
    }
    ui_->serie_cb_->blockSignals(false);
    if ( !series.isEmpty() ){
        on_serie_selection_changed(QString::number(series.at(0)));
    }
}

void CubeLoadMongoDlg::on_serie_selection_changed(const QString& ) {
    QString header;
    ui_->header_text_->clear();
    getHeader(header);
    ui_->header_text_->setText(header);
}



}
