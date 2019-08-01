#include "geoanalog_mongo_dlg.h"
#include "ui_geoanalog_mongo_dlg.h"

#include <QDebug>
#include <q3D/drivers/cube/mongo_cube_driver.h>
#include <q3D/model/model_mgr.h>

#include <q3D/geoanalog/geoanalog_model.h>

struct AggregateResult {
    const char* exam_number;
    const char* title;
    const uint8_t *jpg64;
    double fov;
    int index;
    bool is_3d;
    bool confidential;
    int sheet;

    void init(){
        exam_number = nullptr;
        title = nullptr;
        fov = -1;
        index = -1;
        is_3d = true;
        confidential = true;
        sheet = -1;
        jpg64 = nullptr;
    }

    QString key(){
        return QString("%1_%2").arg(exam_number).arg(fov);
    }

};

void create_aggregate_query( bson_t& query, bool with_match = false ){

    bson_init(&query);

    bson_t *stages = bson_new();
    bson_t* stage;
    int i_stage = 0;
    char i_str[3];

    if ( with_match ){
        //match stage
        stage = BCON_NEW("$match", "{",
                         "is_3d",  BCON_BOOL(true),
                         "}");
        sprintf(i_str, "%d", i_stage);
        bson_append_document(stages, i_str, -1, stage);
        bson_destroy(stage);
        i_stage++;
    }

    //lookup stage
    stage = BCON_NEW("$lookup", "{",
                     "from",  "Exam",
                     "localField",  "Exam number",
                     "foreignField", "Exam number",
                     "as",  "exams",
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;

    //unwind stage
    stage = BCON_NEW("$unwind", "{",
                     "path",  "$exams",
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;

    //lookup stage
    stage = BCON_NEW("$lookup", "{",
                     "from",  "Thumbnail",
                     "localField",  "Exam number",
                     "foreignField", "Exam number",
                     "as",  "thumb",
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;

    //unwind stage
    stage = BCON_NEW("$unwind", "{",
                     "path",  "$thumb",
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;

    //addField stage
    stage = BCON_NEW("$addFields", "{",
                     "Title",  "$exams.Title",
                     "Confidential",  "$exams.Confidential",
                     "Sheet number",  "$exams.Sheet number",
                     "jpg64", "$thumb.jpg64",
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;

    //project stage
    stage = BCON_NEW("$project", "{",
                     "_id",  BCON_INT32(0),
                     "scanner",  BCON_INT32(0),
                     "orig",  BCON_INT32(0),
                     "delta_y",  BCON_INT32(0),
                     "xz_dim",  BCON_INT32(0),
                     "cube_id",  BCON_INT32(0),
                     "cube_type",  BCON_INT32(0),
                     "thumb", BCON_INT32(0),
                     "exams",  BCON_INT32(0),
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;


    bson_append_array(&query, "pipeline", -1, stages);
    bson_destroy(stages);

}

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
    create_aggregate_query(query, true);

    mongoc_collection_t *collection =
            mongoc_client_get_collection (client_,
                                          moi_.getDatabase().toUtf8(),
                                          moi_.getHeaders().toUtf8());
    mongoc_cursor_t *cursor  = mongoc_collection_aggregate (
                collection, MONGOC_QUERY_NONE, &query, NULL, NULL);

    const bson_t *doc;

    QHash<QString, GeoanalogExam*> exams;
    QHash<QString, GeoanalogFOV*> fovs;

    AggregateResult row;
    while (mongoc_cursor_next (cursor, &doc)) {
       bson_iter_t iter;
       row.init();
       if (bson_iter_init (&iter, doc)) {
           while (bson_iter_next(&iter)) {
               uint32_t length;
               const char* key = bson_iter_key(&iter);
               if ( strcmp(key, "Exam number" ) == 0 ){
                   row.exam_number = bson_iter_utf8(&iter, &length);
               }
               else if ( strcmp(key, "fov" ) == 0 ){
                   row.fov = bson_iter_double(&iter);
               }
               else if (strcmp(key, "index" ) == 0 ){
                   row.index = bson_iter_int32(&iter);
               }
               else if ( strcmp(key, "Title" ) == 0 ){
                   row.title = bson_iter_utf8(&iter, &length);
               }
               else if ( strcmp(key, "Confidential") == 0 ){
                   row.confidential = bson_iter_bool(&iter);
               }
               else if ( strcmp(key, "Sheet number") == 0 ){
                   row.sheet = bson_iter_int32(&iter);
               }
               else if ( strcmp(key, "jpg64") == 0 ){
                   bson_subtype_t subtype;
                   uint32_t binary_len;
                   bson_iter_binary(&iter, &subtype, &binary_len, &row.jpg64);
               }
           }
       }
       if ( (row.sheet != -1) &&
            (row.title != nullptr) &&
            (row.exam_number != nullptr)){

           GeoanalogFOV* fov = nullptr;

           if ( !fovs.contains( row.key() ) ){
               GeoanalogExam* exam = nullptr;
               if ( !exams.contains(row.exam_number) ){
                   exam = new GeoanalogExam(
                               geo_collection,
                               row.exam_number,
                               row.title,
                               row.confidential,
                               row.sheet,
                               (const char*)row.jpg64);
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


