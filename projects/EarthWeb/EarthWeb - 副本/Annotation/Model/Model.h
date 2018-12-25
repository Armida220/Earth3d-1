/**
	@brief Model½Ó¿Ú
	@author Jinping	Li
	@date 2017/04/28
*/

#ifndef OSGEARTHX_WEB_MODEL_H
#define OSGEARTHX_WEB_MODEL_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>

//osgEarthX
#include <Annotation/Model.h>

#define ModelDispatchImpl EarthObjectDispatchImpl< osgEarthX::Model, Model, IEarthObject >

FB_FORWARD_PTR(Model)
class Model :
	public ModelDispatchImpl
{
public:
	Model( const std::vector<FB::variant>* pArgs = NULL ) :
		ModelDispatchImpl( ANNOTATION_MODEL )
	{
		registerMethod("setModelPath",make_method(this,&Model::setModelPath));
		registerMethod("setPosition",make_method(this,&Model::setPosition));
	}

	virtual ~Model()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	//impl EarthObject
	void setModelPath( const std::wstring& strPath );
	void setPosition(GeoPointPtr ptrGeoPoint);

protected:
private:
};

#endif