#ifndef OSGEARTHX_WEB_TILE_SOURCE_PGIS_H
#define OSGEARTHX_WEB_TILE_SOURCE_PGIS_H 1

#include <EarthWeb/Source/SourceDispatchImpl.h>
#include <EarthWeb/Source/TileSource/ITileSourceDispath.h>

#include <osgEarthDrivers/PGIS/PGISOptions>

#define TileSourcePGISDispatchImpl SourceDispatchImpl< osgEarth::Drivers::PGISOptions, TileSourcePGIS, ITileSourceDispatch >

FB_FORWARD_PTR(TileSourcePGIS)
class TileSourcePGIS : 
	public TileSourcePGISDispatchImpl
{
public:
	TileSourcePGIS( const std::vector<FB::variant>* pArgs = NULL ) : TileSourcePGISDispatchImpl( TILE_SOURCE_PGIS )
	{
		registerProperty( "url", make_property( this,&TileSourcePGIS::get_url, &TileSourcePGIS::put_url  ) );
	}

	virtual ~TileSourcePGIS(){}

	std::wstring get_url();
	void put_url( const std::wstring& strUrl );

protected:
private:
};

#endif