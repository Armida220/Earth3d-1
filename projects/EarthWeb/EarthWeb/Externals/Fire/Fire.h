/**
	@brief Sky½Ó¿Ú
	@author Hao Luo
	@date 2015/01/24
*/

#ifndef OSGEARTHX_WEB_FIRE_H
#define OSGEARTHX_WEB_FIRE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>
#include <Externals/Fire.h>

#define FireDispatchImpl EarthObjectDispatchImpl< osgEarthX::Fire, Fire, IEarthObject >

FB_FORWARD_PTR(Fire)
class Fire :
	public FireDispatchImpl
{
public:
	Fire( const std::vector<FB::variant>* pArgs = NULL ) :
		FireDispatchImpl(EXTERNAL_FIRE)
	{
		registerProperty("name", make_property(this, &Fire::getName, &Fire::setName));
		registerMethod("setPosition", make_method(this, &Fire::setPosition));
	}

	virtual ~Fire()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();

	//
	void setName(const std::wstring& wstrText);
	std::wstring getName();
	void setPosition(GeoPointPtr ptrGeoPoint);
protected:
private:
};

#endif