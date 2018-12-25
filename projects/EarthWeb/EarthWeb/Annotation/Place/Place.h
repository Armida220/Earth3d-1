/**
	@brief Place JS½Ó¿Ú
	@author Hao Luo
	@date 2015/04/02
*/

#ifndef OSGEARTHX_WEB_PLACE_H
#define OSGEARTHX_WEB_PLACE_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <Annotation/Place.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>

#define PlaceDispatchImpl EarthObjectDispatchImpl< osgEarthX::Place, Place, IEarthObject >

FB_FORWARD_PTR(Place)
class Place : public PlaceDispatchImpl
{
public:
	Place( const std::vector<FB::variant>* pArgs = NULL ) : PlaceDispatchImpl( ANNOTATION_PLACE )
	{
		registerProperty("name", make_property(this, &Place::getName, &Place::setName));
		registerProperty("priority", make_property(this, &Place::getPriority, &Place::setPriority));
		registerMethod( "setPosition", make_method( this, & Place::setPosition ) );
		registerMethod( "getPosition", make_method( this, & Place::getPosition ) );
		registerMethod( "setText", make_method( this, & Place::setText ) );
		registerMethod( "getText", make_method( this, & Place::getText ) );
		registerMethod( "setImageSrc", make_method( this, & Place::setImageSrc ) );
		registerMethod("setImageSrc2", make_method(this, &Place::setImageSrc2));
		registerMethod("setTextExt", make_method(this, &Place::setTextExt));
		registerMethod("setShowText", make_method(this, &Place::setShowText));
		registerMethod("setShowTip", make_method(this, &Place::setShowTip));
		registerMethod("setTipContent", make_method(this, &Place::setTipContent));
		registerMethod("setTipButton", make_method(this, &Place::setTipButton));
		registerMethod("getTipField", make_method(this, &Place::getTipField));
		registerMethod("setField", make_method(this, &Place::setField));
		registerMethod("getField", make_method(this, &Place::getField));
		registerMethod("startEdit", make_method(this, &Place::startEdit));
		registerMethod("stopEdit", make_method(this, &Place::stopEdit));
		registerMethod("setFlash", make_method(this, &Place::setFlash));
		registerMethod("setCulling", make_method(this, &Place::setCulling));
	}

	virtual ~Place(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();

	//

	void setPosition( GeoPointPtr ptrGeoPoint );
	GeoPointPtr getPosition();
	void setText( const std::wstring& wstrText );
	std::wstring getText();
	void setTextExt(const std::wstring & str, unsigned int sz, const std::wstring &col, const std::wstring & fnt);

	void setImageSrc( const std::wstring& wstrImgSrc );
	void setImageSrc2(const std::wstring& wstrImgSrc);
	void setName(const std::wstring& wstrText);
	std::wstring getName();
	void setShowText(bool bShow);
	void setShowTip(bool bShow);
	void setTipContent(const std::wstring& wstrText, unsigned int tp);
	void setTipButton(const std::wstring& wstrText);
	std::wstring getTipField( std::wstring& wstrText);
	void setField(std::wstring strKey, std::wstring strVal);
	std::wstring getField(std::wstring strKey);
	void startEdit();
	void stopEdit();
	void setFlash(bool bFlash);
	void setCulling(bool bCull);
	void setPriority(float fPriority);
	float getPriority();

protected:
private:
};

#endif