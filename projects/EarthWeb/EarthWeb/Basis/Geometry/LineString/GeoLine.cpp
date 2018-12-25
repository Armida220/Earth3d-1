#include <EarthWeb/Basis/Geometry/LineString/GeoLine.h>

bool GeoLine::get_visible()
{
	return true;
}

void GeoLine::put_visible(bool bVisible)
{
}

bool GeoLine::get_enabled()
{
	return true;
}

void GeoLine::addPoint(double x, double y, double z)
{
	getOrCreateInnerObject()->push_back(x,y,z);
}

void GeoLine::popPoint()
{
	getOrCreateInnerObject()->pop_back( );
}

double GeoLine::size()
{
	return getOrCreateInnerObject()->size();
}

double GeoLine::x(int index)
{
	return getOrCreateInnerObject()->at(index).x();
}

double GeoLine::y(int index)
{
	return getOrCreateInnerObject()->at(index).y();
}

double GeoLine::z(int index)
{
	return getOrCreateInnerObject()->at(index).z();
}

void GeoLine::setX(int index, double x)
{
	getOrCreateInnerObject()->at(index).x() = x;
}

void GeoLine::setY(int index, double y)
{
	getOrCreateInnerObject()->at(index).y() = y;
}

void GeoLine::setZ(int index, double z)
{
	getOrCreateInnerObject()->at(index).z() = z;
}
