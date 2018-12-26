#include <Annotation/ImageOverlay.h>
using namespace osgEarthX;

ImageOverlay::ImageOverlay()
{
	m_ObjectType = IMAGE_OVERLAY;
}

ImageOverlay::~ImageOverlay()
{

}

bool ImageOverlay::isEnabled()
{
	return true;
}

bool ImageOverlay::isVisible()
{
	return getNodeMask();
}

void ImageOverlay::setVisible( bool bVisible )
{
	if (bVisible) {
		setNodeMask(1);
	}
	else {
		setNodeMask(0);
	}
}