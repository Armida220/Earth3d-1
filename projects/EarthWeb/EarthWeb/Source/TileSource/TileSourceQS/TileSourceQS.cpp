#include <EarthWeb/Source/TileSource/TileSourceQS/TileSourceQS.h>
#include <atlstr.h>

std::wstring TileSourceQS::get_url()
{
	return ( std::wstring ) CString( getOrCreateInnerObject()->url().get().getString().data() );
}

void TileSourceQS::put_url( const std::wstring& strUrl )
{
	getOrCreateInnerObject()->url() = CStringA( strUrl.data() ).GetString();
}