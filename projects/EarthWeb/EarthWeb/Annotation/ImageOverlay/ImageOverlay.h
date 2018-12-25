/**
	@brief ImageOverlay JS½Ó¿Ú
	@author Hao Luo
	@date 2015/04/02
*/

#ifndef OSGEARTHX_WEB_IMAGE_OVERLAY_H
#define OSGEARTHX_WEB_IMAGE_OVERLAY_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <Annotation/ImageOverlay.h>

#define ImageOverlayDispatchImp EarthObjectDispatchImpl< osgEarthX::ImageOverlay, ImageOverlay, IEarthObject >

FB_FORWARD_PTR(ImageOverlay)
class ImageOverlay : public ImageOverlayDispatchImp
{
public:
	ImageOverlay( const std::vector<FB::variant>* pArgs = NULL ) : ImageOverlayDispatchImp( ANNOTATION_IMAGE_OVERLAY )
	{
		registerProperty("name", make_property(this, &ImageOverlay::getName, &ImageOverlay::setName));
		registerProperty("visible", make_property(this, &ImageOverlay::get_visible, &ImageOverlay::put_visible));
		registerMethod( "setImageSrc", make_method( this, & ImageOverlay::setImageSrc ) );
		registerMethod( "setBounds", make_method( this, & ImageOverlay::setBounds ) );
		registerMethod("getImageSrc", make_method(this, &ImageOverlay::getImageSrc));
		registerMethod("getBounds", make_method(this, &ImageOverlay::getBounds));
		registerMethod("getImageWidth", make_method(this, &ImageOverlay::getImageWidth));
		registerMethod("getImageHeight", make_method(this, &ImageOverlay::getImageHeight));
		registerMethod("setCorners", make_method(this, &ImageOverlay::setCorners));
	}

	virtual ~ImageOverlay(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();

	void setName(const std::wstring& wstrText);
	std::wstring getName();

	void setImageSrc( const std::wstring& wstrImgSrc );
	void setCorners( double lowerLeftX, double lowerLeftY, double lowerRightX, double lowerRightY, double upperLeftX, double upperLeftY, double upperRightX, double upperRightY );
	void setBounds(double lowerLeftX, double lowerLeftY, double upperRightX, double upperRightY);

	std::wstring getImageSrc();
	std::vector<double> getBounds();

	int getImageWidth();
	int getImageHeight();

protected:
private:
	//std::string _wstrImgSrc;
};

#endif