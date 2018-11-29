/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Francois
 *
 * $Id: colormap.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_UTILS_COLORMAP_H_
#define Q3D_UTILS_COLORMAP_H_ 1

#include <q3D/model/model_scope.h>

#include <QObject>
#include <QVector>
#include <QColor>
#include <QHash>
#include <QByteArray>


namespace Q3D {

typedef unsigned char GlColor3uv[3];

class StaticPalettes : public QHash<QString, unsigned char*> {
  public:
    StaticPalettes();
};

class MODELSHARED_EXPORT ColorMap : public QObject
{
    Q_OBJECT
public:
    static const StaticPalettes& palettes();
    static void registerPalette( const QString& name, unsigned char* tab );

    typedef enum {
        NAMED,
        RANDOM,
        INTERPOLATED
    } Type;

public:
    ColorMap( QObject* parent = 0 );

    void initFrom( const ColorMap& cmap );

    ColorMap::Type type() const { return type_; }
    const QString& name() const { return name_; }

    float mini() const { return min_; }
    float maxi() const { return max_; }

	void  setCurrentMin( float min );
	void  setCurrentMax( float max );
    float currentMin() const { return cmin_; }
    float currentMax() const { return cmax_; }

    void  setMinMax ( float min, float max );
    void  getGlColor(float z, GlColor3uv& color) const;

	int            getNColors() const;
	QColor         getQColor( int index ) const;

public:
	void createRandomColormap();
    void createDefaultColormap();
    void createInterpolated( const QColor& c1, const QColor& c2 );
    void createFromName( const QString& name );
    void load( const QString& fileName );

signals:
    void colormapChanged( ColorMap* );

private:
    void createInterpolated( int i_start, int i_end, const QColor& c1, const QColor& c2 );

private:
    Type           type_;
    QString        name_;
    float          min_;
    float          max_;
    float          cmin_;
    float          cmax_ ;
    QVector<unsigned char> colormap_;

    static StaticPalettes palettes_;

};

inline int ColorMap::getNColors() const {
    return colormap_.size()/3;
}

inline QColor ColorMap::getQColor( int index ) const {
    return QColor( colormap_[3*index], colormap_[3*index+1], colormap_[3*index+2] );
}

inline void ColorMap::setCurrentMin( float min ){
    cmin_ = min;
}

inline void ColorMap::setCurrentMax( float max ){
    cmax_ = max;
}

}

#endif
