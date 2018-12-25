#include <EarthWeb/Basis/GeoData/Feature/GeoFeature.h>
#include <osgEarthSymbology/Geometry>
#include <atlstr.h>

bool GeoFeature::get_visible()
{
	return true;
}

void GeoFeature::put_visible(bool bVisible)
{
}

bool GeoFeature::get_enabled()
{
	return true;
}

void GeoFeature::setColor(const std::wstring& col)
{
	char *stopstring;
	unsigned int c = strtoul(CStringA(col.data()).GetString(), &stopstring, 16);
	getOrCreateInnerObject()->setColor(c);
}


std::wstring GeoFeature::getColor()
{
	return 0;
}

void GeoFeature::setFillcolor(const std::wstring& col)
{
	char *stopstring;
	unsigned int c = strtoul(CStringA(col.data()).GetString(), &stopstring, 16);

	getOrCreateInnerObject()->setFillcolor(c);
}


std::wstring GeoFeature::getFillcolor()
{
	return 0;
}

void GeoFeature::setWidth(float w)
{
	getOrCreateInnerObject()->setWidth(w);
}


float GeoFeature::getWidth()
{
	return 0.0;
}

void GeoFeature::setClamp(bool bClamp)
{
	getOrCreateInnerObject()->setClamp(bClamp);
}


bool GeoFeature::getClamp()
{
	return false;
}

void GeoFeature::setPattern(unsigned short pattern)
{
	_pattern = pattern;
	getOrCreateInnerObject()->setPattern(pattern);
}

unsigned short GeoFeature::getPattern()
{
	return _pattern;
}

void GeoFeature::setGeometry(IEarthObjectPtr ptrGeometry)
{
	if (ptrGeometry.get() == NULL)
	{
		return;
	}

	GeoLine* ptrGeoLine = dynamic_cast<GeoLine*>(ptrGeometry.get());
	if(ptrGeoLine)
		getOrCreateInnerObject()->setGeometry(ptrGeoLine->getOrCreateInnerObject());
	else {
		GeoPolygon* ptrGeoPoly = dynamic_cast<GeoPolygon*>(ptrGeometry.get());
		if (ptrGeoPoly) {
			getOrCreateInnerObject()->setGeometry(ptrGeoPoly->getOrCreateInnerObject());
		}
	}
}

IEarthObjectPtr GeoFeature::getGeometry()
{	
	osgEarth::Symbology::LineString* lin = static_cast<osgEarth::Symbology::LineString*>(getOrCreateInnerObject()->getGeometry());
	if (lin) {
		GeoLinePtr ptrGeoLine = boost::make_shared<GeoLine>();
		IEarthObjectPtr ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(ptrGeoLine);
		ptrGeoLine->setInnerObject(lin);
		return ptrJSAPI;
	}
	else {
		osgEarth::Symbology::Polygon* poly = static_cast<osgEarth::Symbology::Polygon*>(getOrCreateInnerObject()->getGeometry());
		if (poly) {
			GeoPolygonPtr ptrGeoPoly = boost::make_shared<GeoPolygon>();
			IEarthObjectPtr ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(ptrGeoPoly);
			ptrGeoPoly->setInnerObject(poly);
			return ptrJSAPI;
		}
	}
}
