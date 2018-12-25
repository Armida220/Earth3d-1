#include <EarthWeb/Basis/Geometry/Polygon/GeoPolygon.h>

bool GeoPolygon::get_visible()
{
	return true;
}

void GeoPolygon::put_visible(bool bVisible)
{
}

bool GeoPolygon::get_enabled()
{
	return true;
}

void GeoPolygon::addPoint(double x, double y, double z)
{
	getOrCreateInnerObject()->push_back(x,y,z);
}

double GeoPolygon::size()
{
	return getOrCreateInnerObject()->size();
}

double GeoPolygon::x(int index)
{
	return getOrCreateInnerObject()->at(index).x();
}

double GeoPolygon::y(int index)
{
	return getOrCreateInnerObject()->at(index).y();
}

double GeoPolygon::z(int index)
{
	return getOrCreateInnerObject()->at(index).z();
}
