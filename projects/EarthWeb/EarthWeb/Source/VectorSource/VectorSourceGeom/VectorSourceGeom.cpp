#include <EarthWeb/Source/VectorSource/VectorSourceGeom/VectorSourceGeom.h>
#include <EarthWeb/Utils/StaticUtils/SourceStaticUtils.h>
#include <atlstr.h>

#define RESOURCE_LIB_URL "C:/prjs/osgEarthX/environment/data/resources/textures_us/catalog.xml"
VectorSourceGeom::VectorSourceGeom( const std::vector<FB::variant>* pArgs ) : VectorSourceGeomDispatchImpl( VECTOR_SOURCE_GEOM )
{
	_register();

	osgEarth::Symbology::Style style;
	style.setName("default");
	osgEarth::Symbology::LineSymbol* lineSymbol = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	lineSymbol->stroke()->color() = osgEarth::Symbology::Color::White;
	//lineSymbol->stroke()->width() = 1.0f;
	lineSymbol->stroke()->width() = 20.5f;
	lineSymbol->stroke()->widthUnits() = osgEarth::Units::METERS;
	_stockColor = (std::wstring) CString("0xffff00");
	_fillColor = (std::wstring) CString("0xffff00");

	// Render the data as translucent yellow lines that are 7.5m wide.
	//osgEarth::Symbology::LineSymbol* line = style.getOrCreate<osgEarth::Symbology::LineSymbol>();
	//line->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 0.5f);
	//line->stroke()->width() = 7.5f;
	//line->stroke()->widthUnits() = osgEarth::Units::METERS;

	// Clamp the lines to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;

	// Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
	// apparent offset (towards the camera) of the geometry from its actual position.
	// The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
	osgEarth::Symbology::RenderSymbol* render = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 6.6f;


	getOrCreateInnerObject()->styles() = new osgEarth::Symbology::StyleSheet();
	getOrCreateInnerObject()->styles()->addStyle( style );
}

IFeatureSourceDispatchPtr VectorSourceGeom::get_featureSource()
{
	IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch;
	return ptrIFeatureSourceDispatch;
}

void VectorSourceGeom::put_featureSource( IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch )
{
	osgEarth::Features::FeatureSourceOptions* pOptions = SourceStaticUtils::getFeatureSourceOptions( ptrIFeatureSourceDispatch.get() );
	if ( pOptions )
	{
		getOrCreateInnerObject()->featureOptions() = *pOptions;
	}
}

std::wstring VectorSourceGeom::get_stock()
{
	return _stockColor;
}

void VectorSourceGeom::put_stock(std::wstring col)
{
	osgEarth::Symbology::Style *style = getOrCreateInnerObject()->styles()->getStyle("default");
	_stockColor = col;
	//osgEarth::Symbology::Style style;
	//style.setName("default");
	osgEarth::Symbology::LineSymbol* lineSymbol = style->getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	lineSymbol->stroke()->color() = osgEarth::Symbology::Color(CStringA(col.data()).GetString());
	//lineSymbol->stroke()->width() = 20.0f;

	// Clamp the lines to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style->getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//alt->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	// Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
	// apparent offset (towards the camera) of the geometry from its actual position.
	// The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
	osgEarth::Symbology::RenderSymbol* render = style->getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 7.0f;

	//getOrCreateInnerObject()->styles() = new osgEarth::Symbology::StyleSheet();
	//getOrCreateInnerObject()->styles()->addStyle(style);
}

void VectorSourceGeom::setStockEx(std::wstring col, double width, unsigned short pattern)
{
	getOrCreateInnerObject()->styles()->removeStyle("default");
	_stockColor = col;
	osgEarth::Symbology::Style style;
	style.setName("default");
	osgEarth::Symbology::LineSymbol* lineSymbol = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	lineSymbol->stroke()->color() = osgEarth::Symbology::Color(CStringA(col.data()).GetString()); //osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 0.5f);
	lineSymbol->stroke()->stipplePattern() = pattern;// 0x1111;
	lineSymbol->stroke()->width() = width;
	//lineSymbol->stroke()->widthUnits() = osgEarth::Units::METERS;

	// Clamp the lines to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//alt->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	// Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
	// apparent offset (towards the camera) of the geometry from its actual position.
	// The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
	osgEarth::Symbology::RenderSymbol* render = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 7.0f;

	getOrCreateInnerObject()->styles()->addStyle(style);
}

