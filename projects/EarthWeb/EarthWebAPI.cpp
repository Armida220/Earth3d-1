/**********************************************************\

  Auto-generated EarthWebAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "EarthWebAPI.h"

#include <EarthWeb/Utils/StaticUtils/EarthStaticUtils.h>
#include <EarthWeb/Utils/StaticUtils/EarthObjectStaticUtils.h>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ModelNode>
#include <EarthWeb/Annotation/Place/Place.h>
#include <EarthWeb/Annotation/Model/Model.h>
#include <EarthWeb\Annotation\ImageOverlay\ImageOverlay.h>

#include <Annotation/Model.h>
#include <Annotation\ImageOverlay.h>

#include <osgEarth/Registry>
//#include <osgEarth/ObjectIndex>
//#include <osgEarth/IntersectionPicker>
//#include <osgEarthUtil/RTTPicker>

#include <atlstr.h>
#include <EarthWeb/common/heatMap.h>
#include "EarthWeb/Utils/StaticUtils/StringStaticUtils.h"
#include "Annotation/FeaturePrimitive.h"
#include "EarthWeb/Annotation/FeaturePrimitive/FeaturePrimitive.h"

MyMeasureToolCallback::MyMeasureToolCallback(EarthWebAPI * obj) :_obj(obj)
{
}


void MyMeasureToolCallback::onDistanceChanged(osgEarth::Util::MeasureToolHandler* sender, double distance)
{
	_obj->fire_measure(distance, 0);
}

//struct PlayClickHandler : public osgEarthXCore::TipClickHandler
//{
//	PlayClickHandler::PlayClickHandler(EarthWebAPI * obj): osgEarthXCore::TipClickHandler(NULL),_obj(obj)
//	{
//	}
//	void onClick(osgEarth::Util::Controls::Control* control, const osg::Vec2f& pos, int mouseButtonMask)
//	{
//		osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(_place.get());
//		if (place != NULL)
//		{
//			PlacePtr ptrPlace = boost::make_shared<Place>();
//			ptrPlace->setInnerObject(place);
//			_obj->fire_tipClicked(ptrPlace,0,0);
//			//anno->setDecoration("hover");			
//		}
//	}
//private:
//	EarthWebAPI* _obj;
//};

struct MyPlayClickCallback : public osgEarthXCore::PlayClickHandler::PlayClickCallback
{
	MyPlayClickCallback::MyPlayClickCallback(EarthWebAPI * obj) : _obj(obj)
	{
	}
	// called when valid map coordinates are found under the mouse
	virtual void playClicked(osgEarthXCore::PlaceProvider* obj,const std::string &strID) {
		osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(obj);
		if (place != NULL)
		{
			PlacePtr ptrPlace = boost::make_shared<Place>();
			ptrPlace->setInnerObject(place);
			_obj->fire_tipClicked(ptrPlace, (std::wstring)CString(strID.data()), 0);
			//anno->setDecoration("hover");			
		}
	}

	virtual ~MyPlayClickCallback() { }

private:
	EarthWebAPI* _obj;
};

struct MyMouseCoordsCallback : public MouseCoordsToolX::MouseCoordsCallback
{
	// called when valid map coordinates are found under the mouse
	MyMouseCoordsCallback::MyMouseCoordsCallback(EarthWebAPI * obj) :_obj(obj)
	{

	}

	void mouseMove(const osgEarth::GeoPoint& coords)
	{
		_obj->fire_mouseMove(coords.x(), coords.y(), coords.z());
	}

	void mouseDown(const osgEarth::GeoPoint& coords)
	{
		_obj->fire_mouseDown(coords.x(), coords.y(), coords.z());
	}

	void mouseUp(const osgEarth::GeoPoint& coords, const std::string strUp)
	{
		_obj->fire_mouseUp(coords.x(), coords.y(), coords.z(), (std::wstring)CString(strUp.data()));
	}

	void mouseDoubleClick(const osgEarth::GeoPoint& coords)
	{
		_obj->fire_mouseDoubleClick(coords.x(), coords.y(), coords.z());
	}
private:
	EarthWebAPI* _obj;
};

struct MyAnnoEventHandler : public osgEarth::Util::AnnotationEventHandler
{
	MyAnnoEventHandler::MyAnnoEventHandler(EarthWebAPI * obj) :_obj(obj)
	{
	}

	void onHoverEnter(osgEarth::Annotation::AnnotationNode* anno, const EventArgs& args)
	{

		osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(anno);
		if (place != NULL)
		{
		    PlacePtr ptrPlace = boost::make_shared<Place>();
			ptrPlace->setInnerObject(place);
			_obj->fire_hoverEnter(ptrPlace);
		    //anno->setDecoration("hover");			
		}
	}

	void onHoverLeave(osgEarth::Annotation::AnnotationNode* anno, const EventArgs& args)
	{
		
		osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(anno);
		if (place != NULL)
		{
			//anno->clearDecoration();
			PlacePtr ptrPlace = boost::make_shared<Place>();
			ptrPlace->setInnerObject(place);
			_obj->fire_hoverLeave(ptrPlace);
		}
	}

	virtual void onClick(osgEarth::Annotation::AnnotationNode* node, const EventArgs& details)
	{
		osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(node);
		if (place == NULL)
		{
			//osgEarth::Annotation::ModelNode* mdl = dynamic_cast<osgEarth::Annotation::ModelNode*>(node);
			osgEarthX::Model* mdl = dynamic_cast<osgEarthX::Model*>(node);
			if (mdl != NULL) {
				//IEarthObjectPtr ptrObj = EarthObjectStaticUtils::createInstance(ANNOTATION_MODEL, node);
				ModelPtr ptrModel = boost::make_shared<Model>();
				ptrModel->setInnerObject(dynamic_cast<osgEarthX::Model*>(mdl));
				IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast< IEarthObject >(ptrModel);
				_obj->fire_featureClicked(ptrObj);
			}
			else {
				osgEarthX::ImageOverlay* img = dynamic_cast<osgEarthX::ImageOverlay*>(node);
				if(img){
					ImageOverlayPtr ptrImg = boost::make_shared<ImageOverlay>();
					ptrImg->setInnerObject(dynamic_cast<osgEarthX::ImageOverlay*>(img));
					IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast< IEarthObject >(ptrImg);
					_obj->fire_featureClicked(ptrObj);
				} else
				{
					osgEarthX::FeaturePrimitive* ftr = dynamic_cast<osgEarthX::FeaturePrimitive*>(node);
					if (ftr){
						FeaturePrimitivePtr ptrFtr = boost::make_shared<FeaturePrimitive>();
						ptrFtr->setInnerObject(dynamic_cast<osgEarthX::FeaturePrimitive*>(ftr));
						IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast< IEarthObject >(ptrFtr);
						_obj->fire_featureClicked(ptrObj);
					}
				}
			}
			OE_WARN << "Thanks for clicking this annotation" << std::endl;
		}
		else
		{
			//IEarthObjectPtr ptrObj = EarthObjectStaticUtils::createInstance(ANNOTATION_PLACE, node);
			PlacePtr ptrPlace = boost::make_shared<Place>();
			ptrPlace->setInnerObject(dynamic_cast<osgEarthX::Place*>(place));
			//_obj->fire_featureClicked(ptrPlace);
			IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast< IEarthObject >(ptrPlace);
			_obj->fire_featureClicked(ptrObj);
			OE_NOTICE << "Thanks for clicking the PlaceNode: " << place->getText() << std::endl;
		}
	}
private:
	EarthWebAPI* _obj;
};

// Tests the (old) intersection-based picker.
//struct TestIsectPicker : public osgGA::GUIEventHandler
//{
//	TestIsectPicker::TestIsectPicker(EarthWebAPI * obj) :_obj(obj)
//	{
//	}
//	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//	{
//		if (ea.getEventType() == ea.RELEASE)
//		{
//			osgEarth::IntersectionPicker picker(dynamic_cast<osgViewer::View*>(aa.asView()));
//			osgEarth::IntersectionPicker::Hits hits;
//			if (picker.pick(ea.getX(), ea.getY(), hits)) {
//				std::set<osgEarth::ObjectID> oids;
//				if (picker.getObjectIDs(hits, oids)) {
//					osgEarth::ObjectIndex* index = osgEarth::Registry::objectIndex();
//					osgEarth::ObjectID oid = *oids.begin();
//					/*osg::ref_ptr<FeatureIndex> fi = index->get<FeatureIndex>(oid);
//					if (fi.valid()) {
//						OE_NOTICE << "IsectPicker: found OID " << oid << "\n";
//						Feature* f = fi->getFeature(oid);
//						if (f) {
//							OE_NOTICE << "...feature ID = " << f->getFID() << "\n";
//						}
//					}
//					osg::ref_ptr<Feature> f = index->get<Feature>(oid);
//					if (f.valid()) {
//						OE_NOTICE << "IsectPicker: found OID " << oid << "\n";
//						OE_NOTICE << "...feature ID = " << f->getFID() << "\n";
//					}*/
//					osg::ref_ptr<osgEarth::Annotation::AnnotationNode> a = index->get<osgEarth::Annotation::AnnotationNode>(oid);
//					if (a.valid()) {
//						osgEarthX::Place* place = dynamic_cast<osgEarthX::Place*>(a.get());
//						if (place == NULL)
//						{
//							//osgEarth::Annotation::ModelNode* mdl = dynamic_cast<osgEarth::Annotation::ModelNode*>(node);
//							osgEarthX::Model* mdl = dynamic_cast<osgEarthX::Model*>(a.get());
//							if (mdl != NULL) {
//								//IEarthObjectPtr ptrObj = EarthObjectStaticUtils::createInstance(ANNOTATION_MODEL, node);
//								ModelPtr ptrModel = boost::make_shared<Model>();
//								ptrModel->setInnerObject(dynamic_cast<osgEarthX::Model*>(mdl));
//								IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast<IEarthObject>(ptrModel);
//								_obj->fire_featureClicked(ptrObj);
//							}
//							OE_NOTICE << "Thanks for clicking this annotation" << std::endl;
//						}
//						else
//						{
//							//IEarthObjectPtr ptrObj = EarthObjectStaticUtils::createInstance(ANNOTATION_PLACE, node);
//							PlacePtr ptrPlace = boost::make_shared<Place>();
//							ptrPlace->setInnerObject(dynamic_cast<osgEarthX::Place*>(place));
//							//_obj->fire_featureClicked(ptrPlace);
//							IEarthObjectPtr ptrObj = boost::dynamic_pointer_cast<IEarthObject>(ptrPlace);
//							_obj->fire_featureClicked(ptrObj);
//							OE_NOTICE << "Thanks for clicking the PlaceNode: " << place->getText() << std::endl;
//						}
//						OE_NOTICE << "IsectPicker: found annotation " << a->getName() << "\n";
//					}
//				}
//				else {
//					OE_NOTICE << "IsectPicker: picked, but no OIDs\n";
//				}
//			}
//			else {
//				OE_NOTICE << "IsectPicker: no intersect\n";
//			}
//		}
//		return false;
//	}
//private:
//	EarthWebAPI* _obj;
//};

