#include "geoanalog_aggregate.h"


void GeoanalogAggregate::create_query( bson_t& query, bool with_match){

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
                     "localField",  "exam_number",
                     "foreignField", "exam_number",
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
                     "localField",  "exam_number",
                     "foreignField", "exam_number",
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
                     "sheet_number",  "$exams.sheet_number",
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

    //sort stage
    stage = BCON_NEW("$sort", "{",
                     "exam_number",  BCON_INT32(1),
                     "}");
    sprintf(i_str, "%d", i_stage);
    bson_append_document(stages, i_str, -1, stage);
    bson_destroy(stage);
    i_stage++;


    bson_append_array(&query, "pipeline", -1, stages);
    bson_destroy(stages);

}

GeoanalogAggregate::GeoanalogAggregate() :
    exam_number(nullptr),
    title(nullptr),
    fov(-1),
    index(-1),
    is_3d(true),
    confidential(true),
    sheet(-1),
    jpg64(nullptr)
{
}

QString GeoanalogAggregate::key() const {
    return QString("%1_%2").arg(exam_number).arg(fov);
}

void GeoanalogAggregate::parse( const bson_t* doc ){
    bson_iter_t iter;
    if (bson_iter_init (&iter, doc)) {
        while (bson_iter_next(&iter)) {
            uint32_t length;
            const char* key = bson_iter_key(&iter);
            if ( strcmp(key, "exam_number" ) == 0 ){
                exam_number = bson_iter_utf8(&iter, &length);
            }
            else if ( strcmp(key, "fov" ) == 0 ){
                fov = bson_iter_double(&iter);
            }
            else if (strcmp(key, "index" ) == 0 ){
                index = bson_iter_int32(&iter);
            }
            else if ( strcmp(key, "Title" ) == 0 ){
                title = bson_iter_utf8(&iter, &length);
            }
            else if ( strcmp(key, "Confidential") == 0 ){
                confidential = bson_iter_bool(&iter);
            }
            else if ( strcmp(key, "sheet_number") == 0 ){
                sheet = bson_iter_int32(&iter);
            }
            else if ( strcmp(key, "jpg64") == 0 ){
                bson_subtype_t subtype;
                uint32_t binary_len;
                bson_iter_binary(&iter, &subtype, &binary_len, &jpg64);
            }
        }
    }
}

