#include <Annotation/Place.h>

using namespace osgEarthX;

Place::Place()
{
	m_ObjectType = PLACE;
	m_bVisible = true;
}

Place::~Place()
{

}

bool Place::isEnabled()
{
	return true;
}

bool Place::isVisible()
{
	return m_bVisible;
}

void Place::setVisible( bool bVisible )
{
	if (m_bVisible == bVisible) return;
	m_bVisible = bVisible;
	if (bVisible) {
		setNodeMask(1);
	}
	else {
		setNodeMask(0);
	}
}