/**
* Callback that you install on the RTTPicker.
*/
//struct MyPickCallback : public osgEarth::Util::RTTPicker::Callback
//{
//	void onHit(osgEarth::ObjectID id)
//	{
//		// Check whether it's an annotation:
//		osgEarth::Annotation::AnnotationNode* anno = osgEarth::Registry::objectIndex()->get<osgEarth::Annotation::AnnotationNode>(id);
//		if (anno)
//		{
//		}
//
//	}
//
//	void onMiss()
//	{
//
//	}
//
//	// pick whenever the mouse moves.
//	bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
//	{
//		return ea.getEventType() == ea.MOVE;
//	}
//};
///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant EarthWebAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant EarthWebAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn EarthWebPtr EarthWebAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
EarthWebPtr EarthWebAPI::getPlugin()
{
    EarthWebPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string EarthWebAPI::get_testString()
{
    return m_testString;
}

void EarthWebAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string EarthWebAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void EarthWebAPI::setOperation(const int oper)
{
	if (oper == 0) {
		MyMeasureToolHandler* pTool = m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateMeasureTool();
		if (pTool != NULL) {
			//pTool->setIgnoreHandledEventsMask(true);
			pTool->setValid(false);
			pTool->clearAll();
		}
	}
	else if (oper == 1) {
		MyMeasureToolHandler* pTool = m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateMeasureTool();
		if (m_measureCallback == NULL) {
			m_measureCallback = new MyMeasureToolCallback(this);
			pTool->addEventHandler(m_measureCallback);
		}
		pTool->setValid(true);
		pTool->setIsArea(false);
	}
	else if (oper == 2) {
		MyMeasureToolHandler* pTool = m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateMeasureTool();
		if (m_measureCallback == NULL) {
			m_measureCallback = new MyMeasureToolCallback(this);
			pTool->addEventHandler(m_measureCallback);
		}
		pTool->setValid(true);
		pTool->setIsArea(true);
	}
}


void EarthWebAPI::setFocus()
{
	HWND hwnd = m_plugin.lock()->getHWND();
	SetFocus(hwnd);
}

bool EarthWebAPI::equal( IEarthDispatchPtr ptr )
{
	return false;
}

void EarthWebAPI::run()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->installPickHover(new MyAnnoEventHandler(this));
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setTipCallback(new MyPlayClickCallback(this));
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->installMouseCoordsToolCallBack(new MyMouseCoordsCallback(this));
	//m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateViewer()->addEventHandler(new TestIsectPicker(this));
	// create a picker of the specified size.
	//osgEarth::Util::RTTPicker* picker = new osgEarth::Util::RTTPicker();
	//m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateViewer()->addEventHandler(picker);

	// add the graph that will be picked.
	//picker->addChild(m_plugin.lock()->getOrCreateInnerObject()->getEarthMap() ->getPlaces());

	// install a callback that controls the picker and listens for hits.
	//picker->setDefaultCallback(new MyPickCallback());
	m_plugin.lock()->getOrCreateInnerObject()->run();
}

