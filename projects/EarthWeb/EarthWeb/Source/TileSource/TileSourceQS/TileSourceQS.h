#ifndef OSGEARTHX_WEB_TILE_SOURCE_QS_H
#define OSGEARTHX_WEB_TILE_SOURCE_QS_H 1

#include <EarthWeb/Source/SourceDispatchImpl.h>
#include <EarthWeb/Source/TileSource/ITileSourceDispath.h>

#include <osgEarthDrivers/geobeans_quad/QuadOptions>

#define TileSourceQSDispatchImpl SourceDispatchImpl< osgEarth::Drivers::QuadOptions, TileSourceQS, ITileSourceDispatch >

FB_FORWARD_PTR(TileSourceQS)
class TileSourceQS : 
	public TileSourceQSDispatchImpl
{
public:
	TileSourceQS( const std::vector<FB::variant>* pArgs = NULL ) : TileSourceQSDispatchImpl(TILE_SOURCE_QS)
	{
		registerProperty( "url", make_property( this,&TileSourceQS::get_url, &TileSourceQS::put_url  ) );
	}

	virtual ~TileSourceQS(){}

	std::wstring get_url();
	void put_url( const std::wstring& strUrl );

protected:
private:
};

#endif