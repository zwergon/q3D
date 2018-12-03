
#ifndef Q3D_MODELFACTORY_HPP_
#define Q3D_MODELFACTORY_HPP_ 1

#include <QString>
#include <QMap>
#include <QList>

#include <q3D/model/model_scope.h>

using namespace std;

template<typename BaseClassType, typename ClassType>
BaseClassType *createObject()
{
	return new ClassType();
}

template<typename BaseClassType, typename UniqueIdType>
class ObjectFactory
{
protected:
	typedef BaseClassType *(*createObjectFunc)();

public:
	typedef typename QMap<UniqueIdType, createObjectFunc>::iterator Iterator;

	template<typename ClassType>
	bool registerFactory(UniqueIdType unique_id)
	{
		if (object_creator_.contains(unique_id) )
			return false;

		object_creator_[unique_id] = &createObject<BaseClassType, ClassType>;

		return true;
	}

	BaseClassType* create(UniqueIdType unique_id) {

		if ( object_creator_.contains(unique_id) ){
			return object_creator_[unique_id]();
		}

		return 0;
	}

	BaseClassType* createDefault()
	{
		Iterator iter = object_creator_.begin();

		if (iter == object_creator_.end())
			return 0;

		return ( iter.value()() );
	}

	int count() const{
		return object_creator_.size();
	}

    void keys( QList<UniqueIdType>& keys ) const{
		keys =  object_creator_.keys();
	}

protected:
	QMap<UniqueIdType, createObjectFunc> object_creator_;
};

#endif /* Q3D_MODELFACTORY_HPP_ */