FB::JSAPIPtr EarthWebAPI::getClassFactory()
{
	return EarthStaticUtils::createInstance( CLASS_FACTORY );
}

FB::JSAPIPtr EarthWebAPI::getEarthMap()
{
	return EarthStaticUtils::createInstance( EARTH_MAP, m_plugin.lock()->getOrCreateInnerObject()->getEarthMap() );
}

FB::JSAPIPtr EarthWebAPI::getEarthRoot()
{
	return EarthObjectStaticUtils::createInstance( EARTH_GROUP, ( osgEarthX::Group* ) m_plugin.lock()->getOrCreateInnerObject()->getEarthRoot() );
}

FB::JSAPIPtr EarthWebAPI::getViewPoint()
{
	return EarthObjectStaticUtils::createInstance( EXTERNAL_VIEWPOINT, ( osgEarthX::Viewpoint* ) m_plugin.lock()->getOrCreateInnerObject()->getViewpoint() );
}

void EarthWebAPI::setActiveSky( SkyPtr ptrSky )
{
	if ( ptrSky.get() == NULL )
	{
		return;
	}
	m_plugin.lock()->getOrCreateInnerObject()->setActiveSky( dynamic_cast< osgEarthX::Sky* >( EarthObjectStaticUtils::getInnerObject( ptrSky.get(), true ) ) );
}

