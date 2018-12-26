#include <Map/BaseMap.h>
#include <ThreadPool/LockMutex.h>

#include <osg/Multisample>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>

using namespace osgEarthXCore;

//OSGX
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>
#include "../../include/Provider/AnnotationProvider/PlaceProvider.h"
#include <Provider/AnnotationProvider/ModelProvider.h>
#include <osgEarthDrivers/mask_feature/FeatureMaskOptions>

BaseMap::BaseMap()
{
	m_pThreadPool = NULL;
	m_pMapViewer = NULL;

	m_refRoot = new osg::Group();
	//lijp
	//m_refRoot->setDataVariance( osg::Object::DYNAMIC );

	char* strSample = getenv("OSG_SAMPLE_NUM");
	if (strSample != 0)
	{
		int samp = atoi(strSample);
		if (samp == 8) {
			osg::Multisample* pms = new osg::Multisample;
			pms->setSampleCoverage(1, true);
			m_refRoot->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);
		}
	}
	//osg::Multisample* pms = new osg::Multisample;
	//pms->setSampleCoverage(1, true);
	//m_refRoot->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);

	m_refMap = new osgEarth::Map();
	m_refMapNode = new osgEarth::MapNode( m_refMap );

	m_refRoot->addChild( m_refMapNode );

	m_refModels = new osg::Group();
	m_refMapNode->addChild(m_refModels);
	m_refModels->setCullingActive(false);
	m_refAnalysis = new osg::Group();
	m_refRoot->addChild(m_refAnalysis);
}

BaseMap::~BaseMap()
{
	SCOPEDLOCK_WRITE_MUTEX( m_MapMutex );

	if ( m_pThreadPool )
	{
		delete m_pThreadPool;
	}

}

osg::Group* BaseMap::getMapRoot()
{
	return m_refRoot;
}

osgEarth::Map* BaseMap::getOsgEarthMap()
{
	return m_refMap;
}

osgEarth::MapNode* BaseMap::getMapNode()
{
	return m_refMapNode;
}

OpenThreads::ReadWriteMutex& BaseMap::getMapMutex()
{
	return m_MapMutex;
}

const osgEarth::SpatialReference* BaseMap::getSRS()
{
	return m_refMapNode->getMapSRS();
}

void BaseMap::setMaxThreadsNum( unsigned int num )
{
	getOrCreateThreadPool()->setMaxThreadsNum( num );
	getOrCreateThreadPool()->wakeUp();
}

unsigned int BaseMap::getMaxThreadsNum()
{
	return getOrCreateThreadPool()->getMaxThreadsNum();
}

osgXThreads::ThreadPool* BaseMap::getOrCreateThreadPool()
{
	if (m_pThreadPool == NULL)
	{
		m_pThreadPool = new osgXThreads::ThreadPool;
	}
	return m_pThreadPool;
}

class SetMaskLayerOperation : public osg::Operation
{
public:
	SetMaskLayerOperation(osgEarth::Map* map, osgEarth::MaskLayer* lyr) : _refMap(map),_maskLayer(lyr) {}

	virtual void operator()(osg::Object*)
	{
		_refMap->addTerrainMaskLayer(_maskLayer.get());
	}

protected:
	virtual ~SetMaskLayerOperation() {}

protected:
	osg::ref_ptr<osgEarth::Map> _refMap;;
	osg::ref_ptr<osgEarth::MaskLayer> _maskLayer;
private:
};

void BaseMap::setMaskLayer(std::vector<double>& cords, double dHeight, bool bOpacity)
{
	if (cords.size()==0 || cords.size() % 2 != 0 ) return;

	osgEarth::Drivers::OGRFeatureOptions ogrFeatureOpt;

	std::string strGeometry = "POLYGON((";
	for (int i = 0; i < cords.size(); i = i + 2)
	{
		char xy[64];
		sprintf(xy,"%.5f %.5f %.1f,", cords[i], cords[i + 1],dHeight);
		strGeometry += xy;
	}
	strGeometry.erase(strGeometry.size() - 1, 1);
	strGeometry += "))";
	//ogrFeatureOpt.geometryConfig() = osgEarth::Config("geometry", "POLYGON(( -111.0466 42.0015 0, -111.0467 40.9979 0, -109.0501 41.0007 0, -109.0452 36.9991 0, -114.0506 37.0004 0, -114.0417 41.9937 0))");
	ogrFeatureOpt.geometryConfig() = osgEarth::Config("geometry", strGeometry.c_str());
	//set the geometry
	ogrFeatureOpt.profile() = osgEarth::ProfileOptions("global-geodetic");

	osgEarth::Drivers::FeatureMaskOptions featureMaskOpt;
	featureMaskOpt.featureOptions() = ogrFeatureOpt;

	//set the featuresouceoption of the featuremasksource
	osgEarth::MaskLayerOptions maskLayerOpt;
	maskLayerOpt.name() = "mask";
	maskLayerOpt.driver() = featureMaskOpt;
	osg::ref_ptr<osgEarth::MaskLayer> maskLayer = new osgEarth::MaskLayer(maskLayerOpt);
	//m_refMap->addTerrainMaskLayer(maskLayer.get());
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new SetMaskLayerOperation(m_refMap.get(), maskLayer.get()));

	if (bOpacity) return;

	osg::ref_ptr<osgEarth::Symbology::Polygon> bott = new osgEarth::Symbology::Polygon();
	for (int i = 0; i < cords.size(); i = i + 2)
	{
		bott->push_back(cords[i], cords[i + 1], dHeight);
	}

	osgEarth::Symbology::Style bottStyle;
	bottStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color(0.665,0.643,0.620));

	osg::ref_ptr<osgEarth::Features::Feature>     bottFeature = new osgEarth::Features::Feature(bott, m_refMapNode->getMapSRS(), bottStyle);
	osg::ref_ptr<osgEarth::Annotation::FeatureNode> featureNode = new osgEarth::Annotation::FeatureNode(m_refMapNode, bottFeature);
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new AddGroupObjectOperation(m_refAnalysis.get(), featureNode.get()));
}

