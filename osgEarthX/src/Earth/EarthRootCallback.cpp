#include <Earth/Earth.h>
#include <Earth/EarthRootCallback.h>
#include <Group/GroupChange.h>

#include <Layer/ImageSurfaceLayer.h>
#include <Layer/VectorSurfaceLayer.h>
#include <Layer/ElevationSurfaceLayer.h>

#include <Externals/Viewpoint.h>
#include <Externals/Sky.h>

#include <Annotation\Model.h>

#include <osgEarthDrivers/model_simple/SimpleModelOptions>

using namespace osgEarthX;

#include <cassert>
#include <osgEarthAnnotation/PlaceNode>

EarthRootCallback::EarthRootCallback( Earth* pEarth )
{
	assert( pEarth != NULL );
	m_pEarth = pEarth;
	m_pEarthRoot = m_pEarth->getEarthRoot();
}

EarthRootCallback::~EarthRootCallback()
{

}

void EarthRootCallback::doCallback(osgEarthXCore::Change* pChange)
{
	osgEarthX::GroupChange* pGroupChange = dynamic_cast< osgEarthX::GroupChange* >( pChange );

	if ( pGroupChange == NULL )
	{
		return;
	}

	switch( pGroupChange->getChangeType().changeType )
	{
	case GroupChangeType::ADD_OBJECT:
		{
			this->addObject( pGroupChange->getChangeType().pObject );
			break;
		}
		

	case GroupChangeType::REMOVE_OBJECT:
		{
			removeObject( pGroupChange->getChangeType().pObject );
		}
		break;

	case GroupChangeType::MOVE_OBJECT:
		{
			moveObject( pGroupChange->getChangeType().pObject );
		}
		break;

	case GroupChangeType::INSERT_OBJECT:
		insertObject( pGroupChange->getChangeType().pObject );
		break;
	case GroupChangeType::REMOVE_ANNOTATIONS:
		removeAllAnnotations();
		break;
	case GroupChangeType::RESET_SCENE:
		resetScene();
		break;
	default:
		break;
	}
}

void EarthRootCallback::removeAllAnnotations()
{
	m_pEarth->getEarthMap()->removeAllAnnotations();
}

#include <Windows.h>
void EarthRootCallback::addObject(Object*pObject)
{

	switch( pObject->getObjectType() )
	{
	case OBJECT:
		break;

	case GROUP:
		{
			Group* pGroup = ( Group* ) pObject;
			pGroup->addCallback( m_pEarthRoot->getNotifiedGroupCallback() );

			//������ӽڵ�������
			ObjectsDynamicVector& objectsVector = pGroup->getObjects();
			for ( unsigned int i = 0, size = objectsVector.size(); i < size; i++ )
			{
				EarthRootCallback::addObject( objectsVector[ i ] );
			}
		}
		break;

	case IMAGE_SURFACE_LAYER:
		m_pEarth->getEarthMap()->addImageLayerProvider( ( ImageSurfaceLayer* ) pObject );//���ܶ�������ͬ��Provider����������һ��Provider�����ض��
		break;

	case VECTOR_SURFACE_LAYER:
		m_pEarth->getEarthMap()->addModelLayerProvider( ( VectorSurfaceLayer* ) pObject );
		break;

	case ELEVATION_SURFACE_LAYER:
		m_pEarth->getEarthMap()->addElevationLayerProvider( ( ElevationSurfaceLayer* ) pObject );
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case SKY:
		// do nothing.
		break;

	case FIRE:
	{
		//osgEarth::Drivers::SimpleModelOptions fireOpt;
		//osgEarthXCore::FireProvider * fire = dynamic_cast<osgEarthXCore::FireProvider *>(pObject);
		//fireOpt.node() = fire->getFireNode();
		//osgEarth::GeoPoint pnt = fire->getPosition();
		//fireOpt.location() = osg::Vec3d(pnt.x(), pnt.y(), pnt.z()+20);
		//m_pEarth->getEarthMap()->addModelLayer(new osgEarth::ModelLayer("model", fireOpt));

		m_pEarth->getEarthMap()->addFire(dynamic_cast<osgEarthXCore::FireProvider *>(pObject));
	}
		break;

	case LOCATION:
	{
		osgEarthXCore::LocationProvider *nd = dynamic_cast<osgEarthXCore::LocationProvider *>(pObject);
		m_pEarth->getEarthViewer()->getOrCreateViewer()->getDatabasePager()->registerPagedLODs(nd);
		//bool b = m_pEarth->getEarthViewer()->getOrCreateViewer()->getDatabasePager()->getDoPreCompile();
		//m_pEarth->getEarthViewer()->getOrCreateViewer()->getDatabasePager()->setDoPreCompile(true);
		m_pEarth->getEarthMap()->addLocationNode(nd);
	}
	break;

	case GRATICULE:
		// do nothing
		break;

	case VIEWPOINT:
		{
			Viewpoint* pViewpoint = ( Viewpoint* )pObject;
			pViewpoint->bindEarthManipulator( m_pEarth->getEarthViewer()->getOrCreateEarthManipulator() );
		}
		break;

	case PLACE:
		/*{
			osgEarth::Annotation::PlaceNode* nd = dynamic_cast<osgEarth::Annotation::PlaceNode*>(pObject);
			if (nd) {
				nd->setOcclusionCulling(true);
			}
		}*/
	case TEXT_LABEL:
	case MODEL:
	case FEATURE_PRIMITIVE:
	case CIRCLE:
	case IMAGE_OVERLAY:
	case RECTANGLE_PRIMITIVE:
	case ELLIPSE_PRIMITIVE:
		{
			m_pEarth->getEarthMap()->addAnnotation( dynamic_cast< osgEarth::Annotation::AnnotationNode* >( pObject ) );
		}
		break;
	case MODELSET:
		{
			osg::ref_ptr<osgEarthX::ModelSet> pModels = (osgEarthX::ModelSet*)pObject;
			pModels->initMapNode(m_pEarth->getEarthMap()->getMapNode());
			pModels->setAnnnotationEventCallback(m_pEarth->getEarthMap()->getAnnnotationEventCallback());
			osg::ref_ptr<osg::Node> nd = pModels->batchLoadingModels();
			m_pEarth->getEarthViewer()->getOrCreateViewer()->getDatabasePager()->registerPagedLODs(nd);
			m_pEarth->getEarthMap()->addAnnotations(nd);
		}
		break;
	case LABEL_SET_LAYER:
		break;

	case MODEL_SET_LAYER:
		break;

	case OVERLAY_IMAGE_SET_LAYER:
		break;

	case SHAPE_SET_LAYER:
		break;

	case PLACE_SET_LAYER:
		break;

	case VIEWPOINT_SET_LAYER:
		break;

	default:
		break;

	}
}

