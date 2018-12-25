/**
	@brief Model½Ó¿Ú
	@author Jinping	Li
	@date 2017/04/28
*/

#ifndef OSGEARTHX_WEB_CIRCLE_H
#define OSGEARTHX_WEB_CIRCLE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

//osgEarthX
#include <Annotation/Circle.h>
//#include <EarthWeb/Basis/GeoData/Feature/GeoFeature.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>

#define CircleDispatchImpl EarthObjectDispatchImpl< osgEarthX::Circle, Circle, IEarthObject >

FB_FORWARD_PTR(Circle)
class Circle :
	public CircleDispatchImpl
{
public:
	Circle( const std::vector<FB::variant>* pArgs = NULL ) :
		CircleDispatchImpl( ANNOTATION_CIRCLE )
	{
		registerProperty("name", make_property(this, &Circle::getName, &Circle::setName));
		registerProperty("visible", make_property(this, &Circle::get_visible, &Circle::put_visible));
		registerProperty("color", make_property(this, &Circle::getColor, &Circle::setColor));
		registerMethod("setPosition", make_method(this, &Circle::setPosition));
		registerMethod("getPosition", make_method(this, &Circle::getPosition));
		registerMethod("setRadius", make_method(this, &Circle::setRadius));
		registerMethod("setRadius2", make_method(this, &Circle::setRadius2));
		registerMethod("getRadius", make_method(this, &Circle::getRadius));
	}

	virtual ~Circle()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	void setColor(const std::wstring& col);
	std::wstring getColor();

	void setName(const std::wstring& wstrText);
	std::wstring getName();

	void setPosition(GeoPointPtr ptrGeoPoint);
	GeoPointPtr getPosition();

	double getRadius();
	void setRadius(double dRadius);
	void setRadius2(double dLon,double dLat);
protected:
private:
};

#endif