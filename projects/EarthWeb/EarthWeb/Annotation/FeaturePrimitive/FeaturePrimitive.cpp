#include <EarthWeb/Annotation/FeaturePrimitive/FeaturePrimitive.h>
#include <atlstr.h>

void FeaturePrimitive::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring FeaturePrimitive::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

bool FeaturePrimitive::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void FeaturePrimitive::put_visible(bool bVisible)
{
	getOrCreateInnerObject()->setVisible(bVisible);
}

bool FeaturePrimitive::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void FeaturePrimitive::setFeature(GeoFeaturePtr ptrGeoFeature)
{
	getOrCreateInnerObject()->setFeature(ptrGeoFeature->getOrCreateInnerObject());
}

GeoFeaturePtr FeaturePrimitive::getFeature()
{
	osgEarth::Features::Feature * pObject = const_cast<osgEarth::Features::Feature *>(getOrCreateInnerObject()->getFeature());
	GeoFeaturePtr ptrFeature = boost::make_shared<  GeoFeature >();
	osgEarthXCore::Feature* ftr = dynamic_cast<osgEarthXCore::Feature*>(pObject);
	if(ftr)
	    ptrFeature->setInnerObject(ftr);
	return ptrFeature;
}

void FeaturePrimitive::updateFeature()
{
	return getOrCreateInnerObject()->update();
}
