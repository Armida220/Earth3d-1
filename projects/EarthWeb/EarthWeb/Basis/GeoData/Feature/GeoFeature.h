/**
	@brief osgEarth::GeoPoint½Ó¿Ú
	@author Hao Luo
	@date 2015/04/13
*/

#ifndef OSGEARTHX_WEB_GEO_FEATURE_H
#define OSGEARTHX_WEB_GEO_FEATURE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <Basis/Geo/Feature.h>
#include <EarthWeb/Basis/GeoData/GeoClassType.h>
#include <EarthWeb/Basis/Geometry/LineString/GeoLine.h>
#include <EarthWeb/Basis/Geometry/Polygon/GeoPolygon.h>

#define GeoFeatureDispatchImpl EarthObjectDispatchImpl< osgEarthXCore::Feature, GeoFeature, IEarthObject >

FB_FORWARD_PTR(GeoFeature)
class GeoFeature : public GeoFeatureDispatchImpl
{
public:
	GeoFeature( const std::vector<FB::variant>* pArgs = NULL ) : GeoFeatureDispatchImpl(GEODATA_FEATURE)
	{
		registerProperty("color", make_property(this, &GeoFeature::getColor, &GeoFeature::setColor));
		registerProperty("fillColor", make_property(this, &GeoFeature::getFillcolor, &GeoFeature::setFillcolor));
		registerProperty("width", make_property(this, &GeoFeature::getWidth, &GeoFeature::setWidth));
		registerProperty("clamp", make_property(this, &GeoFeature::getClamp, &GeoFeature::setClamp));
		registerProperty("pattern", make_property(this, &GeoFeature::getPattern, &GeoFeature::setPattern));

		registerMethod("setGeometry", make_method(this, &GeoFeature::setGeometry));
		registerMethod("getGeometry", make_method(this, &GeoFeature::getGeometry));
	}
	virtual ~GeoFeature(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();
	void setColor(const std::wstring& col);
	std::wstring getColor();
	void setFillcolor(const std::wstring& col);
	std::wstring getFillcolor();
	void setWidth(float w);
	float getWidth();
	void setClamp(bool bClamp);
	bool getClamp();
	void setPattern(unsigned short pattern);
	unsigned short getPattern();
	void setGeometry(IEarthObjectPtr ptrGeometry);
	IEarthObjectPtr getGeometry();
protected:
private:
	unsigned short _pattern;
};

#endif