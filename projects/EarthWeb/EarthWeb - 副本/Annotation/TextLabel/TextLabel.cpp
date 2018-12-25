#include <EarthWeb/Annotation/TextLabel/TextLable.h>
#include <atlstr.h>

bool TextLabel::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void TextLabel::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool TextLabel::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void TextLabel::setPosition( GeoPointPtr ptrGeoPoint )
{
	if ( ptrGeoPoint.get() == NULL )
	{
		return;
	}

	getOrCreateInnerObject()->setPosition( * ptrGeoPoint->getOrCreateInnerObject() );
}

GeoPointPtr TextLabel::getPosition()
{
	GeoPointPtr ptrGeoPoint = boost::make_shared<GeoPoint>();
	*( ptrGeoPoint->getOrCreateInnerObject() ) = getOrCreateInnerObject()->getPosition();
	return ptrGeoPoint;
}

void TextLabel::setText( const std::wstring& wstrText )
{
	getOrCreateInnerObject()->setText( CStringA( wstrText.data() ).GetString() );
}

std::wstring TextLabel::getText()
{
	return ( std::wstring ) CString( getOrCreateInnerObject()->getText().data() );
}
