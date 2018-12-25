#include <EarthWeb/Source/TileSource/TileSourcePGIS/TileSourcePGIS.h>
#include <atlstr.h>

std::wstring TileSourcePGIS::get_url()
{
	return ( std::wstring ) CString( getOrCreateInnerObject()->url().get().getString().data() );
}

void TileSourcePGIS::put_url( const std::wstring& strUrl )
{
	getOrCreateInnerObject()->url() = CStringA( strUrl.data() ).GetString();
}