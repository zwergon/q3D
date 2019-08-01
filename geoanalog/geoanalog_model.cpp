#include "geoanalog_model.h"

#include <QDebug>
#include <QColor>

/*************************************************/


GeoanalogModel::GeoanalogModel(QObject* parent) : QAbstractItemModel(parent)
{
    rootItem_ = new GeoanalogCollection();
}

GeoanalogModel::~GeoanalogModel(){
    delete rootItem_;
}

void GeoanalogModel::setCollection(GeoanalogCollection *collection){
    if ( nullptr != rootItem_ ){
        delete rootItem_;
    }
    rootItem_ = collection;
}


QVariant GeoanalogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    GeoanalogItem* item = static_cast<GeoanalogItem*>(index.internalPointer());
    const GeoanalogExam* exam = item->getGeoanalogExam();
    if ( exam != nullptr ){
        if (role == Qt::BackgroundRole){
            if  ( (index.column() > 0 ) ){
                if (exam->row() % 2 == 0 ) {
                    return QVariant(QColor(220,220,240));
                }
                else {
                    return QVariant(QColor(240,240,240));
                }
            }
            return QVariant();
        }

        if ( role == Qt::ToolTipRole){
            if ( exam->getIcon() != nullptr ){
                return QVariant(QString("<img src='data:image/jpeg;base64, %0'/>").arg(exam->getIcon()));
            }
            return QVariant();
        }
    }

    if (role != Qt::DisplayRole)
        return QVariant();


    if ( nullptr != item ){
        return item->data(index.column());
    }

    return QVariant();
}

Qt::ItemFlags GeoanalogModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
            return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant GeoanalogModel:: headerData(
        int section,
        Qt::Orientation orientation,
        int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case GeoanalogItem::Column::EXAM_NUMBER:
                return QString("Name");
            case GeoanalogItem::Column::TITLE:
                return QString("Title");
            case GeoanalogItem::Column::IS_3D:
                return QString("Is3D");
            case GeoanalogItem::Column::CONFIDENTIAL:
                return QString("Confidential");
            case GeoanalogItem::Column::SHEET:
                return QString("Sheet");
            }
        }
    }
    return QVariant();
}

QModelIndex GeoanalogModel::index(
        int row,
        int column,
        const QModelIndex &parent ) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    GeoanalogItem *parentItem;

    if (!parent.isValid()){
        parentItem = rootItem_;
    }
    else {
         parentItem = static_cast<GeoanalogItem*>(parent.internalPointer());
    }

    GeoanalogItem *childItem = parentItem->child(row);
    if (childItem != nullptr)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex GeoanalogModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
          return QModelIndex();

      GeoanalogItem *childItem = static_cast<GeoanalogItem*>(index.internalPointer());
      GeoanalogItem *parentItem = childItem->parent();

      if (parentItem == rootItem_)
          return QModelIndex();

      return createIndex(parentItem->row(), 0, parentItem);
}

int GeoanalogModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
           return 0;

    GeoanalogItem* parentItem;
    if (!parent.isValid())
        parentItem = rootItem_;
    else
        parentItem = static_cast<GeoanalogItem*>(parent.internalPointer());

    return parentItem->rowCount();

}

int GeoanalogModel::columnCount(const QModelIndex& ) const {
    return GeoanalogItem::Column::LAST;
}


