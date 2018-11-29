#include <q3D/mesh/region.h>

#include <q3D/mesh/region_db.h>

/***************************************************************/

uint qHash(const Q3D::PropertyInfo& p){
    return ::qHash( QPair<QString, int>(p.name(), (int)p.type() ) );
}


namespace Q3D {

/**********************************************************/

RegionInfo::RegionInfo()
    : name_(QString::null)
{
}

RegionInfo::RegionInfo( const QString& name, Element::Type type )
    : name_( name),
      type_( type )
{
}

bool RegionInfo::operator==( const RegionInfo& p ) const {
    return ( p.name_ == name_ ) && ( p.type_ == type_ );
}

bool RegionInfo::operator!=( const RegionInfo& p ) const {
    return ( p.name_ != name_ ) ||  ( p.type_ != type_ );
}

/**********************************************************/

bool region_less_than_by_aster_name (
        const Region* r1,
        const Region* r2 )
{
    return r1->aster_name() < r2->aster_name();
}

bool region_less_than_by_gocad_name (
        const Region* r1,
        const Region* r2 )
{
    return r1->visible_name() < r2->visible_name();
}

bool region_great_than_by_aster_name (
        const Region* r1,
        const Region* r2 )
{
    return !region_less_than_by_aster_name(r1, r2);
}

bool region_great_than_by_gocad_name (
        const Region* r1,
        const Region* r2 )
{
    return !region_less_than_by_gocad_name(r1, r2);
}

/**
    * \class Region
    * @ingroup topology
    *
    * Defines base class for all Region that can be handled by Q3D.
    */

/**
     * \fn virtual void Region::get_corners( CornerSet& corners, bool remove_all_first = true) const
     * Retrieves all corner_id for all nodes in this Region
     */

/**
     * Constructor
     * @param parent the QObject this Region belongs. Can be a AsterSimulation, a RegionsDefinition
     * @param dim 0 for NodeRegion, 1 for OneDRegion, 2 for TwoDRegion, 3 for CellRegion
     */
Region::Region() :
    QObject(),
    visible_name_(QString::null),
    polarity_(Unknown)
{
}

/**
     * destructor
     */
Region::~Region() {
}

RegionDB* Region::get_region_db() const {
    return static_cast<RegionDB*>(parent());
}

Mesh* Region::get_mesh() const {
    RegionDB* region_db = get_region_db();
    if (0 != region_db ){
        return region_db->get_mesh();
    }

    return 0;
}

void Region::set_polarity( const Polarity& polarity ){
    polarity_ = polarity;
}

/**
     * Get the polarity of this Region
     */
const Region::Polarity& Region::polarity() const {
    return polarity_;
}



bool Region::is_empty() const {
    CornerSet corners;
    get_corners( corners );
    return corners.isEmpty();
}

/**
     * Retrieves all Point3d of this Region
     * @see virtual void get_corners(CornerSet& corners, bool remove_all_first) const
     */
void Region::get_corners(
        QList<Point3d>& points ) const
{

    Mesh* mesh = get_mesh();
    if ( 0 == mesh ){
        return;
    }
    CornerSet corners;
    get_corners( corners );

    QSetIterator<corner_id> it( corners );
    while( it.hasNext() ){
        Point3d point;
        mesh->get_point( it.next(), point );
        points.append( point );
    }
}

void Region::clear_links(){
    links_.clear();
}


/**
     * return true if region is linked to this
     */
bool Region::is_linked_to( Region* region ){
    return links_.contains( region );
}

/**
     * link the Region region with this Region. Association is done from this
     * to region, and from region to this. polarity of regions are dual ( if this is plus,
     * linked region is minus )
     */
void Region::link_with( Region* region ){
    if ( is_linked_to( region ) ){
        return;
    }
    links_.insert( region );
    connect( region, SIGNAL( destroyed(QObject*) ),
             this, SLOT( region_destroyed(QObject*) ) );
    region->link_with( this );
    if ( region->polarity() == Minus ){
        set_polarity( Plus );
    }
    else {
        set_polarity( Minus );
    }
}

/**
     * Get the Set of Region this Region is linked with
     */
const QSet<Region*>& Region::get_linked_regions() const {
    return links_;
}

void Region::region_destroyed( QObject* object ) {
    links_.remove( reinterpret_cast<Region*>( object ) );
}

std::ostream& operator<< (std::ostream &out, Region& region)
{
    CornerSet corners;
    region.get_corners( corners );

    out << "Region: n_corners:" << corners.count() << std::endl;

    QSetIterator<corner_id> it_co( corners );
    while( it_co.hasNext() ){
        cell_id co_id = it_co.next();
        out << co_id << std::endl;
   }

    return out;
}



/***************************************************************/

/**
    * \class CellRegion
    * @ingroup topology
    *
    * Implements one specific Region that is a collection of cell.
    */


CellRegion::CellRegion() : Region()
{
}


void CellRegion::get_corners(
        CornerSet& corners,
        bool remove_all_first ) const {

    if(remove_all_first) {
        corners.clear();
    }

    Mesh* mesh = get_mesh();
    if ( 0 == mesh ){
        return;
    }

    corners.reserve( corners.size() + cells_.size()*4 );
    QSetIterator<cell_id> iter(cells_);
    while( iter.hasNext() ){
        cell_id ce_id = iter.next();
        const ICell& cell = mesh->get_cell( ce_id );

        for( int i=0; i<cell.count(); i++ ){
            corners.insert( cell[i] );
        }
    }

}

bool CellRegion::is_empty() const {
    return cells_.isEmpty();
}

bool CellRegion::contains(cell_id cell) const {
    return cells_.contains(cell);
}

void CellRegion::get_cells(
        CellSet& cells,
        bool remove_all_first ) const {

    if ( remove_all_first ){
        cells.clear();
    }

    cells |= cells_;

}

/**
     * set the cell_id set of this CellRegion
     */
void CellRegion::set_cells( const CellSet& cells ) {
    cells_ = cells;
}

/**
     * remove one cell_id to this CellRegion
     */
void CellRegion::remove(cell_id cell) {
    cells_.remove(cell);
}

/**
     * add one cell_id to this CellRegion
     */
void CellRegion::add_cell(cell_id cell) {
    cells_.insert(cell);
}

std::ostream& operator<< (std::ostream &out, CellRegion& region)
{
    CellSet cells;
    region.get_cells( cells );

    out << "CellRegion: n_cells:" << cells.count() << std::endl;

    QSetIterator<cell_id> it_ce( cells );
    while( it_ce.hasNext() ){
        cell_id ce_id = it_ce.next();
        out << ce_id << std::endl;

    }

    out << (Region&)region;

    return out;
}




/***************************************************************/
/**
    * \class RegionGroup
    * @ingroup topology
    *
    * Implements one specific TwoDRegion that is a collection of Region.
    * uses the Composite Pattern.
    */

RegionGroup::RegionGroup() : Region()
{
}


RegionGroup::~RegionGroup(){
    QMutableListIterator<Region*> itr( regions_ );
    while( itr.hasNext() ){
        Region* region = itr.next();
        disconnect( region, SIGNAL( destroyed( QObject* ) ),
                    this, SLOT( region_destroyed( QObject* ) ) );
        itr.remove();
    }
}

void RegionGroup::add_regions( const QList<Region*> regions ){
    QListIterator<Region*> itr( regions );
    while( itr.hasNext() ){
        add_region( itr.next()  );
    }
}

void RegionGroup::add_region( Region* region ){
    if ( 0 == region ){
        return;
    }
    if ( !regions_.contains( region ) ){
        regions_.append( region );
        connect( region, SIGNAL( destroyed( QObject* ) ),
                 this, SLOT( region_destroyed( QObject* ) ) );
    }
}

void RegionGroup::remove_region( Region* region ){
    if ( regions_.contains( region ) ){
        regions_.removeAll( region );
        disconnect( region, SIGNAL( destroyed( QObject* ) ),
                    this, SLOT( region_destroyed( QObject* ) ) );
    }
}

void RegionGroup::remove_all_region(){
    QMutableListIterator<Region*> itr( regions_ );
    while( itr.hasNext() ){
        Region* region = itr.next();
        disconnect( region, SIGNAL( destroyed( QObject* ) ),
                    this, SLOT( region_destroyed( QObject* ) ) );
        itr.remove();
    }
}

/**
    * return true if the RegionGroup region contains the face referenced by this face_id
    */
bool RegionGroup::contains( face_id face ) const {
    QListIterator<Region*> itr( regions_ );
    while( itr.hasNext() ){
        Region* region = itr.next();
        if ( region->contains( face ) ) {
            return true;
        }
    }
    return false;
}


void RegionGroup::get_corners(
        CornerSet& corners,
        bool remove_all_first) const {

    if(remove_all_first) {
        corners.clear();
    }
    QListIterator<Region*> itr( regions_ );
    while( itr.hasNext() ){
        Region* region = itr.next();
        region->get_corners(corners, false );
    }
}

bool RegionGroup::is_empty() const {

    QListIterator<Region*> itr( regions_ );
    while( itr.hasNext() ){
        Region* region = itr.next();
        if ( !region->is_empty() ) {
            return false ;
        }
    }
    return true;
}

void RegionGroup::region_destroyed( QObject* region ){
    remove_region( reinterpret_cast<Region*>( region ) );
}


/***************************************************************/
/**
    * \class NodeRegion
    * @ingroup topology
    *
    * Implements one specific Region that is a collection of node ( corner_id ).
    *
    */


NodeRegion::NodeRegion() : Region()
{
}

NodeRegion::~NodeRegion() {
}


bool NodeRegion::contains(corner_id corner) const {
    return corners_.contains(corner );
}


void NodeRegion::get_corners(
        CornerSet& corners,
        bool remove_all_first ) const {

    if (remove_all_first) {
        corners.clear();
    }

    Mesh* mesh = get_mesh();
    if ( 0 == mesh ){
        return;
    }

    corners |= corners_;

}

void NodeRegion::add_corner( corner_id co_id ){
    corners_.insert( co_id );
}

/**
     * set the corner_id set of this Region
     */
void NodeRegion::set_corners( const CornerSet& corners ){
    corners_ = corners;
}

void NodeRegion::clear(){
    corners_.clear();
}





} // namespace Q3D
