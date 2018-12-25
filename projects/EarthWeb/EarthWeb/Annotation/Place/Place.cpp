#include <EarthWeb/Annotation/Place/Place.h>
#include <atlstr.h>
#include <EarthWeb/Utils/StaticUtils/StringStaticUtils.h>

bool Place::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void Place::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool Place::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void Place::setPosition( GeoPointPtr ptrGeoPoint )
{
	if ( ptrGeoPoint.get() == NULL )
	{
		return;
	}

	getOrCreateInnerObject()->setPosition( * ptrGeoPoint->getOrCreateInnerObject() );
}

GeoPointPtr Place::getPosition()
{
	GeoPointPtr ptrGeoPoint = boost::make_shared<GeoPoint>();
	*( ptrGeoPoint->getOrCreateInnerObject() ) = getOrCreateInnerObject()->getPosition();
	return ptrGeoPoint;
}

void Place::setText( const std::wstring& wstrText )
{
	std::string stra = StringStaticUtils::UrlDecode(CStringA(wstrText.data()).GetString());
	//getOrCreateInnerObject()->setLocalText(CStringA(wstrText.data()).GetString());
	getOrCreateInnerObject()->setLocalText(stra);
}

std::wstring Place::getText()
{
	return ( std::wstring ) CString( getOrCreateInnerObject()->getText().data() );
}

void Place::setTextExt(const std::wstring & str, unsigned int sz, const std::wstring &col, const std::wstring & fnt)
{
	std::string stra = StringStaticUtils::UrlDecode(CStringA(str.data()).GetString());
	char *stopstring;
	unsigned int c = strtoul(CStringA(col.data()).GetString(),&stopstring,16);
	//getOrCreateInnerObject()->setLocalTextExt(CStringA(str.data()).GetString(),sz,c, CStringA(fnt.data()).GetString());
	getOrCreateInnerObject()->setLocalTextExt(stra, sz, c, CStringA(fnt.data()).GetString());
}

void Place::setImageSrc( const std::wstring& wstrImgSrc )
{
	getOrCreateInnerObject()->setImageSrc( CStringA( wstrImgSrc.data() ).GetString() );
}

void Place::setImageSrc2(const std::wstring& wstrImgSrc)
{
	getOrCreateInnerObject()->setImageSrc2(CStringA(wstrImgSrc.data()).GetString());
}

void Place::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring Place::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

void Place::setShowText(bool bShow)
{
	getOrCreateInnerObject()->setShowText(bShow);
}

void Place::setShowTip(bool bShow)
{
	getOrCreateInnerObject()->showTip(bShow);
}

void Place::setFlash(bool bFlash)
{
	getOrCreateInnerObject()->setFlash(bFlash);
}

void Place::setTipContent(const std::wstring& wstrText, unsigned int tp)
{
	std::string stra = StringStaticUtils::UrlDecode(CStringA(wstrText.data()).GetString());
	//getOrCreateInnerObject()->setTipContent( CStringA(wstrText.data()).GetString(),tp);
	getOrCreateInnerObject()->setTipContent(stra, tp);
}


void Place::setTipButton(const std::wstring& wstrText)
{
	getOrCreateInnerObject()->setTipButton(CStringA(wstrText.data()).GetString());
}

std::wstring Place::getTipField(std::wstring& wstrText)
{
	return (std::wstring) CString(getOrCreateInnerObject()->getTipField(CStringA(wstrText.data()).GetString()).data());
}

void Place::setField(std::wstring strKey, std::wstring strVal)
{
	getOrCreateInnerObject()->setField(CStringA(strKey.data()).GetString(), CStringA(strVal.data()).GetString());

}

std::wstring Place::getField(std::wstring strKey)
{
	return (std::wstring) CString(getOrCreateInnerObject()->getField(CStringA(strKey.data()).GetString()).data());
}

void Place::startEdit()
{
	getOrCreateInnerObject()->startEdit();
}

void Place::stopEdit()
{
	getOrCreateInnerObject()->stopEdit();
}

void Place::setCulling(bool bCull)
{
	getOrCreateInnerObject()->setCulling(bCull);
}

void Place::setPriority(float fPriority)
{
	getOrCreateInnerObject()->setPriority(fPriority);
}

float Place::getPriority()
{
	return getOrCreateInnerObject()->getPriority();
}
