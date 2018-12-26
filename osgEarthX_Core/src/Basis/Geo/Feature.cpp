#include <Basis/Geo/Feature.h>
using namespace osgEarthXCore;

unsigned int Feature::_col = 0xFF0000FF;
 unsigned int Feature::_fillcol = 0xFF0000FF;
 float Feature::_width = 3;
 bool Feature::_bClamp = false;
 unsigned short Feature::_pattern = 0xffff;

void Feature::getDefaultPolygonStyle( osgEarth::Symbology::Style& outStyle )
{
	//outStyle.getOrCreate< osgEarth::Symbology::PolygonSymbol >()->fill()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	//outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

	outStyle.getOrCreate< osgEarth::Symbology::PolygonSymbol >()->fill()->color() = osgEarth::Symbology::Color(_fillcol);
	if (_bClamp) {
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	}
	else {
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;

	}
	osgEarth::Symbology::RenderSymbol* render = outStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 6.6f;
}

void Feature::getDefaultLineStringStyle( osgEarth::Symbology::Style& outStyle )
{
	//outStyle.getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->color() = osgEarth::Symbology::Color::Red;
	//outStyle.getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->width() = 3;
	////outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	////outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	//outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;

	outStyle.getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->color() = osgEarth::Symbology::Color(_col);
	outStyle.getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->width() = _width;
	outStyle.getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->stipplePattern() = _pattern;
	if (_bClamp) {
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	}
	else {
		outStyle.getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
	}
	osgEarth::Symbology::RenderSymbol* render = outStyle.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	render->depthOffset()->minBias() = 6.6f;
}

Feature::Feature()
{

}

Feature::~Feature()
{

}

void Feature::init( const osgEarth::SpatialReference* pSRS )
{
	if ( ! _geom.valid() && _style.isSet() )
	{
		return;
	}	
	
	osgEarth::Style style = _style.get();
	switch( _geom->getType() )
	{
	case osgEarth::Symbology::Geometry::TYPE_POINTSET:
		break;
	case osgEarth::Symbology::Geometry::TYPE_LINESTRING:
		getDefaultLineStringStyle( style );
		break;

	case osgEarth::Symbology::Geometry::TYPE_RING:
		getDefaultLineStringStyle( style );
		break;

	case osgEarth::Symbology::Geometry::TYPE_POLYGON:
		getDefaultPolygonStyle( style );
		break;

	case osgEarth::Symbology::Geometry::TYPE_MULTI:
		break;

	default:
		break;
	}

	_style = style;

	if ( _srs.valid() )
	{
		transform( pSRS );
	}
	else
	{
		_srs = pSRS;
	}
}

void osgEarthXCore::Feature::setColor(unsigned int col)
{
	if (!_style.isSet())
	{
		_col = col;
		return;
	}

	_style->getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->color() = osgEarth::Symbology::Color(col);
}

void osgEarthXCore::Feature::setFillcolor(unsigned int col)
{
	if (!_style.isSet())
	{
		_fillcol = col;
		return;
	}
	_style->getOrCreate< osgEarth::Symbology::PolygonSymbol >()->fill()->color() = osgEarth::Symbology::Color(col);

}

void osgEarthXCore::Feature::setWidth(float w)
{
	if (!_style.isSet())
	{
		_width = w;
		return;
	}
	_style->getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->width() = w;
}

void osgEarthXCore::Feature::setClamp(bool bClamp)
{
	if (!_style.isSet())
	{
		_bClamp = bClamp;
		return;
	}
	if (bClamp) {
		_style->getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		_style->getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
	}
	else{
		_style->getOrCreate< osgEarth::Symbology::AltitudeSymbol >()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;
    }
}

void osgEarthXCore::Feature::setPattern(unsigned short pattern)
{
	if (!_style.isSet())
	{
		_pattern = pattern;
		return;
	}
	_style->getOrCreate< osgEarth::Symbology::LineSymbol >()->stroke()->stipplePattern() = pattern;
}
