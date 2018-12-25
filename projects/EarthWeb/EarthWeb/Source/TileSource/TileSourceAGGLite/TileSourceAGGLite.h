#ifndef OSGEARTHX_WEB_TILE_SOURCE_AGGLITE_H
#define OSGEARTHX_WEB_TILE_SOURCE_AGGLITE_H 1

#include <EarthWeb/Source/SourceDispatchImpl.h>
#include <EarthWeb/Source/TileSource/ITileSourceDispath.h>

#include <osgEarthDrivers/agglite/AGGLiteOptions>

#define TileSourceAGGLiteDispatchImpl SourceDispatchImpl< osgEarth::Drivers::AGGLiteOptions, TileSourceAGGLite, ITileSourceDispatch >
#include <EarthWeb/Source/FeatureSource/IFeatureSourceDispatch.h>

FB_FORWARD_PTR(TileSourceAGGLite)
class TileSourceAGGLite : 
	public TileSourceAGGLiteDispatchImpl
{
public:
	TileSourceAGGLite( const std::vector<FB::variant>* pArgs = NULL ) : TileSourceAGGLiteDispatchImpl( TILE_SOURCE_AGGLITE )
	{
		registerProperty( "featureSource", make_property( this, &TileSourceAGGLite::get_featureSource, &TileSourceAGGLite::put_featureSource ) );
		registerProperty("fill", make_property(this, &TileSourceAGGLite::get_fill, &TileSourceAGGLite::put_fill));
		registerProperty("color", make_property(this, &TileSourceAGGLite::get_color, &TileSourceAGGLite::put_color));
		registerProperty("width", make_property(this, &TileSourceAGGLite::get_width, &TileSourceAGGLite::put_width));

		
		osgEarth::Symbology::Style style("first");
		osgEarth::Symbology::LineSymbol* lineSymbol = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
		lineSymbol->stroke()->color() = osgEarth::Symbology::Color::Yellow;
		lineSymbol->stroke()->width() = 2.0f;

		getOrCreateInnerObject()->styles() = new osgEarth::Symbology::StyleSheet();
		getOrCreateInnerObject()->styles()->addStyle( style );
	}

	virtual ~TileSourceAGGLite(){}
    
	IFeatureSourceDispatchPtr get_featureSource();
	void put_featureSource( IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch );

	std::wstring get_fill();
	void put_fill(std::wstring col);

	std::wstring get_color();
	void put_color(std::wstring col);

	double get_width();
	void put_width(double w);

protected:
private:
	std::wstring _fillColor;
	std::wstring _lineColor;
	double _width;
};


#endif