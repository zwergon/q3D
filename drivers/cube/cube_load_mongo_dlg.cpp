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
    client_(nullptr),
    i_row_(-1),
    i_col_(-1)
{
     ui_->setupUi(this);
     connect(ui_->button_box, &QDialogButtonBox::accepted,
             this, &CubeLoadMongoDlg::on_button_box_accepted);
     connect(ui_->search_button, &QPushButton::clicked,
             this, &CubeLoadMongoDlg::on_search_button_clicked);

     client_ = moi_.createClient();


}

CubeLoadMongoDlg::~CubeLoadMongoDlg(){
    if (nullptr != client_){
        mongoc_client_destroy(client_);
    }
}


void CubeLoadMongoDlg::clearTable(){
    i_row_ = -1;
    i_col_ = -1;
    while( ui_->results_table->columnCount() > 0){
        ui_->results_table->removeColumn(0);
    }
    ui_->results_table->setRowCount(0);
}

void CubeLoadMongoDlg::populateTable( const QString &) {
    bson_t* query = BCON_NEW();

    mongoc_collection_t *collection =
            mongoc_client_get_collection (client_, moi_.getDatabase().toUtf8(), moi_.getHeaders().toUtf8());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (
                collection,
                query,
                nullptr,  /* additional options */
                nullptr); /* read prefs, NULL for default */
    bson_destroy (query);

    QSet<QString> columns;

    int i_row = 0;

    mongoc_cursor_t* local_cur = mongoc_cursor_clone(cursor);
    const bson_t *doc;
    while (mongoc_cursor_next (local_cur, &doc)) {
        bson_iter_t iter;

        if (bson_iter_init (&iter, doc)) {
            while (bson_iter_next (&iter)) {
                QString key(bson_iter_key (&iter));
                columns.insert(key);
            }
        }
        i_row++;
        //bson_destroy (&doc);
    }
    columns.remove("cube_id");
    columns.remove("cube_type");
    columns.remove("header_type");

    QStringList column_list = columns.toList();
    qSort(column_list);

    for( int i_col=0; i_col<column_list.size(); i_col++){
        if (column_list.at(i_col) == "_id"){
            i_col_ = i_col;
        }
    }
    ui_->results_table->setColumnCount(column_list.size());
    ui_->results_table->setRowCount(i_row);
    ui_->results_table->setHorizontalHeaderLabels(column_list);
    mongoc_cursor_destroy (local_cur);

    i_row = 0;
    local_cur = mongoc_cursor_clone(cursor);
    while (mongoc_cursor_next (local_cur, &doc)) {
        bson_iter_t iter;
        QString cell_str;

        for( int i_col=0; i_col<column_list.size(); i_col++){
            QString key = column_list.at(i_col);
            if ( bson_iter_init (&iter, doc) && bson_iter_find(&iter, key.toUtf8().constData()) )  {
                const bson_value_t* value = bson_iter_value(&iter);
                switch(value->value_type){
                case BSON_TYPE_OID:
                    char str[25];
                    bson_oid_to_string(&value->value.v_oid, str);
                    cell_str = QString(str);
                    break;
                case BSON_TYPE_INT32:
                    cell_str = QString::number(value->value.v_int32);
                    break;
                case BSON_TYPE_INT64:
                    cell_str = QString::number(value->value.v_int64);
                    break;
                case BSON_TYPE_BOOL:
                    cell_str = (value->value.v_bool == true)? "true" : "false";
                    break;
                case BSON_TYPE_DOUBLE:
                    cell_str = QString::number(value->value.v_double);
                    break;
                case BSON_TYPE_UTF8:
                    cell_str = QString(value->value.v_utf8.str);
                    break;
                default:
                    cell_str = "unknown";
                    break;
                }
                ui_->results_table->setItem(i_row, i_col, new QTableWidgetItem(cell_str));
            }
        }
        i_row++;
        //bson_destroy (&doc);
    }
    mongoc_cursor_destroy (local_cur);


    mongoc_cursor_destroy (cursor);  
    mongoc_collection_destroy (collection);
}


void CubeLoadMongoDlg::on_button_box_accepted(){

    auto selected_rows = ui_->results_table->selectionModel()->selectedRows();
    if ( selected_rows.isEmpty()){
        return;
    }
    auto selected_row = selected_rows.at(0);
    i_row_ = selected_row.row();
    auto data = selected_row.data();
    moi_.setId(data.toString());
    ModelManager::instance()->loadModel(moi_);
}

void CubeLoadMongoDlg::on_search_button_clicked( ) {
    QString header;
    clearTable();
    populateTable(header);
}



}
