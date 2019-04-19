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

#include <q3D/model/model_scope.h>

#include <QString>

namespace Q3D {

class MODELSHARED_EXPORT ModelOpenInfo
{
public:
    virtual ~ModelOpenInfo(){}
};


class MODELSHARED_EXPORT FileModelOpenInfo : public ModelOpenInfo {
public:
    FileModelOpenInfo():filename_(QString::null) {}
    FileModelOpenInfo(const QString& pfilename) : filename_(pfilename) {}
    const QString& fileName() const;
    void setFilename( const QString& filename );

    void setExtension( const QString& extension );
    const QString& extension() const;

protected:
    QString filename_;
    QString extension_;

};

inline const QString& FileModelOpenInfo::fileName() const {
	return filename_;
}

inline void FileModelOpenInfo::setFilename(const QString &filename){
    filename_ = filename;
}

inline void FileModelOpenInfo::setExtension(const QString &extension){
    extension_ = extension;
}


inline const QString& FileModelOpenInfo::extension() const {
    return extension_;
}

}

#endif /* Q3D_MODEL_MODEL_OPEN_INFO_H_ */