std::wstring VectorSourceGeom::get_fill()
{
	return _fillColor;
}

void VectorSourceGeom::put_fill(std::wstring col)
{
	_fillColor = col;
	getOrCreateInnerObject()->styles()->removeStyle("default");
	osgEarth::Symbology::Style style;
	style.setName("default");

	osgEarth::Symbology::PolygonSymbol* poly = style.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color(CStringA(_fillColor.data()).GetString());

	//osgEarth::Symbology::TextSymbol* text = style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	//text->content() = osgEarth::Symbology::StringExpression("[name]");
	//text->priority() = osgEarth::Symbology::NumericExpression("[area1]");
	//text->removeDuplicateLabels() = true;
	//text->size() = 16.0f;
	//text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	//text->fill()->color() = osgEarth::Symbology::Color::White;
	//text->halo()->color() = osgEarth::Symbology::Color::DarkGray;

	// Clamp the buildings to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//alt->technique() = alt->TECHNIQUE_DRAPE;
	alt->binding() = alt->BINDING_VERTEX;

	osgEarth::Symbology::RenderSymbol* render = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 6.0f;

	getOrCreateInnerObject()->styles()->addStyle(style);

}

std::wstring VectorSourceGeom::get_extrude()
{
	return _extrude;
}

void VectorSourceGeom::put_extrude(std::wstring col)
{
	_extrude = col;
	//getOrCreateInnerObject()->styles()->removeStyle("default");
	//osgEarth::Symbology::Style style;
	//style.setName("default");

	//osgEarth::Symbology::PolygonSymbol* poly = style.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	//poly->fill()->color() = osgEarth::Symbology::Color(CStringA(_fillColor.data()).GetString());;

	//// Clamp the buildings to the terrain.
	//osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	//alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//alt->binding() = alt->BINDING_VERTEX;

	//osgEarth::Symbology::ExtrusionSymbol* extrusion = style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>();
	//extrusion->heightExpression() = osgEarth::Symbology::NumericExpression(CStringA(_extrude.data()).GetString()); //("3.5 * max( [story_ht_], 1 )");
	//extrusion->flatten() = true;
	//extrusion->wallGradientPercentage() = 0.7;
	//getOrCreateInnerObject()->styles()->addStyle(style);

	getOrCreateInnerObject()->styles()->removeStyle("default");
	osgEarth::Symbology::Style buildingStyle;
	buildingStyle.setName("buildings");
	// Extrude the shapes into 3D buildings.
	osgEarth::Symbology::ExtrusionSymbol* extrusion = buildingStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>();
	extrusion->heightExpression() = osgEarth::Symbology::NumericExpression(CStringA(_extrude.data()).GetString());//("3.5 * max( [story_ht_], 1 )");
	extrusion->flatten() = true;
	extrusion->wallStyleName() = "building-wall";
	extrusion->roofStyleName() = "building-roof";

	osgEarth::Symbology::PolygonSymbol* poly = buildingStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color::White;

	// Clamp the buildings to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = buildingStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	alt->binding() = alt->BINDING_VERTEX;

	// a style for the wall textures:
	osgEarth::Symbology::Style wallStyle;
	wallStyle.setName("building-wall");
	osgEarth::Symbology::SkinSymbol* wallSkin = wallStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
	wallSkin->libraryName() = "us_resources";
	wallSkin->addTag("building");
	wallSkin->randomSeed() = 1;

	// a style for the rooftop textures:
	osgEarth::Symbology::Style roofStyle;
	roofStyle.setName("building-roof");
	osgEarth::Symbology::SkinSymbol* roofSkin = roofStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
	roofSkin->libraryName() = "us_resources";
	roofSkin->addTag("rooftop");
	roofSkin->randomSeed() = 1;
	roofSkin->isTiled() = true;

	// assemble a stylesheet and add our styles to it:
	getOrCreateInnerObject()->styles()->addStyle(buildingStyle);
	getOrCreateInnerObject()->styles()->addStyle(wallStyle);
	getOrCreateInnerObject()->styles()->addStyle(roofStyle);

	// load a resource library that contains the building textures.
	osgEarth::Symbology::ResourceLibrary* reslib = new osgEarth::Symbology::ResourceLibrary("us_resources", RESOURCE_LIB_URL);
	getOrCreateInnerObject()->styles()->addResourceLibrary(reslib);

	// set up a paging layout for incremental loading. The tile size factor and
	// the visibility range combine to determine the tile size, such that
	// tile radius = max range / tile size factor.
	osgEarth::Features::FeatureDisplayLayout layout;
	layout.tileSizeFactor() = 52.0;
	layout.addLevel(osgEarth::Features::FeatureLevel(0.0f, 20000.0f, "buildings"));

	// create a model layer that will render the buildings according to our style sheet.
	getOrCreateInnerObject()->layout() = layout;


}

