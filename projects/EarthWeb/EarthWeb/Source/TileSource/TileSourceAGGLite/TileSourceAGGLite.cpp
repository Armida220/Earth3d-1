#include <EarthWeb/Source/TileSource/TileSourceAGGLite/TileSourceAGGLite.h>
#include <EarthWeb/Utils/StaticUtils/SourceStaticUtils.h>
#include <atlstr.h>

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

std::wstring TileSourceAGGLite::get_fill()
{
	return _fillColor;
}

void TileSourceAGGLite::put_fill(std::wstring col)
{
	_fillColor = col;

	osgEarth::Symbology::PolygonSymbol* poly = getOrCreateInnerObject()->styles()->getStyle("first")->getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color(CStringA(_fillColor.data()).GetString());

}

std::wstring TileSourceAGGLite::get_color()
{
	return _lineColor;
}

void TileSourceAGGLite::put_color(std::wstring col)
{
	_lineColor = col;

	osgEarth::Symbology::LineSymbol* lin = getOrCreateInnerObject()->styles()->getStyle("first")->getOrCreate<osgEarth::Symbology::LineSymbol>();
	lin->stroke()->color() = osgEarth::Symbology::Color(CStringA(_lineColor.data()).GetString());
}

double TileSourceAGGLite::get_width()
{
	return _width;
}

void TileSourceAGGLite::put_width(double w)
{
	_width = w;
	osgEarth::Symbology::LineSymbol* lin = getOrCreateInnerObject()->styles()->getStyle("first")->getOrCreate<osgEarth::Symbology::LineSymbol>();
	lin->stroke()->width() = w;

}
