#include <EarthWeb/Utils/StaticUtils/EarthObjectStaticUtils.h>

#include <EarthWeb/EarthGroup/EarthGroup.h>
#include <EarthWeb/Layer/ImageSurfaceLayer/ImageSurfaceLayer.h>
#include <EarthWeb/Layer/VectorSurfaceLayer/VectorSurfaceLayer.h>
#include <EarthWeb/Layer/ElevationSurfaceLayer/ElevationSurfaceLayer.h>

#include <EarthWeb/Externals/Graticule/Graticule.h>
#include <EarthWeb/Externals/Sky/Sky.h>
#include <EarthWeb/Externals/Fire/Fire.h>
#include <EarthWeb/Externals/Viewpoint/Viewpoint.h>

#include <EarthWeb/Annotation/ImageOverlay/ImageOverlay.h>
#include <EarthWeb/Annotation/Place/Place.h>
#include <EarthWeb/Annotation/Model/Model.h>

#include <EarthWeb\Basis\GeoData\SpatialReference\SpatialReference.h>
#include <EarthWeb/Annotation/TextLabel/TextLable.h>
#include <EarthWeb/Annotation/Location/Location.h>

#include <EarthWeb/Annotation/ModelSet/ModelSet.h>
#include <EarthWeb/Annotation/FeaturePrimitive/FeaturePrimitive.h>
#include <EarthWeb/Annotation/Circle/Circle.h>

