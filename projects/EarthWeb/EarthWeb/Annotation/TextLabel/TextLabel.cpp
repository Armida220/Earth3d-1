#include <EarthWeb/Annotation/TextLabel/TextLable.h>
#include <atlstr.h>
#include <EarthWeb/Utils/StaticUtils/StringStaticUtils.h>

void TextLabel::setName(const std::wstring& wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring TextLabel::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

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
	std::string stra = StringStaticUtils::UrlDecode(CStringA(wstrText.data()).GetString());
	//getOrCreateInnerObject()->setLocalText(CStringA(wstrText.data()).GetString());
	getOrCreateInnerObject()->setLocalText(stra);
}

//unsigned char ToHex(unsigned char x)
//{
//	return  x > 9 ? x + 55 : x + 48;
//}
//
//unsigned char FromHex(unsigned char x)
//{
//	unsigned char y;
//	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
//	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
//	else if (x >= '0' && x <= '9') y = x - '0';
//	else assert(0);
//	return y;
//}
//
//std::string UrlDecode(const std::string& str)
//{
//	std::string strTemp = "";
//	size_t length = str.length();
//	for (size_t i = 0; i < length; i++)
//	{
//		if (str[i] == '+') strTemp += ' ';
//		else if (str[i] == '%')
//		{
//			assert(i + 2 < length);
//			unsigned char high = FromHex((unsigned char)str[++i]);
//			unsigned char low = FromHex((unsigned char)str[++i]);
//			strTemp += high * 16 + low;
//		}
//		else strTemp += str[i];
//	}
//	return strTemp;
//}

void TextLabel::setTextExt(const std::wstring& str, unsigned sz, const std::wstring& col, const std::wstring& fnt)
{
	std::string stra = StringStaticUtils::UrlDecode(CStringA(str.data()).GetString());
	char *stopstring;
	unsigned int c = strtoul(CStringA(col.data()).GetString(), &stopstring, 16);
	//getOrCreateInnerObject()->setLocalTextExt(CStringA(str.data()).GetString(), sz, c, CStringA(fnt.data()).GetString());
	getOrCreateInnerObject()->setLocalTextExt(stra, sz, c, CStringA(fnt.data()).GetString());
}

std::wstring TextLabel::getText()
{
	return ( std::wstring ) CString( getOrCreateInnerObject()->getText().data() );
}
