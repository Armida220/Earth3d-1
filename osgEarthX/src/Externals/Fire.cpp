#include <Externals/Fire.h>
using namespace osgEarthX;

Fire::Fire()
{
	m_ObjectType = FIRE;
}

Fire::~Fire()
{

}

bool Fire::isEnabled()
{
	return true;
}

bool Fire::isVisible()
{
	return true;
}

void Fire::setVisible(bool bVisible)
{

}