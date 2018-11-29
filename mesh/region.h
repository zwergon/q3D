#ifndef Q3D_REGION_H
#define Q3D_REGION_H

#include <q3D/mesh/mesh_global.h>

#include <q3D/mesh/mesh.h>


namespace Q3D {


/****************************************************************/
class MESHSHARED_EXPORT RegionInfo
{
public:
    RegionInfo();
    RegionInfo( const QString& name, Element::Type type = Element::CELL );

    void set_name( const QString& name );
    const QString& name() const;

    void set_type( Element::Type type );
    Element::Type type() const;

    bool operator ==(const RegionInfo& ) const;
    bool operator !=(const RegionInfo& ) const;

protected:
    QString name_;
    Element::Type type_;

};

inline void RegionInfo::set_name(const QString &name){
    name_ = name;
}

inline const QString& RegionInfo::name() const {
    return name_;
}

inline Element::Type RegionInfo::type() const {
    return type_;
}

inline void RegionInfo::set_type( Element::Type type ) {
    type_ = type;
}

/****************************************************************/

class MESHSHARED_EXPORT Region :
        public QObject
{
    Q_OBJECT

public:
    typedef enum Polarity_ {
        Unknown,
        Plus,
        Minus
    } Polarity;


public:
    Region();
    virtual ~Region();

    RegionDB* get_region_db() const;
    Mesh* get_mesh() const;

    void set_visible_name(const QString& name);
    const QString& visible_name() const;

    void set_aster_name(const QString& name);
    QString aster_name() const;

    QString aster_name_node() const;
    QString aster_name_cell() const;

    void set_polarity( const Polarity& polarity );
    const Polarity&  polarity() const;

    void get_corners(
            QList<Point3d>& points ) const;

    virtual bool is_empty() const;
    virtual bool contains( corner_id ) const = 0;

    virtual void get_corners(
            CornerSet& corners,
            bool remove_all_first = true ) const = 0;


    void clear_links();
    bool is_linked_to( Region* region );
    void link_with( Region* region );
    const QSet<Region*>& get_linked_regions() const;

protected slots:
    void region_destroyed( QObject* object );



protected:
    QString visible_name_;
    Polarity polarity_;
    QSet<Region*> links_;

};

/**
         * set the name use for CodeAster.
         * names in CodeAster need only 8 characters. Name are build using one char
         * ('N', 'R', 'S', 'T') indicating kind of Region and then an id using a 36 basis.
         */
inline void Region::set_aster_name(const QString& name) {
    setObjectName( name );
}

inline void Region::set_visible_name(const QString& name){
    visible_name_ = name;
}

inline const QString& Region::visible_name() const{
    return visible_name_;
}

/**
     * Get Aster name
     */
inline QString Region::aster_name() const {
    return objectName();
}

/**
     * Get AsterName for this region as a Set of nodes.
     */
inline QString Region::aster_name_node() const {
    return QString("%1NO").arg(aster_name());
}

/**
     * Get AsterName for this region as a Set of cell.
     */
inline QString Region::aster_name_cell() const {
    return QString("%1MA").arg(aster_name());
}



/****************************************************************/

MESHSHARED_EXPORT bool region_less_than_by_aster_name (const Region* r1, const Region* r2 );
MESHSHARED_EXPORT bool region_less_than_by_gocad_name (const Region* r1, const Region* r2 );
MESHSHARED_EXPORT bool region_great_than_by_aster_name(const Region* r1, const Region* r2 );
MESHSHARED_EXPORT bool region_great_than_by_gocad_name(const Region* r1, const Region* r2 );


/***************************************************************/

class MESHSHARED_EXPORT RegionGroup : public Region {
    Q_OBJECT

public:
    RegionGroup();
    virtual ~RegionGroup();

    void add_region( Region* region );
    void add_regions( const QList<Region*> regions );
    void remove_region( Region* region );
    void remove_all_region();

    const QList<Region*>& get_regions() const;

    virtual bool is_empty() const;
    virtual bool contains(corner_id co_id) const;

    virtual void get_corners(
            CornerSet& corners,
            bool remove_all_first = true ) const;

protected slots:
    void region_destroyed( QObject* region );

protected:
    QList<Region*> regions_;

};

inline const QList<Region*>& RegionGroup::get_regions() const {
    return regions_;
}

std::ostream& operator<< (std::ostream &out, Region& mesh);


/***************************************************************/


class MESHSHARED_EXPORT CellRegion : public Region {
    Q_OBJECT

public:
    CellRegion();

    virtual bool is_empty() const;

    bool contains( cell_id cell ) const;

    const CellSet& get_cells() const;

    void get_cells( CellSet& cells,
                    bool remove_all_first = true ) const;

    void set_cells( const CellSet& cells );

    virtual void get_corners(
            CornerSet& corners,
            bool remove_all_first = true ) const;


protected:
    void add_cell(cell_id cell);
    void remove(cell_id cell);


protected:
    CellSet cells_;
};

inline const CellSet& CellRegion::get_cells() const {
    return cells_;
}

std::ostream& operator<< (std::ostream &out, CellRegion& region);


/***************************************************************/

class MESHSHARED_EXPORT NodeRegion : public Region {
    Q_OBJECT

public:
    NodeRegion();
    virtual ~NodeRegion();

    void set_corners( const CornerSet& corners );
    void add_corner( corner_id co_id );
    void clear();

    bool contains(corner_id corner) const;
    virtual void get_corners(
            CornerSet& corners,
            bool remove_all_first = true) const;

protected:
    CornerSet corners_;

};


}

#endif
