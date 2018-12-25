#ifndef OSGEARTHX_WEB_TILE_SOURCE_BING_H
#define OSGEARTHX_WEB_TILE_SOURCE_BING_H 1

#include <EarthWeb/Source/SourceDispatchImpl.h>
#include <EarthWeb/Source/TileSource/ITileSourceDispath.h>

#include <osgEarthDrivers/bing/BingOptions>

#define TileSourceBingDispatchImpl SourceDispatchImpl< osgEarth::Drivers::BingOptions, TileSourceBing, ITileSourceDispatch >

FB_FORWARD_PTR(TileSourceBing)
class TileSourceBing : 
	public TileSourceBingDispatchImpl
{
public:
	TileSourceBing( const std::vector<FB::variant>* pArgs = NULL ) : TileSourceBingDispatchImpl( TILE_SOURCE_BING )
	{
		registerProperty("key", make_property(this, &TileSourceBing::get_key, &TileSourceBing::put_key));
		registerProperty("imageSet", make_property(this, &TileSourceBing::get_imageSet, &TileSourceBing::put_imageSet));
	}

	virtual ~TileSourceBing(){}

	std::wstring get_key();
	void put_key(const std::wstring& strKey);
	std::wstring get_imageSet();
	void put_imageSet(const std::wstring& strUrl);

protected:
private:
};
#endif