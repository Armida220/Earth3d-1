#include <Provider/AnnotationProvider/LabelProvider.h>
using namespace osgEarthXCore;

#include <ConvertChar/ConvertChar.h>

LabelProvider::LabelProvider()
{
	osgEarth::Symbology::TextSymbol* pTextSymbol = _style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	pTextSymbol->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	pTextSymbol->font() = "simsun.ttc";
	pTextSymbol->size() = 20;
	pTextSymbol->fill()->color() = osgEarth::Symbology::Color(0xff0000ff);
	pTextSymbol->halo()->color() = osgEarth::Symbology::Color(0xffffffff);
	pTextSymbol->halo()->width() = 8.0;
	init();
}

LabelProvider::~LabelProvider()
{

}

void LabelProvider::setLocalTextExt(const std::string & str, unsigned int sz, unsigned int col, const std::string & fnt)
{
	osgEarth::Symbology::TextSymbol* pTextSymbol = _style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	pTextSymbol->size() = sz;
	pTextSymbol->font() = fnt;
	pTextSymbol->fill()->color() = osgEarth::Symbology::Color(col);
	//setUnicodeText(osgX::ConvertChar::convertLocalStrToUTF8Str(str));
	init();	
	setUnicodeText(str);

}

void LabelProvider::setLocalText( const std::string& str )
{
	//setUnicodeText(osgX::ConvertChar::convertLocalStrToUTF8Str(str));
	setUnicodeText(str);
}

void LabelProvider::setUnicodeText( const std::string& str )
{
	LabelNode::setText( str );
}

std::string LabelProvider::getText()
{
	return _text;
}