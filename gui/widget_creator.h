#ifndef WIDGET_CREATOR_H
#define WIDGET_CREATOR_H

#include <q3D/gui/gui_global.h>

#include <QWidget>
#include <QDomElement>

#include <q3D/model/process_info.h>

namespace Q3D {

/**************************************/
class GUISHARED_EXPORT WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) = 0;
    virtual QString getValue( QWidget* ) const = 0;
};

class GUISHARED_EXPORT BoolBuilder : public WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) override;
    virtual QString getValue( QWidget* ) const override;
};

class GUISHARED_EXPORT IntegerBuilder : public WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) override;
    virtual QString getValue( QWidget* ) const override;
};


class GUISHARED_EXPORT DoubleBuilder : public WidgetBuilder {
public:
    virtual QWidget* create( const QDomElement& element, QWidget* parent ) override;
    virtual QString getValue( QWidget* ) const override;
};

/***************************************/
class GUISHARED_EXPORT WidgetCreator
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
