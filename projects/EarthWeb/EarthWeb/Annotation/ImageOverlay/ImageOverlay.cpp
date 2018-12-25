#include <EarthWeb/Annotation/ImageOverlay/ImageOverlay.h>
#include <atlstr.h>

bool ImageOverlay::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void ImageOverlay::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool ImageOverlay::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void ImageOverlay::setImageSrc( const std::wstring& wstrImgSrc )
{
	getOrCreateInnerObject()->setImageSrcNoCache(CStringA(wstrImgSrc.data()).GetString());
}

void ImageOverlay::setCorners(double lowerLeftX, double lowerLeftY, double lowerRightX, double lowerRightY, double upperLeftX, double upperLeftY, double upperRightX, double upperRightY)
{
	getOrCreateInnerObject()->setCorners(osg::Vec2d(lowerLeftX, lowerLeftY), osg::Vec2d(lowerRightX, lowerRightY), osg::Vec2d(upperLeftX, upperLeftY), osg::Vec2d(upperRightX, upperRightY));
}

void ImageOverlay::setBounds( double lowerLeftX, double lowerLeftY, double upperRightX, double upperRightY )
{
	getOrCreateInnerObject()->setBounds( osgEarth::Bounds::Bounds( lowerLeftX, lowerLeftY, upperRightX, upperRightY ) );
}

std::wstring ImageOverlay::getImageSrc()
{
	return  (std::wstring) CString(getOrCreateInnerObject()->getImageSrcNoCache().data());
}

std::vector<double> ImageOverlay::getBounds()
{
	osgEarth::Bounds bounds = getOrCreateInnerObject()->getBounds();
	std::vector<double> res;
	res.push_back(bounds.xMin());
	res.push_back(bounds.yMin());
	res.push_back(bounds.xMax());
	res.push_back(bounds.yMax());
	return res;
}

int ImageOverlay::getImageWidth()
{
	return getOrCreateInnerObject()->getImage()->s();
}

int ImageOverlay::getImageHeight()
{
	return getOrCreateInnerObject()->getImage()->t();
}

void ImageOverlay::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring ImageOverlay::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}