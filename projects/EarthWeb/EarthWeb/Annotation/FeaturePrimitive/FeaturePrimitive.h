/**
	@brief Model½Ó¿Ú
	@author Jinping	Li
	@date 2017/04/28
*/

#ifndef OSGEARTHX_WEB_FEATUREPRIMITIVE_H
#define OSGEARTHX_WEB_FEATUREPRIMITIVE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

//osgEarthX
#include <Annotation/FeaturePrimitive.h>
#include <EarthWeb/Basis/GeoData/Feature/GeoFeature.h>

#define FeaturePrimitiveDispatchImpl EarthObjectDispatchImpl< osgEarthX::FeaturePrimitive, FeaturePrimitive, IEarthObject >

FB_FORWARD_PTR(FeaturePrimitive)
class FeaturePrimitive :
	public FeaturePrimitiveDispatchImpl
{
public:
	FeaturePrimitive( const std::vector<FB::variant>* pArgs = NULL ) :
		FeaturePrimitiveDispatchImpl( ANNOTATION_FEATURE )
	{
		registerProperty("name", make_property(this, &FeaturePrimitive::getName, &FeaturePrimitive::setName));
		registerProperty("visible", make_property(this, &FeaturePrimitive::get_visible, &FeaturePrimitive::put_visible));
		registerMethod("setFeature", make_method(this, &FeaturePrimitive::setFeature));
		registerMethod("getFeature", make_method(this, &FeaturePrimitive::getFeature));
		registerMethod("updateFeature", make_method(this, &FeaturePrimitive::updateFeature));
	}

	virtual ~FeaturePrimitive()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	void setName(const std::wstring& wstrText);
	std::wstring getName();

	void setFeature(GeoFeaturePtr ptrGeoFeature);
	GeoFeaturePtr getFeature();
	void updateFeature();
protected:
private:
};

#endif