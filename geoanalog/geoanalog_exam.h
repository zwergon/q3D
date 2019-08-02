#ifndef GEOANALOG_EXAM_H
#define GEOANALOG_EXAM_H

#include <QObject>
#include <QVariant>
#include <QVector>

#include <q3D/geoanalog/geoanalog_aggregate.h>

class GeoanalogExam;

/**
 * @brief Tree Item structure for GeoanalogModel.
 * Tree with nodes from up to bottom : GeoanalogCollection/GeoanalogExam/GeoanalogFOV/GeoanalogCube
 */

class GeoanalogItem : public QObject  {
    Q_OBJECT
public:
    enum Column {
       EXAM_NUMBER = 0,
       SHEET,
       TITLE,
       CONFIDENTIAL,
       IS_3D,
       LAST
    };

public:
    int rowCount() const;
    int row() const;
    GeoanalogItem* parent() const;
    GeoanalogItem* child( int row ) const;


    virtual QVariant data(int column ) const = 0;
    virtual const GeoanalogExam* getGeoanalogExam() const = 0;


protected:
    explicit GeoanalogItem(QObject* parent);
};

/*************************************************/
class GeoanalogCollection : public GeoanalogItem {
    Q_OBJECT
public:
    GeoanalogCollection();
    virtual QVariant data(int ) const override;

    virtual const GeoanalogExam* getGeoanalogExam() const override;

};

/*************************************************/
class GeoanalogExam : public GeoanalogItem {
    Q_OBJECT

public:
    GeoanalogExam(
            GeoanalogItem* parent,
            const GeoanalogAggregate& row );

    QString getExamNumber() const;

    const QString& getIcon() const;

    virtual QVariant data(int column) const override;
    virtual const GeoanalogExam* getGeoanalogExam() const override;


private:
    QVector<QVariant> data_;
    QString jpg64_;
};

/*************************************************/
class GeoanalogFOV : public GeoanalogItem {
    Q_OBJECT
public:
    explicit GeoanalogFOV(GeoanalogExam* exam, double fov);

    virtual const GeoanalogExam* getGeoanalogExam() const override;
    virtual QVariant data(int column) const override;

    QString getExamNumber() const;
    double getFov() const;

private:
    double fov_;
};

/*************************************************/
class GeoanalogCube : public GeoanalogItem {
    Q_OBJECT
public:
    explicit GeoanalogCube(GeoanalogFOV* fov, int index, bool is3D);

    GeoanalogFOV* getGeoanalogFOV() const;

    virtual const GeoanalogExam* getGeoanalogExam() const override;
    virtual QVariant data(int column) const override;

    int getIndex() const;
    double getFov() const;
    QString getExamNumber() const;

private:
    int index_;
    bool is3D_;
};

#endif // GEOANALOG_EXAM_H
