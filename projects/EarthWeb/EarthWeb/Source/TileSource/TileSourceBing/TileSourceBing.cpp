#include <EarthWeb/Source/TileSource/TileSourceBing/TileSourceBing.h>
#include <atlstr.h>
std::wstring TileSourceBing::get_key()
{
	return std::wstring();
}

void TileSourceBing::put_key(const std::wstring & strKey)
{
	getOrCreateInnerObject()->key() = CStringA(strKey.data()).GetString();
}

std::wstring TileSourceBing::get_imageSet()
{
	return std::wstring();
}

void TileSourceBing::put_imageSet(const std::wstring & strUrl)
{
	getOrCreateInnerObject()->imagerySet() = CStringA(strUrl.data()).GetString();
}
