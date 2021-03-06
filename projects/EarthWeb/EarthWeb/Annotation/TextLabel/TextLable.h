/**
	@brief Place JS�ӿ�
	@author Hao Luo
	@date 2015/04/02
*/

#ifndef OSGEARTHX_WEB_TEXTLABEL_H
#define OSGEARTHX_WEB_TEXTLABEL 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

#include <Annotation/TextLabel.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>

#define TextLabelDispatchImpl EarthObjectDispatchImpl< osgEarthX::TextLabel, TextLabel, IEarthObject >

FB_FORWARD_PTR(TextLabel)
class TextLabel : public TextLabelDispatchImpl
{
public:
	TextLabel( const std::vector<FB::variant>* pArgs = NULL ) : TextLabelDispatchImpl( ANNOTATION_TEXT_LABEL )
	{
		registerProperty("name", make_property(this, &TextLabel::getName, &TextLabel::setName));
		registerMethod( "setPosition", make_method( this, & TextLabel::setPosition ) );
		registerMethod( "getPosition", make_method( this, & TextLabel::getPosition ) );
		registerMethod( "setText", make_method( this, & TextLabel::setText ) );
		registerMethod( "getText", make_method( this, & TextLabel::getText ) );
		registerMethod("setTextExt", make_method(this, &TextLabel::setTextExt));
	}

	virtual ~TextLabel(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();
	void setName(const std::wstring& wstrText);
	std::wstring getName();
	//
	void setPosition( GeoPointPtr ptrGeoPoint );
	GeoPointPtr getPosition();
	void setText( const std::wstring& wstrText );
	void setTextExt(const std::wstring & str, unsigned int sz, const std::wstring &col, const std::wstring & fnt);
	std::wstring getText();

protected:
private:
};

#endif