void EarthRootCallback::removeObject( Object*pObject )
{
	switch( pObject->getObjectType() )
	{
	case OBJECT:
		break;

	case GROUP:
		{
			Group* pGroup = ( Group* ) pObject;
			pGroup->removeCallback( m_pEarthRoot->getNotifiedGroupCallback() );

			//������ӽڵ�������
			ObjectsDynamicVector& objectsVector = pGroup->getObjects();
			for ( unsigned int i = 0, size = objectsVector.size(); i < size; i++ )
			{
				EarthRootCallback::removeObject( objectsVector[ i ] );
			}
		}

		break;

	case IMAGE_SURFACE_LAYER:
		m_pEarth->getEarthMap()->removeImageLayerProvider( ( ImageSurfaceLayer* ) pObject );
		break;

	case VECTOR_SURFACE_LAYER:
		m_pEarth->getEarthMap()->removeModelLayerProvider( ( VectorSurfaceLayer* ) pObject );
		break;

	case ELEVATION_SURFACE_LAYER:
		m_pEarth->getEarthMap()->removeElevationLayerProvider( ( ElevationSurfaceLayer* ) pObject );
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case SKY:
		{
			if ( m_pEarth->getEarthMap()->getSkyProvider() == ( osgEarthXCore::SkyProvider* ) pObject )
			{
				m_pEarth->getEarthMap()->removeSkyProvider();
			}
		}
		break;

	case FIRE:
		m_pEarth->getEarthMap()->removeFire(dynamic_cast<osgEarthXCore::FireProvider*>( pObject) );
    	break;

	case LOCATION:
		m_pEarth->getEarthMap()->removeLocationNode(dynamic_cast<osgEarthXCore::LocationProvider*>(pObject));
		break;

	case GRATICULE:
		{
			if ( m_pEarth->getEarthMap()->getGraticuleProvider() == ( osgEarthXCore::GraticuleProvider* ) pObject )
			{
				m_pEarth->getEarthMap()->removeGraticuleProvider();
			}
		}
		break;

	case TEXT_LABEL:
	case MODEL:
	case FEATURE_PRIMITIVE:
	case CIRCLE:
	case IMAGE_OVERLAY:
	case PLACE:
	case RECTANGLE_PRIMITIVE:
	case ELLIPSE_PRIMITIVE:
		{
			m_pEarth->getEarthMap()->removeAnnotation( dynamic_cast< osgEarth::Annotation::AnnotationNode* >( pObject ) );
		}
		break;

	case VIEWPOINT:
		break;

	case LABEL_SET_LAYER:
		break;

	case MODEL_SET_LAYER:
		break;

	case OVERLAY_IMAGE_SET_LAYER:
		break;

	case SHAPE_SET_LAYER:
		break;

	case PLACE_SET_LAYER:
		break;

	case VIEWPOINT_SET_LAYER:
		break;

	}
}

