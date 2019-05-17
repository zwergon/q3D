#include "geoanalog_mongo_dlg.h"
#include "ui_geoanalog_mongo_dlg.h"

#include <QDebug>
#include <q3D/drivers/cube/mongo_cube_driver.h>
#include <q3D/model/model_mgr.h>

namespace Q3D {

GeoAnalogMongoDlg::GeoAnalogMongoDlg(QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::GeoAnalogMongoDlg),
    moi_(),
    client_(nullptr)
{
     ui_->setupUi(this);
     connect(ui_->buttonBox, &QDialogButtonBox::accepted,
             this, &GeoAnalogMongoDlg::on_buttonBox_accepted);
     connect(ui_->study_cb_, &QComboBox::currentTextChanged,
             this, &GeoAnalogMongoDlg::on_study_selection_changed);
     connect(ui_->fov_cb_, &QComboBox::currentTextChanged,
             this, &GeoAnalogMongoDlg::on_fov_selection_changed);
     connect(ui_->index_cb_, &QComboBox::currentTextChanged,
             this, &GeoAnalogMongoDlg::on_index_selection_changed);

     client_ = moi_.createClient();
     QStringList studies;
     listStudies(studies);
     ui_->study_cb_->addItems(studies);
     ui_->study_cb_->setCurrentIndex(0);

}

GeoAnalogMongoDlg::~GeoAnalogMongoDlg(){
    if (nullptr != client_){
        mongoc_client_destroy(client_);
    }
}

QString GeoAnalogMongoDlg::getStudy() const {
    return ui_->study_cb_->currentText();
}

float GeoAnalogMongoDlg::getFOV() const {
    return ui_->fov_cb_->currentText().toFloat();
}

int GeoAnalogMongoDlg::getIndex() const {
    return ui_->index_cb_->currentText().toInt();
}

void GeoAnalogMongoDlg::getHeader(QString &header) const {
    bson_t* query = BCON_NEW(
                "study", BCON_UTF8(getStudy().toUtf8()),
                "fov", BCON_DOUBLE(getFOV()),
                "index", BCON_INT32(getIndex()));

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

void GeoAnalogMongoDlg::listStudies(QStringList &studies) const{
    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("study"));

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
                studies.append(val);
            }
        }
    }

    studies.sort();

    bson_destroy (command);
    bson_destroy (&reply);

}

void GeoAnalogMongoDlg::listFOVS(const QString &study, QList<double> &fovs) const {

    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("fov"),
                "query", "{", "study", BCON_UTF8(study.toUtf8()), "}");

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
            if (BSON_ITER_HOLDS_DOUBLE (&array_iter)) {
                auto val = bson_iter_double(&array_iter);
                fovs.append(val);
            }
        }
    }

    qSort(fovs);

    bson_destroy (command);
    bson_destroy (&reply);

}

void GeoAnalogMongoDlg::listIndices(
        const QString &study,
        int fov,
        QList<int> &indices) const {

    bson_t* command = BCON_NEW(
                "distinct", BCON_UTF8("headers"),
                "key", BCON_UTF8("index"),
                "query", "{",
                    "study", BCON_UTF8(study.toUtf8()),
                    "fov", BCON_DOUBLE(fov),
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
                indices.append(val);
            }
        }
    }

    qSort(indices);

    bson_destroy (command);
    bson_destroy (&reply);

}

void GeoAnalogMongoDlg::on_buttonBox_accepted(){
    moi_.setStudy(getStudy());
    moi_.setFov(getFOV());
    moi_.setIndex(getIndex());
    ModelManager::instance()->loadModel(moi_);
}

void GeoAnalogMongoDlg::on_study_selection_changed(const QString& study){
    QList<double> fovs;
    listFOVS(study, fovs);
    ui_->fov_cb_->blockSignals(true);
    ui_->fov_cb_->clear();
    for( auto n : fovs ){
        ui_->fov_cb_->addItem(QString::number(n));
    }
    ui_->fov_cb_->blockSignals(false);
    if ( !fovs.isEmpty()){
        on_fov_selection_changed(QString::number(fovs.at(0)));
    }
}

void GeoAnalogMongoDlg::on_fov_selection_changed(const QString& fov_str){
    QList<int> indices;
    listIndices(getStudy(), fov_str.toDouble(), indices);
    ui_->index_cb_->blockSignals(true);
    ui_->index_cb_->clear();
    for( auto n : indices ){
        ui_->index_cb_->addItem(QString::number(n));
    }
    ui_->index_cb_->blockSignals(false);
    if ( !indices.isEmpty() ){
        on_index_selection_changed(QString::number(indices.at(0)));
    }
}

void GeoAnalogMongoDlg::on_index_selection_changed(const QString& ) {
    QString header;
    ui_->header_text_->clear();
    getHeader(header);
    ui_->header_text_->setText(header);
}



}