class RemoveMaskLayerOperation : public osg::Operation
{
public:
	RemoveMaskLayerOperation(osgEarth::Map* map, osgEarth::MaskLayer* lyr) : _refMap(map), _maskLayer(lyr) {}

	virtual void operator()(osg::Object*)
	{
		_refMap->removeTerrainMaskLayer(_maskLayer.get());
	}

protected:
	virtual ~RemoveMaskLayerOperation() {}

protected:
	osg::ref_ptr<osgEarth::Map> _refMap;;
	osg::ref_ptr<osgEarth::MaskLayer> _maskLayer;
private:
};

void BaseMap::removeMaskLayer()
{
	osgEarth::MaskLayerVector masks;
	m_refMap->getTerrainMaskLayers(masks);
	for (int i = 0; i < masks.size(); i++)
	{
		osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new RemoveMaskLayerOperation(m_refMap.get(), masks[i].get()));
	}
	//m_refMap->removeTerrainMaskLayer();
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new RemoveAllGroupObjectOperation(m_refAnalysis.get()));
}

void BaseMap::initMapViewer( MapViewer* pMapViewer )
{
	m_pMapViewer = pMapViewer;
}

void BaseMap::addGroupObject( osg::Group* pGroup, osg::Node* pNode )
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new AddGroupObjectOperation( pGroup, pNode ) );
}

void BaseMap::removeGroupObjet( osg::Group* pGroup, osg::Node* pNode )
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new RemoveGroupObjectOperation( pGroup, pNode ) );
}

void BaseMap::removeAllGroupObjet(osg::Group* pGroup)
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new RemoveAllGroupObjectOperation(pGroup));
}

/////////////////////////////
//Operations
/////////////////////////////

//AddGroupObjectOperation
AddGroupObjectOperation::AddGroupObjectOperation( osg::Group* pGroup, osg::Node* pNode )
{
	m_refGroup = pGroup;
	m_refNode = pNode;
}

AddGroupObjectOperation::~AddGroupObjectOperation()
{

}

void AddGroupObjectOperation::operator()( osg::Object* )
{
	if ( m_refGroup.get() == NULL || m_refNode.get() == NULL )
	{
		return;
	}

	m_refGroup->addChild( m_refNode );
}

//RemoveMapObjectOperation
RemoveGroupObjectOperation::RemoveGroupObjectOperation( osg::Group* pGroup, osg::Node* pNode )
{
	m_refGroup = pGroup;
	m_refNode = pNode;
}

void RemoveGroupObjectOperation::operator()( osg::Object* )
{
	if ( m_refGroup.get() == NULL || m_refNode.get() == NULL )
	{
		return;
	}
	osgEarthXCore::PlaceProvider* pla = dynamic_cast<osgEarthXCore::PlaceProvider *>(m_refNode.get());
	if (pla) {
		pla->clearAttached();
	}
	else {
		osgEarthXCore::ModelProvider* mdl = dynamic_cast<osgEarthXCore::ModelProvider *>(m_refNode.get());
		if (mdl) {
			mdl->clearAttachedEditor();
		}
	}
	m_refGroup->removeChild( m_refNode );
}

RemoveGroupObjectOperation::~RemoveGroupObjectOperation()
{

}

RemoveAllGroupObjectOperation::RemoveAllGroupObjectOperation(osg::Group* pGroup)
{
	m_refGroup = pGroup;
}

void RemoveAllGroupObjectOperation::operator()(osg::Object*)
{
	if (m_refGroup.get() == NULL)
	{
		return;
	}

	for (int i = 0; i < m_refGroup->getNumChildren(); i++) {
		osgEarthXCore::PlaceProvider* pla = dynamic_cast<osgEarthXCore::PlaceProvider *>(m_refGroup->getChild(i));
		if (pla) {
			pla->clearAttached();
		}
		else {
			osgEarthXCore::ModelProvider* mdl = dynamic_cast<osgEarthXCore::ModelProvider *>(m_refGroup->getChild(i));
			if (mdl) {
				mdl->clearAttachedEditor();
			}
		}
	}
	m_refGroup->removeChildren(0, m_refGroup->getNumChildren());
}

RemoveAllGroupObjectOperation::~RemoveAllGroupObjectOperation()
{
	
}