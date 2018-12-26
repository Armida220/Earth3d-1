
#include <Map/Map.h>
#include <ThreadPool/LockMutex.h>
using namespace osgEarthXCore;

osgEarthXCore::Map::Map()
{
}

osgEarthXCore::Map::~Map()
{
}

void osgEarthXCore::Map::initMapViewer( MapViewer* pMapViewer )
{
	ImageLayerMap::initMapViewer( pMapViewer );
	ModelLayerMap::initMapViewer( pMapViewer );
	ElevationLayerMap::initMapViewer( pMapViewer );
	TerrainMaskLayerMap::initMapViewer( pMapViewer );
	ExternalsMap::initMapViewer( pMapViewer );
}