void EarthWebAPI::removeActiveSky()
{
	m_plugin.lock()->getOrCreateInnerObject()->removeActiveSky();
}

void EarthWebAPI::setHomeViewpoint( ViewpointPtr ptrViewpoint )
{
	if ( ptrViewpoint.get() == NULL )
	{
		return;
	}

	m_plugin.lock()->getOrCreateInnerObject()->setHomeViewpoint( dynamic_cast< osgEarthX::Viewpoint* >( EarthObjectStaticUtils::getInnerObject( ptrViewpoint.get(), true ) ) );
}

void EarthWebAPI::setActiveGraticule( GraticulePtr ptrGraticule )
{
	if ( ptrGraticule.get() == NULL )
	{
		return;
	}
	m_plugin.lock()->getOrCreateInnerObject()->setActiveGraticule( dynamic_cast< osgEarthX::Graticule* >( EarthObjectStaticUtils::getInnerObject( ptrGraticule.get(), true ) ) );
}

void EarthWebAPI::removeActiveGraticule()
{
	m_plugin.lock()->getOrCreateInnerObject()->removeActiveGraticule();
}

void EarthWebAPI::beginFly()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->beginFly();
}

void EarthWebAPI::stopFly()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->stopFly();
}

void EarthWebAPI::beginRotate(double dlon, double dlat, double dele, double drange)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->beginRotate(dlon, dlat, dele, drange);
}

void EarthWebAPI::beginFocusRotate(double dlon, double dlat, double dele, double dmaxRange, double dminRange)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->beginRotateEx(dlon, dlat, dele, dmaxRange, dminRange, 0.0, 0.0);
}

void EarthWebAPI::beginPitchRotate(double dlon, double dlat, double dele, double dRange, double dmaxPitch, double dminPitch)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->beginRotateEx(dlon, dlat, dele, dRange, dRange, dmaxPitch, dminPitch);
}

void EarthWebAPI::stopRotate()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->stopRotate();
}

void EarthWebAPI::print(std::wstring filePath)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->print(CStringA(filePath.data()).GetString());
}

void EarthWebAPI::clearScene()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->clearScene();
}

void EarthWebAPI::addMask(std::vector<double>& cords,double h,bool bOpacity)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setMaskLayer(cords, h, bOpacity);
}

void EarthWebAPI::removeMask()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->removeMaskLayer();
}

void EarthWebAPI::fullScreen(bool bfull)
{
	/*HWND hwnd = m_plugin.lock()->getHWND();
	::SetParent(hwnd, ::GetDesktopWindow());

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, cx, cy, SWP_FRAMECHANGED | SWP_DEFERERASE);*/
}

