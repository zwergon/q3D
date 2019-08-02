#ifndef GEOANALOGAGGREGATE_H
#define GEOANALOGAGGREGATE_H

#include <mongoc.h>
#include <QString>

/**
 * @brief The GeoanalogAggregate class extracts from MongoDB database a collection
 * that sumarrize informations required by QAbstractItemModel.
 *
 * aggregate that builds a collection with at least the following columns
 *
 * | exam_number | title | fov | index | is_3D | confidential | sheed | jpg64  |
 */

class GeoanalogAggregate
{
public:
    static void create_query( bson_t& query, bool with_match = false );

public:
    const char* exam_number; // primary key
    const char* title;
    const uint8_t *jpg64; //base 64 encoded string for exam thumbnail
    double fov;
    int index;
    bool is_3d;
    bool confidential;
    int sheet;

public:
    GeoanalogAggregate();
    void parse( const bson_t* doc );

    bool isValid() const;

    QString key() const;
};

inline bool GeoanalogAggregate::isValid() const {
    return (sheet != -1) && (title != nullptr) &&  (exam_number != nullptr);
}

#endif // GEOANALOGAGGREGATE_H
