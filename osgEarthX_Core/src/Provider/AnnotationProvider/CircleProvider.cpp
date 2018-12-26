#include <Provider/AnnotationProvider/CircleProvider.h>
using namespace osgEarthXCore;

#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>

class UpdateRadiusOperation : public osg::Operation
{
public:
	UpdateRadiusOperation(CircleProvider* ftr,double dRadius);
	virtual void operator()(osg::Object*);

protected:
	virtual ~UpdateRadiusOperation();

protected:
	osg::ref_ptr<CircleProvider> m_ftr;
	double m_dRadius;
private:
};

UpdateRadiusOperation::UpdateRadiusOperation(CircleProvider* ftr, double dRadius)
{
	m_dRadius = dRadius;
	m_ftr = ftr;
}

void UpdateRadiusOperation::operator()(osg::Object*)
{
	m_ftr->setRadius(m_dRadius);
}

UpdateRadiusOperation::~UpdateRadiusOperation()
{

}

void CircleProvider::getDefaultStyle( osgEarth::Symbology::Style& style )
{
	style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Cyan, 0.5);
	style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
}

CircleProvider::CircleProvider()
{
	osgEarth::Symbology::Style style;
	getDefaultStyle( style );
	setStyle( style );
}

CircleProvider::~CircleProvider()
{

}

void CircleProvider::initMapNode( osgEarth::MapNode* pMapNode )
{
	CircleNode::initMapNode( pMapNode );
}

void CircleProvider::UpdateRadius(double dRadius)
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new UpdateRadiusOperation(this,dRadius));
}
