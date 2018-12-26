#include <Provider/AnnotationProvider/ImageOverlayProvider.h>
using namespace osgEarthXCore;

ImageOverlayProvider::ImageOverlayProvider()
{
}

ImageOverlayProvider::~ImageOverlayProvider()
{

}

void ImageOverlayProvider::_setImage( osg::Image* pImage )
{
	osgEarth::Annotation::ImageOverlay::setImage( pImage );
}

void ImageOverlayProvider::_setImage2(osg::Image* pImage)
{

}