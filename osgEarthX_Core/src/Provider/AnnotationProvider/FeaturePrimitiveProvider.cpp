#include <Provider/AnnotationProvider/FeaturePrimitiveProvider.h>
using namespace osgEarthXCore;

#include <Basis/Geo/Feature.h>
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>

class UpdateFeatureOperation : public osg::Operation
{
public:
	UpdateFeatureOperation(FeaturePrimitiveProvider* ftr);
	virtual void operator()(osg::Object*);

protected:
	virtual ~UpdateFeatureOperation();

protected:
	osg::ref_ptr<FeaturePrimitiveProvider> m_ftr;
private:
};

UpdateFeatureOperation::UpdateFeatureOperation(FeaturePrimitiveProvider* ftr)
{
	m_ftr = ftr;
}

void UpdateFeatureOperation::operator()(osg::Object*)
{
	m_ftr->init();
}

UpdateFeatureOperation::~UpdateFeatureOperation()
{

}


FeaturePrimitiveProvider::FeaturePrimitiveProvider()
{

}

FeaturePrimitiveProvider::~FeaturePrimitiveProvider()
{
	
}

void FeaturePrimitiveProvider::initMapNode( osgEarth::MapNode* pMapNode )
{
	//Èç¹ûÊÇosgEarthX::Feature
	Feature* pFeature = dynamic_cast< Feature* >( _feature.get() );
	if( pFeature )
	{
		pFeature->init( pMapNode->getMapSRS() );
	}

	FeatureNode::initMapNode( pMapNode );
	FeatureNode::init();
}

void osgEarthXCore::FeaturePrimitiveProvider::update()
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new UpdateFeatureOperation(this));
}
