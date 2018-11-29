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
 * $Id: model_open_info.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_MODEL_MODEL_OPEN_INFO_H_
#define Q3D_MODEL_MODEL_OPEN_INFO_H_ 1

#include <QString>

namespace Q3D {

using namespace std;

class ModelOpenInfo
{
public:
	ModelOpenInfo( const QString& pfilename) : filename_(pfilename) {}
	const QString& fileName() const;

protected: 
	QString filename_;
};

inline const QString& ModelOpenInfo::fileName() const {
	return filename_;
}

}

#endif /* Q3D_MODEL_MODEL_OPEN_INFO_H_ */

