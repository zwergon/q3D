#include "geoanalog_mongo_dlg.h"
#include "ui_geoanalog_mongo_dlg.h"

#include <QDebug>
#include <q3D/drivers/cube/mongo_cube_driver.h>
#include <q3D/model/model_mgr.h>

#include <q3D/geoanalog/geoanalog_model.h>
#include <q3D/geoanalog/geoanalog_aggregate.h>

GeoAnalogMongoDlg::GeoAnalogMongoDlg(QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::GeoAnalogMongoDlg),
    moi_(),
    client_(nullptr)
{
     ui_->setupUi(this);
     connect(ui_->buttonBox, &QDialogButtonBox::accepted,
             this, &GeoAnalogMongoDlg::on_buttonBox_accepted);

     client_ = moi_.createClient();

     GeoanalogCollection* geo_collection = new GeoanalogCollection();
     getExams(geo_collection);

     GeoanalogModel* model = new GeoanalogModel(this);
     model->setCollection(geo_collection);

     ui_->examTreeView->setModel(model);
}

GeoAnalogMongoDlg::~GeoAnalogMongoDlg(){
    if (nullptr != client_){
        mongoc_client_destroy(client_);
    }
}

void GeoAnalogMongoDlg::getExams( GeoanalogCollection* geo_collection ) const {
    bson_t query;
    GeoanalogAggregate::create_query(query, true);

    mongoc_collection_t *collection =
            mongoc_client_get_collection (client_,
                                          moi_.getDatabase().toUtf8(),
                                          moi_.getHeaders().toUtf8());
    mongoc_cursor_t *cursor  = mongoc_collection_aggregate (
                collection, MONGOC_QUERY_NONE, &query, NULL, NULL);


    QHash<QString, GeoanalogExam*> exams;
    QHash<QString, GeoanalogFOV*> fovs;

    const bson_t *doc;
    while (mongoc_cursor_next (cursor, &doc)) {

        GeoanalogAggregate row;
        row.parse(doc);

        if ( row.isValid() ){

            GeoanalogFOV* fov = nullptr;

            if ( !fovs.contains( row.key() ) ){
                GeoanalogExam* exam = nullptr;
                if ( !exams.contains(row.exam_number) ){
                    exam = new GeoanalogExam(geo_collection, row);
                    exams.insert(row.exam_number, exam);
                }
                else {
                    exam = exams[row.exam_number];
                }

                fov = new GeoanalogFOV(exam, row.fov);
                fovs.insert(row.key(), fov);
            }
            else {
                fov = fovs[row.key()];
            }

            new GeoanalogCube(fov, row.index, row.is_3d);
        }
    }

    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
}

void GeoAnalogMongoDlg::on_buttonBox_accepted(){

    QModelIndex index = ui_->examTreeView->currentIndex();
    if ( index.isValid() ){
       GeoanalogItem* item = static_cast<GeoanalogItem*>(index.internalPointer());
       GeoanalogCube* cube = dynamic_cast<GeoanalogCube*>(item);
       if ( nullptr != cube ){
           moi_.setIndex(cube->getIndex());
           moi_.setFov(cube->getFov());
           moi_.setStudy(cube->getExamNumber());
           Q3D::ModelManager::instance()->loadModel(moi_);
       }
    }
}


