#ifndef WIDGET_CREATOR_H
#define WIDGET_CREATOR_H

#include <q3D/plugim/plugim_global.h>

#include <QWidget>
#include <QDomElement>

#include <q3D/model/process_info.h>

namespace Q3D {



/**************************************/
class WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) = 0;
    virtual QString getValue( QWidget* ) const = 0;
};

class BoolBuilder : public WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) override;
    virtual QString getValue( QWidget* ) const override;
};

class DoubleBuilder : public WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) override;
    virtual QString getValue( QWidget* ) const override;
};

/***************************************/
class WidgetCreator
{
public:
    WidgetCreator();
    ~WidgetCreator();

    QWidget* create(const QDomElement& element, QWidget* parent );
    QString getValue(const QDomElement &element, QWidget*) const;

private:
    QHash<QString, WidgetBuilder*> builders_;
};


}

#endif // WIDGET_CREATOR_H