void EarthRootCallback::moveObject( Object*pObject )
{
	unsigned int newIndex = 0;
	switch( pObject->getObjectType() )
	{
	case OBJECT:
		break;

	case GROUP:
		{
			Group* pGroup = ( Group* ) pObject;

			//������ӽڵ�������
			ObjectsDynamicVector& objectsVector = pGroup->getObjects();
			for ( unsigned int i = 0, size = objectsVector.size(); i < size; i++ )
			{
				EarthRootCallback::moveObject( objectsVector[ i ] );
			}
		}
		break;

	case IMAGE_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject, newIndex ) )
			{
				m_pEarth->getEarthMap()->moveImageLayerProvider( ( ImageSurfaceLayer* )pObject, newIndex );
			}
		}
		break;

	case VECTOR_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject, newIndex ) )
			{
				m_pEarth->getEarthMap()->moveModelLayerProvider( ( VectorSurfaceLayer* ) pObject, newIndex );
			}
		}
		break;

	case ELEVATION_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject, newIndex ) )
			{
				m_pEarth->getEarthMap()->moveElevationLayerProvider( (ElevationSurfaceLayer*) pObject, newIndex );
			}
		}
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case SKY:
		// do nothing
		break;

	case GRATICULE:
		// do nothing
		break;

	case TEXT_LABEL:
		break;

	case MODEL:
		break;

	case IMAGE_OVERLAY:
		break;

	case FEATURE_PRIMITIVE:
		break;

	case PLACE:
		break;

	case VIEWPOINT:
		break;

	case LABEL_SET_LAYER:
		break;

	case MODEL_SET_LAYER:
		break;

	case OVERLAY_IMAGE_SET_LAYER:
		break;

	case SHAPE_SET_LAYER:
		break;

	case PLACE_SET_LAYER:
		break;

	case VIEWPOINT_SET_LAYER:
		break;

	}
}



void EarthRootCallback::insertObject(Object* pObject)
{
	unsigned int pos = 0;
	switch( pObject->getObjectType() )
	{
	case OBJECT:
		break;

	case GROUP:
		{
			Group* pGroup = ( Group* ) pObject;

			//������ӽڵ�������
			ObjectsDynamicVector& objectsVector = pGroup->getObjects();
			for ( unsigned int i = 0, size = objectsVector.size(); i < size; i++ )
			{
				EarthRootCallback::insertObject( objectsVector[ i ] );
			}
		}
		break;

	case IMAGE_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject, pos ) )
			{
				m_pEarth->getEarthMap()->insertImageLayerProvider( ( ImageSurfaceLayer* ) pObject, pos );
			}
		}
		break;

	case VECTOR_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject, pos ) )
			{
				m_pEarth->getEarthMap()->insertModelLayerProvider( ( VectorSurfaceLayer* ) pObject, pos );
			}
		}
		break;

	case ELEVATION_SURFACE_LAYER:
		{
			if ( m_pEarthRoot->countObjectsOfSameKindUntil( pObject,pos ) )
			{
				m_pEarth->getEarthMap()->insertElevationLayerProvider( ( ElevationSurfaceLayer* ) pObject, pos );
			}
		}
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case SKY:
		break;

	case TEXT_LABEL:
		break;

	case MODEL:
		break;

	case IMAGE_OVERLAY:
		break;

	case FEATURE_PRIMITIVE:
		break;

	case PLACE:
		break;

	case VIEWPOINT:
		break;

	case LABEL_SET_LAYER:
		break;

	case MODEL_SET_LAYER:
		break;

	case OVERLAY_IMAGE_SET_LAYER:
		break;

	case SHAPE_SET_LAYER:
		break;

	case PLACE_SET_LAYER:
		break;

	case VIEWPOINT_SET_LAYER:
		break;

	}
}

void EarthRootCallback::resetScene()
{
	m_pEarth->getEarthMap()->clearScene();
}