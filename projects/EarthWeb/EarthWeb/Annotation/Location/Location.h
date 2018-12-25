/**
	@brief Place JS½Ó¿Ú
	@author Hao Luo
	@date 2015/04/02
*/

#ifndef OSGEARTHX_WEB_LOCATION_H
#define OSGEARTHX_WEB_LOCATION_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <Annotation/Location.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>

#define LocationDispatchImpl EarthObjectDispatchImpl< osgEarthX::Location, Location, IEarthObject >

FB_FORWARD_PTR(Location)
class Location : public LocationDispatchImpl
{
public:
	Location( const std::vector<FB::variant>* pArgs = NULL ) : LocationDispatchImpl(EXTERNAL_LOCATION)
	{
		registerProperty("name", make_property(this, &Location::getName, &Location::setName));
		registerMethod( "setPosition", make_method( this, &Location::setPosition ) );
		registerMethod( "getPosition", make_method( this, &Location::getPosition ) );
		registerMethod( "setModelPath", make_method( this, &Location::setModelPath) );
		registerMethod("setModelXML", make_method(this, &Location::setModelXML));
		registerMethod("setRotate", make_method(this, &Location::setRotate));
		registerMethod("showChild", make_method(this, &Location::showChild));
		registerMethod("showChilds", make_method(this, &Location::showChilds));
		registerMethod("setEffect", make_method(this, &Location::setEffect));
		registerMethod("setColor", make_method(this, &Location::setColor));
		registerMethod("removeColor", make_method(this, &Location::removeColor));
		registerMethod("setColorEx", make_method(this, &Location::setColorEx));
		registerMethod("removeColorEx", make_method(this, &Location::removeColorEx));
		registerMethod("getChildPositon", make_method(this, &Location::getChildPositon));
		registerMethod("setColors", make_method(this, &Location::setColors));
		registerMethod("removeColors", make_method(this, &Location::removeColors));
	}

	virtual ~Location(){}

	//impl EarthObject
	void setName(const std::wstring& wstrText);
	std::wstring getName();
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();

	//
	void setPosition( GeoPointPtr ptrGeoPoint );
	GeoPointPtr getPosition();
	void setModelPath( const std::wstring& wstrText );
	void setModelXML(const std::wstring& wstrText);
	void setRotate(double ax, double ay, double az);
	void showChild(const std::wstring& name, bool vis);
	void showChilds(std::vector<const std::wstring> name, bool vis);
	void setEffect(unsigned int uiEffect);
	void setColor(const std::wstring& col);
	void removeColor();
	void setColorEx(const std::wstring& name, const std::wstring& col);
	void removeColorEx(const std::wstring& name);
	void setColors(std::vector<const std::wstring> name, const std::wstring& col);
	void removeColors(std::vector<const std::wstring> name);
	std::vector<double> getChildPositon(const std::wstring& name);
protected:
private:
};

#endif