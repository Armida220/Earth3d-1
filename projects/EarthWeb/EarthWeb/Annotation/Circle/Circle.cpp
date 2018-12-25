#include <EarthWeb/Annotation/Circle/Circle.h>
#include <atlstr.h>
#include <OSGEarth\GeoMath>

void Circle::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring Circle::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

bool Circle::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void Circle::put_visible(bool bVisible)
{
	getOrCreateInnerObject()->setVisible(bVisible);
}

bool Circle::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void Circle::setPosition(GeoPointPtr ptrGeoPoint)
{
	if (ptrGeoPoint.get() == NULL)
	{
		return;
	}

	getOrCreateInnerObject()->setPosition(*ptrGeoPoint->getOrCreateInnerObject());
}

GeoPointPtr Circle::getPosition()
{
	GeoPointPtr ptrGeoPoint = boost::make_shared<GeoPoint>();
	*(ptrGeoPoint->getOrCreateInnerObject()) = getOrCreateInnerObject()->getPosition();
	return ptrGeoPoint;
}

double Circle::getRadius()
{
	return getOrCreateInnerObject()->getRadius().as(osgEarth::Units::METERS);
}

void Circle::setRadius(double dRadius)
{
	getOrCreateInnerObject()->UpdateRadius(dRadius);
}

void Circle::setRadius2(double dLon, double dLat)
{
	double dRadius;
	osgEarth::GeoPoint pnt = getOrCreateInnerObject()->getPosition();
	dRadius = osgEarth::GeoMath::distance(osg::DegreesToRadians(pnt.y()), osg::DegreesToRadians(pnt.x()), osg::DegreesToRadians(dLat), osg::DegreesToRadians(dLon));
	getOrCreateInnerObject()->UpdateRadius(dRadius);
}

void Circle::setColor(const std::wstring& col)
{
	char *stopstring;
	unsigned int c = strtoul(CStringA(col.data()).GetString(), &stopstring, 16);
	osgEarth::Symbology::Style st = getOrCreateInnerObject()->getStyle();
	st.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(c);
	getOrCreateInnerObject()->setStyle(st);
}


std::wstring Circle::getColor()
{
	return 0;
}