void VectorSourceGeom::addextrude(std::wstring name, std::wstring h, std::wstring col, std::wstring con)
{
	osgEarth::Symbology::Style style;
	style.setName(CStringA(name.data()).GetString());

	osgEarth::Symbology::PolygonSymbol* poly = style.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color(CStringA(col.data()).GetString());;

	// Clamp the buildings to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	alt->binding() = alt->BINDING_VERTEX;

	osgEarth::Symbology::ExtrusionSymbol* extrusion = style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>();
	extrusion->heightExpression() = osgEarth::Symbology::NumericExpression(CStringA(h.data()).GetString()); //("3.5 * max( [story_ht_], 1 )");
	extrusion->flatten() = true;
	extrusion->wallGradientPercentage() = 0.7;

	getOrCreateInnerObject()->styles()->addStyle(style);

	osgEarth::Symbology::StyleSelector sel1;
	sel1.name() = CStringA(name.data()).GetString();
	sel1.styleName() = CStringA(name.data()).GetString();
	osgEarth::Symbology::Query qu1;
	qu1.expression() = CStringA(con.data()).GetString();
	sel1.query() = qu1;
	getOrCreateInnerObject()->styles()->selectors().push_back(sel1);
}

void VectorSourceGeom::addfill(std::wstring name, std::wstring col, std::wstring con)
{
	osgEarth::Symbology::Style style;
	style.setName(CStringA(name.data()).GetString());

	osgEarth::Symbology::PolygonSymbol* poly = style.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color(CStringA(col.data()).GetString());

	//osgEarth::Symbology::TextSymbol* text = style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	//text->content() = osgEarth::Symbology::StringExpression("[name]");
	//text->priority() = osgEarth::Symbology::NumericExpression("[area]");
	//text->removeDuplicateLabels() = true;
	//text->size() = 16.0f;
	//text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	//text->fill()->color() = osgEarth::Symbology::Color::White;
	//text->halo()->color() = osgEarth::Symbology::Color::DarkGray;

	// Clamp the buildings to the terrain.
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//alt->technique() = alt->TECHNIQUE_DRAPE;
	alt->binding() = alt->BINDING_VERTEX;

	osgEarth::Symbology::RenderSymbol* render = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 6.0f;

	getOrCreateInnerObject()->styles()->addStyle(style);

	osgEarth::Symbology::StyleSelector sel1;
	sel1.name() = CStringA(name.data()).GetString();
	sel1.styleName() = CStringA(name.data()).GetString();
	osgEarth::Symbology::Query qu1;
	qu1.expression() = CStringA(con.data()).GetString();
	sel1.query() = qu1;
	getOrCreateInnerObject()->styles()->selectors().push_back(sel1);
}