#include <EarthWeb/Basis/GeoData/Feature/GeoFeature.h>
#include <osgEarthSymbology/Geometry>
//所有object在传参数给void*指针前都要转到Object*类型
IEarthObjectPtr EarthObjectStaticUtils::createInstance( EarthObjectClassType classType, void* innerParams /* = NULL */ )
{
	IEarthObjectPtr ptrJSAPI;

	switch ( classType )
	{
	case SPATIAL_REFERENCE:
	{
		SpatialReferencePtr ptrSpatialReference = boost::make_shared< SpatialReference >();
		ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrSpatialReference);

		if (innerParams)
		{
			ptrSpatialReference->setInnerObject((osgEarth::SpatialReference*)innerParams);
		}
	}
	break;
	case GEODATA_FEATURE:
	{
		GeoFeaturePtr ptrFeature = boost::make_shared< GeoFeature >();
		ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(ptrFeature);

		if (innerParams)
		{
			ptrFeature->setInnerObject((osgEarthXCore::Feature*)innerParams);
		}
	}
	break;

	case GEODATA_LINE:
	{
		GeoLinePtr lin = boost::make_shared< GeoLine >();
		ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(lin);

		if (innerParams)
		{
			lin->setInnerObject((osgEarth::Symbology::LineString *)innerParams);
		}
	}
	break;

	case GEODATA_POLYGON:
	{
		GeoPolygonPtr lin = boost::make_shared< GeoPolygon >();
		ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(lin);

		if (innerParams)
		{
			lin->setInnerObject((osgEarth::Symbology::Polygon *)innerParams);
		}
	}
	break;

	case EARTH_GROUP:
		{
			EarthGroupPtr ptrEarthGroup = boost::make_shared< EarthGroup >();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >( ptrEarthGroup );

			if ( innerParams )
			{
				ptrEarthGroup->setInnerObject( ( osgEarthX::Group* ) ( osgEarthX::Object* ) innerParams);
			}
		}
		break;

	case IMAGE_SURFACE_LAYER:
		{
			ImageSurfaceLayerPtr ptrImageSurfaceLayer = boost::make_shared<ImageSurfaceLayer>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrImageSurfaceLayer);

			if ( innerParams )
			{
				ptrImageSurfaceLayer->setInnerObject( ( osgEarthX::ImageSurfaceLayer* ) ( osgEarthX::Object* ) innerParams  );
			}
		}
		break;

	case VECTOR_SURFACE_LAYER:
		{
			VectorSurfaceLayerPtr ptrVectorSurfaceLayer = boost::make_shared<VectorSurfaceLayer>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrVectorSurfaceLayer);

			if ( innerParams )
			{
				ptrVectorSurfaceLayer->setInnerObject( ( osgEarthX::VectorSurfaceLayer* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case ELEVATION_SURFACE_LAYER:
		{
			ElevationSurfaceLayerPtr ptrLayer = boost::make_shared<ElevationSurfaceLayer>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrLayer);

			if ( innerParams )
			{
				ptrLayer->setInnerObject( ( osgEarthX::ElevationSurfaceLayer* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case EXTERNAL_GRATICLUE:
		{
			GraticulePtr ptrGraticule = boost::make_shared< Graticule >();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >( ptrGraticule );

			if ( innerParams )
			{
				ptrGraticule->setInnerObject( ( osgEarthX::Graticule* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case EXTERNAL_SKY:
		{
			SkyPtr ptrSky = boost::make_shared<Sky>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrSky);

			if ( innerParams )
			{
				ptrSky->setInnerObject( ( osgEarthX::Sky* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case EXTERNAL_FIRE:
	{
		FirePtr ptrSky = boost::make_shared<Fire>();
		ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrSky);

		if (innerParams)
		{
			ptrSky->setInnerObject((osgEarthX::Fire*) (osgEarthX::Object*) innerParams);
		}
	}
	break;

	case EXTERNAL_LOCATION:
	{
		LocationPtr ptrSky = boost::make_shared<Location>();
		ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrSky);

		if (innerParams)
		{
			ptrSky->setInnerObject((osgEarthX::Location*) (osgEarthX::Object*) innerParams);
		}
	}
	break;

	case ANNOTATION_TEXT_LABEL:
		{
			TextLabelPtr ptrSky = boost::make_shared<TextLabel>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrSky);

			if ( innerParams )
			{
				ptrSky->setInnerObject( ( osgEarthX::TextLabel* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case ANNOTATION_MODEL:
		{
		    ModelPtr ptrModel = boost::make_shared<Model>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrModel);

			if (innerParams)
			{
				ptrModel->setInnerObject((osgEarthX::Model*) (osgEarthX::Object*) innerParams);
			}
		}
		break;

	case ANNOTATION_MODELSET:
		{
		    ModelSetPtr ptrModel = boost::make_shared<ModelSet>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrModel);

			if (innerParams)
			{
				ptrModel->setInnerObject((osgEarthX::ModelSet*) (osgEarthX::Object*) innerParams);
			}
		}
		break;

	case ANNOTATION_IMAGE_OVERLAY:
		{
			ImageOverlayPtr ptrImageOverlay = boost::make_shared<ImageOverlay>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >( ptrImageOverlay );

			if ( innerParams )
			{
				ptrImageOverlay->setInnerObject( ( osgEarthX::ImageOverlay* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case ANNOTATION_FEATURE:
	{
		FeaturePrimitivePtr ptrPlace = boost::make_shared<FeaturePrimitive>();
		ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(ptrPlace);

		if (innerParams)
		{
			ptrPlace->setInnerObject((osgEarthX::FeaturePrimitive*) (osgEarthX::Object*) innerParams);
		}
	}
		break;

	case ANNOTATION_CIRCLE:
	{
		CirclePtr ptrPlace = boost::make_shared<Circle>();
		ptrJSAPI = boost::dynamic_pointer_cast<IEarthObject>(ptrPlace);

		if (innerParams)
		{
			ptrPlace->setInnerObject((osgEarthX::Circle*) (osgEarthX::Object*) innerParams);
		}
	}
	break;

	case ANNOTATION_PLACE:
		{
			PlacePtr ptrPlace = boost::make_shared<Place>();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >( ptrPlace );

			if ( innerParams )
			{
				ptrPlace->setInnerObject( ( osgEarthX::Place* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case EXTERNAL_VIEWPOINT:
		{
			ViewpointPtr ptrViewpoint = boost::make_shared< Viewpoint >();
			ptrJSAPI = boost::dynamic_pointer_cast< IEarthObject >(ptrViewpoint);

			if ( innerParams )
			{
				ptrViewpoint->setInnerObject( ( osgEarthX::Viewpoint* ) ( osgEarthX::Object* ) innerParams );
			}
		}
		break;

	case SHAPE_SET_LAYER:
		break;

	case PLACE_SET_LAYER:
		break;

	case VIEWPOINT_SET_LAYER:
		break;
	}

	return ptrJSAPI;
}

osgEarthX::Object* EarthObjectStaticUtils::getInnerObject( IEarthObject* pIEarthObject, bool bCreateIfNull /* = false */ )
{
	if ( pIEarthObject == NULL )
	{
		return NULL;
	}

	osgEarthX::Object* pObject = NULL;
	EarthObjectClassType classType = pIEarthObject->get_classType();

	switch ( classType )
	{
	case EARTH_GROUP:
		{
			EarthGroup* pEarthGroup = dynamic_cast< EarthGroup* >( pIEarthObject );
			pObject = bCreateIfNull ? pEarthGroup->getOrCreateInnerObject() : pEarthGroup->getInnerObject();
		}
		break;

	case IMAGE_SURFACE_LAYER:
		{
			ImageSurfaceLayer* pImageSurfaceLayer = dynamic_cast< ImageSurfaceLayer* >( pIEarthObject );
			pObject = bCreateIfNull ? pImageSurfaceLayer->getOrCreateInnerObject() : pImageSurfaceLayer->getInnerObject();
		}
		break;

	case VECTOR_SURFACE_LAYER:
		{
			VectorSurfaceLayer* pVectorSurfaceLayer = dynamic_cast< VectorSurfaceLayer* >( pIEarthObject );
			pObject = bCreateIfNull ? pVectorSurfaceLayer->getOrCreateInnerObject() : pVectorSurfaceLayer->getInnerObject();
		}
		break;

	case ELEVATION_SURFACE_LAYER:
		{
			ElevationSurfaceLayer* pElevationSurfaceLayer = dynamic_cast< ElevationSurfaceLayer* >( pIEarthObject );
			pObject = bCreateIfNull ? pElevationSurfaceLayer->getOrCreateInnerObject() : pElevationSurfaceLayer->getInnerObject();
		}
		break;

	case TERMASK_SURFACE_LAYER:
		break;

	case EXTERNAL_GRATICLUE:
		{
			Graticule* pGraticule = dynamic_cast< Graticule* >( pIEarthObject );
			pObject = bCreateIfNull ? pGraticule->getOrCreateInnerObject() : pGraticule->getInnerObject();
		}
		break;

	case EXTERNAL_SKY:
		{
			Sky* pSky = dynamic_cast< Sky* >( pIEarthObject );
			pObject = bCreateIfNull ? pSky->getOrCreateInnerObject() : pSky->getInnerObject();
		}
		break;

	case EXTERNAL_FIRE:
	{
		Fire* pSky = dynamic_cast< Fire* >(pIEarthObject);
		pObject = bCreateIfNull ? pSky->getOrCreateInnerObject() : pSky->getInnerObject();
	}
	break;

	case EXTERNAL_LOCATION:
	{
		Location* pSky = dynamic_cast< Location* >(pIEarthObject);
		pObject = bCreateIfNull ? pSky->getOrCreateInnerObject() : pSky->getInnerObject();
	}
	break;

	case ANNOTATION_TEXT_LABEL:
		{
			TextLabel* pSky = dynamic_cast< TextLabel* >( pIEarthObject );
			pObject = bCreateIfNull ? pSky->getOrCreateInnerObject() : pSky->getInnerObject();
		}
		break;
		break;

	case ANNOTATION_MODEL:
		{
			Model* pModel = dynamic_cast< Model* >(pIEarthObject);
			pObject = bCreateIfNull ? pModel->getOrCreateInnerObject() : pModel->getInnerObject();
		}
		break;

    case ANNOTATION_MODELSET:
		{
			ModelSet* pModel = dynamic_cast< ModelSet* >(pIEarthObject);
			pObject = bCreateIfNull ? pModel->getOrCreateInnerObject() : pModel->getInnerObject();
		}
		break;

	case ANNOTATION_IMAGE_OVERLAY:
		{
			ImageOverlay* pImageOverlay = dynamic_cast< ImageOverlay* >( pIEarthObject );
			pObject = bCreateIfNull ? pImageOverlay->getOrCreateInnerObject() : pImageOverlay->getInnerObject();
		}
		break;

	//case GEODATA_FEATURE:
	//{
	//	GeoFeature* pFtr = dynamic_cast<GeoFeature*>(pIEarthObject);
	//	pObject = bCreateIfNull ? pFtr->getOrCreateInnerObject() : pFtr->getInnerObject();
	//}
	//break;

	case ANNOTATION_FEATURE:
	{
		FeaturePrimitive* pftrPrim = dynamic_cast<FeaturePrimitive*>(pIEarthObject);
		pObject = bCreateIfNull ? pftrPrim->getOrCreateInnerObject() : pftrPrim->getInnerObject();
	}
	break;

	case ANNOTATION_CIRCLE:
	{
		Circle* pftrPrim = dynamic_cast<Circle*>(pIEarthObject);
		pObject = bCreateIfNull ? pftrPrim->getOrCreateInnerObject() : pftrPrim->getInnerObject();
	}
	break;

	case ANNOTATION_PLACE:
		{
			Place* pPlace = dynamic_cast< Place* >( pIEarthObject );
			pObject = bCreateIfNull ? pPlace->getOrCreateInnerObject() : pPlace->getInnerObject();
		}
		break;

	case EXTERNAL_VIEWPOINT:
		{
			Viewpoint* pViewpoint = dynamic_cast< Viewpoint* >( pIEarthObject );
			pObject = bCreateIfNull ? pViewpoint->getOrCreateInnerObject() : pViewpoint->getInnerObject();
		}
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
	return pObject;
}