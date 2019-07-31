#ifndef GEOANALOG_MODEL_H
#define GEOANALOG_MODEL_H

#include <QAbstractItemModel>
#include <QHash>

#include <q3D/geoanalog/geoanalog_exam.h>

class GeoanalogCollection;

class GeoanalogModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    GeoanalogModel(QObject* parent = nullptr);
    ~GeoanalogModel();

    void setCollection( GeoanalogCollection* collection );

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    GeoanalogCollection* rootItem_;
};

#endif // GEOANALOG_MODEL_H
