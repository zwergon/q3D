#ifndef GEOANALOG_EXAM_H
#define GEOANALOG_EXAM_H

#include <QObject>
#include <QVariant>
#include <QVector>

class GeoanalogItem : public QObject  {
    Q_OBJECT
public:
    enum Column {
       EXAM_NUMBER = 0,
       TITLE,
       IS_3D,
       CONFIDENTIAL,
       SHEET,
       LAST
    };

public:
    int rowCount() const;

    int row() const;
    GeoanalogItem* parent() const;
    GeoanalogItem* child( int row ) const;
    virtual QVariant data(int column ) const = 0;


protected:
    explicit GeoanalogItem(QObject* parent);
};

/*************************************************/
class GeoanalogCollection : public GeoanalogItem {
    Q_OBJECT
public:
    GeoanalogCollection();
    virtual QVariant data(int ) const override;

};

/*************************************************/
class GeoanalogExam : public GeoanalogItem {
    Q_OBJECT

public:
    GeoanalogExam(
            GeoanalogItem* parent,
            const QString& exam_number,
            const QString& title,
            bool confidential,
            int sheet );

    QString getExamNumber() const;
    virtual QVariant data(int column) const override;

private:
    QVector<QVariant> data_;
};

/*************************************************/
class GeoanalogFOV : public GeoanalogItem {
    Q_OBJECT
public:
    explicit GeoanalogFOV(GeoanalogExam* exam, double fov);

    GeoanalogExam* getGeoanalogExam() const;
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
    GeoanalogExam* getGeoanalogExam() const;

    virtual QVariant data(int column) const override;

    int getIndex() const;
    double getFov() const;
    QString getExamNumber() const;

private:
    int index_;
    bool is3D_;
};

#endif // GEOANALOG_EXAM_H
