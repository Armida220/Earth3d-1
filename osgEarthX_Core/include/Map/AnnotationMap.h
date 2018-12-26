/**
	@brief Map of annotations
	@author Hao Luo
	@date 2015/03/31
*/

#ifndef OSGEARTHX_CORE_ANNOTATION_MAP_H
#define OSGEARTHX_CORE_ANNOTATION_MAP_H 1

#include <osgEarthXCoreExport/Export.h>

#include <Map/BaseMap.h>

#include <osgEarthAnnotation/AnnotationNode>
#include <osg/Group>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthUtil/Controls>
#include <Provider/AnnotationProvider/PlaceProvider.h>
#include <Provider/AnnotationProvider/LocationProvider.h>
#include <Provider/ExternalProvider/FireProvider.h>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT AnnotationMap : virtual public BaseMap
	{
	protected:
		osg::ref_ptr<osg::Group> m_refAnnotations;
		osg::ref_ptr<osg::Group> m_refVIPAnnotations;
		//osg::ref_ptr<osg::Group> m_refModels;
		osg::ref_ptr<osg::Group> m_refFeatures;
		osg::ref_ptr<osg::Group> m_refImgs;
		osg::ref_ptr< osg::Group> m_refEditors;
		osg::ref_ptr< osg::Group> m_refParticles;
		osg::ref_ptr< osg::Group> m_refBims;

		osg::ref_ptr<osgEarthXCore::PlayClickHandler::PlayClickCallback> m_tipCallback;
		osg::ref_ptr<osgEarth::Util::AnnotationEventCallback> m_refCB;
	public:
		AnnotationMap();
		osg::Group* getModels();
		osg::Group * getPlaces();
		void addAnnotation( osgEarth::Annotation::AnnotationNode* pAnnotationNode );
		void addAnnotations(osg::Node* pNode);
		void removeAnnotation( osgEarth::Annotation::AnnotationNode* pAnnotationNode );
		void removeAllAnnotations();
		osgEarth::Annotation::AnnotationNode* findAnnotation(std::string strID);
		void installPickHover(osgEarth::Util::AnnotationEventHandler* evtHandler);
		void setTipCallback(osgEarthXCore::PlayClickHandler::PlayClickCallback* cb);
		osgEarth::Util::AnnotationEventCallback* getAnnnotationEventCallback();
		//void installUpdateTipCallback(osgViewer::Viewer* vw);
		void addFire(FireProvider* pFire);
		void removeFire(FireProvider* pFire);
		void addLocationNode(LocationProvider* loc);
		void removeLocationNode(LocationProvider* loc);
		void clearScene();
		void showSingleTip(double lon,double lat,double alt,const std::string strContent);
		void clearSingleTip();
		void updateSingleTip();
		void createContent();
	protected:
		virtual ~AnnotationMap(){}

		double m_dTipLon;
		double m_dTipLat;
		double m_dTipAlt;
		std::string m_strSingleTipContent;
		osg::ref_ptr< osgEarth::Util::Controls::Control > m_singleTip;
		osg::ref_ptr<osgEarth::Util::Controls::LabelControl> m_singleTipcontent;
		osg::ref_ptr< osgEarth::Util::Controls::Grid > m_refContent;
		std::vector <std::string> m_fields;
		bool m_bContentChanged;

	private:
	};
}

#endif