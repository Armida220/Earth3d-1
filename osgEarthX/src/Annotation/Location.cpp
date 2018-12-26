#include <Annotation/Location.h>

using namespace osgEarthX;

Location::Location()
{
	m_ObjectType = LOCATION;
	m_bVisible = true;
}

Location::~Location()
{

}

bool Location::isEnabled()
{
	return true;
}

bool Location::isVisible()
{
	return m_bVisible;
}

void Location::setVisible( bool bVisible )
{	
	if (m_bVisible == bVisible) return;
	m_bVisible = bVisible;

	showNode(bVisible);

	/*if (bVisible) {
		setNodeMask(1);
	}
	else {
		setNodeMask(0);
	}*/
}


