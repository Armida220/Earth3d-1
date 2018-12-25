#include <EarthWeb/Source/TileSource/TileSourceAGGLite/TileSourceAGGLite.h>
#include <EarthWeb/Utils/StaticUtils/SourceStaticUtils.h>

IFeatureSourceDispatchPtr TileSourceAGGLite::get_featureSource()
{
	IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch;
	return ptrIFeatureSourceDispatch;
}

void TileSourceAGGLite::put_featureSource( IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch )
{
	osgEarth::Features::FeatureSourceOptions* pOptions = SourceStaticUtils::getFeatureSourceOptions( ptrIFeatureSourceDispatch.get() );
	if ( pOptions )
	{
		getOrCreateInnerObject()->featureOptions() = *pOptions;
	}
}