#include <EarthWeb/Externals/Fire/Fire.h>
#include <atlstr.h>
bool Fire::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void Fire::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool Fire::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void Fire::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring Fire::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

void Fire::setPosition(GeoPointPtr ptrGeoPoint)
{
	if (ptrGeoPoint.get() == NULL)
	{
		return;
	}

	getOrCreateInnerObject()->setPosition(*ptrGeoPoint->getOrCreateInnerObject());
}