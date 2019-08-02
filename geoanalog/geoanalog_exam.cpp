#include "geoanalog_exam.h"

/*************************************************/
GeoanalogItem::GeoanalogItem(QObject* parent) : QObject(parent){
}

int GeoanalogItem::rowCount() const {
    return children().size();
}

GeoanalogItem* GeoanalogItem::child( int row ) const {
    QObjectList children_ = children();
    if ( children_.at(row) != nullptr)
        return static_cast<GeoanalogItem*>(children_.at(row));
    return nullptr;
}

GeoanalogItem* GeoanalogItem::parent() const {
    QObject* parent_ = QObject::parent();
    if ( parent_ != nullptr )
        return static_cast<GeoanalogItem*>(parent_);

    return nullptr;
}

int GeoanalogItem::row() const {
    if ( parent() != nullptr ){
        return parent()->children().indexOf( const_cast<GeoanalogItem*>(this));
    }
    return 0;
}

/*************************************************/
GeoanalogCollection::GeoanalogCollection(): GeoanalogItem(nullptr){}
QVariant GeoanalogCollection::data(int ) const{
    return QVariant();
}

const GeoanalogExam* GeoanalogCollection::getGeoanalogExam() const {
    return static_cast<GeoanalogExam*>(child(0));
}


/*************************************************/

GeoanalogExam::GeoanalogExam(
        GeoanalogItem* root,
        const GeoanalogAggregate& row ):
    GeoanalogItem(root),
    data_(LAST)
{
    data_[EXAM_NUMBER] = QVariant(row.exam_number);
    data_[TITLE] = QVariant(row.title);
    data_[CONFIDENTIAL] = QVariant(row.confidential);
    data_[SHEET] = QVariant(row.sheet);
    jpg64_ = QString("%0").arg((const char*)row.jpg64);
}

const QString& GeoanalogExam::getIcon() const {
    return jpg64_;
}

QString GeoanalogExam::getExamNumber() const {
    return data_[EXAM_NUMBER].toString();
}

QVariant GeoanalogExam::data(int column) const{
    if (column < 0 || column >= LAST)
        return QVariant();
    return data_.at(column);
}

const GeoanalogExam* GeoanalogExam::getGeoanalogExam() const {
    return this;
}

/*************************************************/

GeoanalogFOV::GeoanalogFOV(GeoanalogExam* exam, double fov) : GeoanalogItem(exam), fov_(fov){
}

QVariant GeoanalogFOV::data(int column) const{
   switch(column){
   case EXAM_NUMBER:
       return QVariant(fov_);
   case SHEET:
       return QVariant( QString("%1 Cubes").arg(children().size()));
   default:
       break;
   }

   return QVariant();
}

const GeoanalogExam* GeoanalogFOV::getGeoanalogExam() const {
    return static_cast<GeoanalogExam*>(parent());
}

QString GeoanalogFOV::getExamNumber() const {
    return getGeoanalogExam()->getExamNumber();

}
double GeoanalogFOV::getFov() const{
    return fov_;
}

/*************************************************/

GeoanalogCube::GeoanalogCube(GeoanalogFOV* fov, int index, bool is3D): GeoanalogItem(fov), index_(index), is3D_(is3D){
}

QVariant GeoanalogCube::data(int column) const{
   switch(column){
   case EXAM_NUMBER:
       return QVariant(index_);
   case IS_3D:
       return QVariant(is3D_);
   default:
       break;
   }

   return QVariant();
}

GeoanalogFOV* GeoanalogCube::getGeoanalogFOV() const {
    return static_cast<GeoanalogFOV*>(parent());
}

const GeoanalogExam* GeoanalogCube::getGeoanalogExam() const {
    return getGeoanalogFOV()->getGeoanalogExam();
}

QString GeoanalogCube::getExamNumber() const {
    return getGeoanalogExam()->getExamNumber();
}

double GeoanalogCube::getFov() const{
    return getGeoanalogFOV()->getFov();
}

int GeoanalogCube::getIndex() const {
    return index_;
}