void EarthWebAPI::beginFlybyline(GeoLinePtr lin, double dEvl, double dHitch, double dSpeed)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->beginFlybyline(lin->getInnerObject(), dEvl, dHitch, dSpeed);
}

void EarthWebAPI::stopFlybyline()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->stopFlybyline();
}

std::vector<double> EarthWebAPI::heatMap(std::string pstrFileName, int opacity, std::vector<double> &LonLat)
{
	heatMapImp(pstrFileName.c_str(), opacity, LonLat);
	std::vector<double> res;
	res.push_back(leftDown.x);
	res.push_back(leftDown.y);
	res.push_back(rightUp.x);
	res.push_back(rightUp.y);
	return res;
}

double EarthWebAPI::getLocationZ(double dlon, double dlat)
{
	osgEarth::MapNode* mapNode = m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->getMapNode();
	osgEarth::GeoPoint mapStart(
		mapNode->getMapSRS(), dlon, dlat, 100);
	osg::Vec3d worldStart;
	mapStart.toWorld(worldStart);
	osg::Vec3d worldEnd;
	osgEarth::GeoPoint mapEnd(
		mapNode->getMapSRS(), dlon, dlat, 0);
	mapEnd.toWorld(worldEnd);

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(worldStart, worldEnd);
	osgUtil::IntersectionVisitor iv(intersector.get());
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateViewer()->getCamera()->accept(iv);

	if (intersector->containsIntersections())
	{
		osgEarth::GeoPoint map;
		osg::Vec3d  v = intersector->getIntersections().begin()->getWorldIntersectPoint();
		map.fromWorld(mapNode->getMapSRS(), v);
		return map.z();
	}
	else
		return 0.0;
}

void EarthWebAPI::startViewshed(double dlon, double dlat, double dAlt, double dHead, double dPitch,double dRange)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->UpdateShadowCaster(osg::Vec3d(dlon,dlat,dAlt), dHead, dPitch, dRange);
}

void EarthWebAPI::stopViewshed()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->stopViewShed();
}

std::vector<double> EarthWebAPI::getScreenXY(double x, double y, double z)
{
	osgEarth::MapNode* mapNode = m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->getMapNode();
	osgEarth::GeoPoint pnt(
		mapNode->getMapSRS(), x, y, z);

	osg::Vec3d vWorld;
	pnt.toWorld(vWorld);
	osg::Camera* cam = m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->getOrCreateViewer()->getCamera();
	osg::Matrix VPW = cam->getViewMatrix() *
		cam->getProjectionMatrix() *
		cam->getViewport()->computeWindowMatrix();
	osg::Vec3d vWin = vWorld * VPW;
	float wy = cam->getViewport()->height() - vWin.y();
    float wx = vWin.x();
	std::vector<double> res;
	res.push_back(wx);
	res.push_back(wy);
	return res;
}

void EarthWebAPI::setFog(bool bFog)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setFog(bFog);
}

void EarthWebAPI::setRain(bool bRain)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setRain(bRain);
}

void EarthWebAPI::setSnow(bool bSnow)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setSnow(bSnow);
}

void EarthWebAPI::setTerrainEffect(bool bEffect)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->setTerrainEffect(bEffect);
}

void EarthWebAPI::LineOfSight(double dlon, double dlat, double dAlt, double dlonEnd, double dlatEnd, double dAltEnd)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->LineOfSight(dlon, dlat, dAlt, dlonEnd, dlatEnd, dAltEnd);
}

void EarthWebAPI::RadialLineOfSight(double dlon, double dlat, double dAlt, double dr)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->RadialLineOfSight(dlon, dlat, dAlt, dr);
}

void EarthWebAPI::TerrainProfile(double dlon, double dlat, double dlonEnd, double dlatEnd, bool bShow)
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->TerrainProfile(dlon,  dlat,  dlonEnd,  dlatEnd,  bShow);
}

void EarthWebAPI::ClearCalResult()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->ClearCalResult();
}

void EarthWebAPI::showTip(double lon, double lat, double alt, const std::wstring& strContent)
{
	std::string stra = StringStaticUtils::UrlDecode(CStringA(strContent.data()).GetString());
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->showSingleTip(lon, lat, alt, stra);
}

void EarthWebAPI::clearTip()
{
	m_plugin.lock()->getOrCreateInnerObject()->getEarthMap()->clearSingleTip();
}

void EarthWebAPI::setSelectColor(const std::wstring& col, bool bCol)
{
	std::string strCol = CStringA(col.data()).GetString();
	m_plugin.lock()->getOrCreateInnerObject()->getEarthViewer()->setSelectedColor(osgEarth::Symbology::Color(strCol), bCol);
}