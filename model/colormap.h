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

typedef quint8 Color3ub[3];
typedef quint8 Color4ub[4];

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
    ColorMap( QObject* parent = nullptr );

    void initFrom( const ColorMap& cmap );

    ColorMap::Type type() const { return type_; }
    const QString& name() const { return name_; }

    double mini() const { return min_; }
    double maxi() const { return max_; }

    void  setCurrentMin( double min );
    void  setCurrentMax( double max );
    double currentMin() const { return cmin_; }
    double currentMax() const { return cmax_; }


    bool hasAlpha() const;

    void setAlphaIdx( int alpha_idx );
    int  alphaIdx() const;

    void  setMinMax ( double min, double max );
    void  getGlColor(double z, Color3ub& color) const;
    void  getGlColor(double z, Color4ub& color) const;

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
    double          min_;
    double          max_;
    double          cmin_;
    double          cmax_ ;
    bool            has_alpha_;
    int             alpha_idx_;
    QVector<unsigned char> colormap_;

    static StaticPalettes palettes_;

};

inline int ColorMap::getNColors() const {
    return colormap_.size()/3;
}

inline QColor ColorMap::getQColor( int index ) const {
    return QColor( colormap_[3*index], colormap_[3*index+1], colormap_[3*index+2] );
}

inline void ColorMap::setCurrentMin( double min ){
    cmin_ = min;
}

inline void ColorMap::setCurrentMax( double max ){
    cmax_ = max;
}

inline bool ColorMap::hasAlpha() const {
    return has_alpha_;
}

inline void ColorMap::setAlphaIdx(int alpha_idx){
    if ( alpha_idx == -1 ) {
        has_alpha_ = false;
    }
    else {
        alpha_idx_ = alpha_idx;
        has_alpha_ = true;
    }
}

inline int ColorMap::alphaIdx() const {
    return alpha_idx_;
}

}

#endif
