#include <EarthWeb/Layer/ImageSurfaceLayer/ImageSurfaceLayer.h>
#include <EarthWeb/Utils/StaticUtils/SourceStaticUtils.h>

bool ImageSurfaceLayer::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void ImageSurfaceLayer::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool ImageSurfaceLayer::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

float ImageSurfaceLayer::get_opacity()
{
	return getOrCreateInnerObject()->getOpacity();
}

void ImageSurfaceLayer::put_opacity(float opa)
{
	getOrCreateInnerObject()->setOpacity(opa);
}

void ImageSurfaceLayer::testEvent()
{
	fire_onInitializeCompleted();
}

ITileSourceDispatchPtr ImageSurfaceLayer::get_tileSource()
{
	ITileSourceDispatchPtr ptrITileSourceDispatch;
	return ptrITileSourceDispatch;
}

void ImageSurfaceLayer::put_tileSource( ITileSourceDispatchPtr ptrITileSourceDispatch )
{
	osgEarth::TileSourceOptions* pOptions = SourceStaticUtils::getTileSourceOptions( ptrITileSourceDispatch.get() );
	if ( pOptions )
	{
		getOrCreateInnerObject()->setTileSource( *pOptions );
	}
}