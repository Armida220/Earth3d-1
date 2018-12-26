#include <Viewer/MapViewer.h>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
using namespace osgEarthXCore;

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/Pickers>
//#include <osgGA/StateSetManipulator>

//OSGX
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>

#include <Map/BaseMap.h>
#include "../../include/Map/AnnotationMap.h"
#include <osgEarthAnnotation/ModelNode>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/TerrainManipulator>
#include "WalkManipulator.h"
#include "Compass"

bool FlyToViewpointHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (_bPlay) {
		osgEarth::Util::Viewpoint vp = _manip->getViewpoint();
		if (!_bStart) {
			//get eye point as geographic coordinate
			osg::View* v = aa.asView();
			osg::Camera* cam = v->getCamera();
			osg::Vec3 vPosEye, vCenter, vUp;
			cam->getViewMatrixAsLookAt(vPosEye, vCenter, vUp,vp.getRange());//获取世界坐标系下的视点值
			double dlon, dlat, dele;
			_mapNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(vPosEye.x(), vPosEye.y(), vPosEye.z(), dlat, dlon, dele);

			dlon = osg::RadiansToDegrees(dlon);
			dlat = osg::RadiansToDegrees(dlat);
			_dEle = dele;
			vp.setFocalPoint(osg::Vec3d(dlon, dlat, _dEle));
			
			_bStart = true;

			_dSp = dSpeed*1000.0 / 3600.0; //换算成米每秒
			_dSp = _dSp / 50;//如果帧率是50，米每帧
			_dSp = _dSp * 360 / 40000000;//度每帧

			double dHead = vp.getHeading();
			_dSin = sin(dHead* osg::PI / 180);
			_dCos = cos(dHead* osg::PI / 180);
			//return false;
		}
		
		double dHead = vp.getHeading();
		double dPitch = vp.getPitch();
		vp.setRange(100);
		//vp.setPitch(-30);
		//vp.z() = vp.z() + sp*sin(dPitch* osg::PI / 180);
		if (ea.getEventType() == ea.KEYDOWN && (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down || ea.getKey() == osgGA::GUIEventAdapter::KEY_Up))
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down) {
				_dEle -= 10;
			}
			else {
				_dEle += 10;
			}
		}

		if (ea.getEventType() == ea.KEYDOWN && (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left || ea.getKey() == osgGA::GUIEventAdapter::KEY_Right))
		{
			double dHead = vp.getHeading();
			_dSin = sin(dHead* osg::PI / 180);
			_dCos = cos(dHead* osg::PI / 180);
		}

		vp.z() = _dEle;
		vp.x() = vp.x() + _dSp*_dSin;
		vp.y() = vp.y() + _dSp*_dCos;
		//vp.x() = vp.x() + _dSp*sin(dHead* osg::PI / 180);
		//vp.y() = vp.y() + _dSp*cos(dHead* osg::PI / 180);
		_manip->setViewpoint(vp);
	}
	return false;
}

bool FlybylineHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (_bPlay) {
		if (_curIndex >= _line->size()-1) {
			//_bPlay = false;
			//return false;
			_curIndex = 0;
		}

		osgEarth::Util::Viewpoint vp;
		double dHead = _dHeads[_curIndex];

		vp.setRange(100);
		vp.setPitch(_dHitch);
		vp.setHeading(osg::RadiansToDegrees(dHead));
		vp.z() = _dEle;
		vp.x() = _line.get()->at(_curIndex).x() + _dLength*_dsin[_curIndex];
		vp.y() = _line.get()->at(_curIndex).y() + _dLength*_dcos[_curIndex];

		if ((vp.x() - _line.get()->at(_curIndex + 1).x())*(_line.get()->at(_curIndex).x() - _line.get()->at(_curIndex + 1).x()) < 0 ||
			(vp.y() - _line.get()->at(_curIndex + 1).y())*(_line.get()->at(_curIndex).y() - _line.get()->at(_curIndex + 1).y()) < 0)
		{
			vp.x() = _line.get()->at(_curIndex + 1).x();
			vp.y() = _line.get()->at(_curIndex + 1).y();
			vp.setHeading(osg::RadiansToDegrees(_dHeads[_curIndex+1]));
			_manip->setViewpoint(vp,0.5);
			_dLength = _dGap;
			_curIndex++;
		}
		else {
			_manip->setViewpoint(vp);
			_dLength += _dGap;
		}
	}
	return false;
}

void RotateByViewpointHandler::init(double dlon, double dlat, double dele, double dRadius)
{
	_dlon = dlon;
	_dlat = dlat;
	_dele = dele;
	_dRadius = dRadius;
	osgEarth::Util::Viewpoint vp;
	vp.setRange(dRadius);
	vp.setPitch(-5.0);
	vp.setHeading(0.0);
	vp.z() = dele;
	vp.x() = dlon;
	vp.y() = dlat;
	_dHead = 0.0;
	_dPitch = 0.0;
	_dRadius = 0.0;
	_manip->setViewpoint(vp);
	_startFrameTick = osg::Timer::instance()->tick();
}

void RotateByViewpointHandler::init(double dlon, double dlat, double dele, double dMaxRadius,double dMinRadius,double dMaxPitch,double dMinPitch)
{
	_dlon = dlon;
	_dlat = dlat;
	_dele = dele;
	osgEarth::Util::Viewpoint vp;
	vp.setRange(dMaxRadius);
	vp.setPitch(dMaxPitch);
	vp.setHeading(0.0);
	vp.z() = dele;
	vp.x() = dlon;
	vp.y() = dlat;
	_dHead = 0.0;
	_manip->setViewpoint(vp);
	_startFrameTick = osg::Timer::instance()->tick();
	_dRadius = (dMaxRadius - dMinRadius) / 360.0;
	_dPitch = (dMaxPitch - dMinPitch) / 360.0;
}


bool RotateByViewpointHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (_bPlay) {
		if (_dHead>360.0)
		{
			_bPlay = false;
			return false;
		}
		osg::Timer_t endFrameTick = osg::Timer::instance()->tick();
		double frameTime = osg::Timer::instance()->delta_s(_startFrameTick, endFrameTick)*1000;
		if (frameTime < 50) return false;
		_startFrameTick = endFrameTick;
		osgEarth::Util::Viewpoint vp = _manip->getViewpoint();
		if (!_bStart)
		{
			_bStart = true;
		}
		vp.setHeading(vp.getHeading()+0.5);

		if (_dHead<180.0)
		{
			if (fabs(_dRadius)>0.000001)
				vp.setRange(vp.getRange() - _dRadius);
			if (fabs(_dPitch)>0.000001)
				vp.setPitch(vp.getPitch() - _dPitch);

		} else
		{
			if (fabs(_dRadius)>0.000001)
				vp.setRange(vp.getRange() + _dRadius);
			if (fabs(_dPitch)>0.000001)
				vp.setPitch(vp.getPitch() + _dPitch);
		}
		_manip->setViewpoint(vp);
		_dHead += 0.5;
	}
	return false;
}

MouseCoordsToolX::MouseCoordsToolX(osgEarth::MapNode* mapNode, osg::Node* nd) :_places(nd), _mapNode(mapNode), m_bSelectedCol(true)
{
	_mapNodePath.push_back(mapNode->getTerrainEngine());
	m_material_selected = new osg::Material();
	m_material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 0.98f, 0.2f, 0.5f));
	m_material_selected->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 0.85f, 0.2f, 1.0f));
	m_material_selected->setShininess(osg::Material::FRONT_AND_BACK, 35.0);
	m_material_selected->setColorMode(osg::Material::SPECULAR);
}

void MouseCoordsToolX::setSelectedColor(osg::Vec4f col, bool bCol)
{
	m_bSelectedCol = bCol;
	if (bCol){
		m_material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, col);
		m_material_selected->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(col.r(), osg::clampAbove(col.g()-0.13,0.0), col.b(), 1.0f));
		m_material_selected->setShininess(osg::Material::FRONT_AND_BACK, 35.0);
		m_material_selected->setColorMode(osg::Material::SPECULAR);
	}
}

void
MouseCoordsToolX::addCallback(MouseCoordsToolX::MouseCoordsCallback* cb)
{
	_callbacks.push_back(cb);
}

bool
MouseCoordsToolX::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osg::Vec3d world;
	if (_mapNode->getTerrain()->getWorldCoordsUnderMouse(aa.asView(), ea.getX(), ea.getY(), world))
	{
		osgEarth::GeoPoint map;
		map.fromWorld(_mapNode->getMapSRS(), world);
		if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->mouseDown(map);

		}
		else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			osgViewer::View* view = static_cast<osgViewer::View*>(&aa);

			osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
				new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
			osgUtil::IntersectionVisitor iv(intersector.get());
			//iv.setTraversalMask(~0x1);
			//unsigned int iOld = _places->getNodeMask();
			//_places->setNodeMask(0x1);
			view->getCamera()->accept(iv);
			//_places->setNodeMask(iOld);
			std::string strUp = "Null";
			if (intersector->containsIntersections())
			{
				//osg::Vec3d  v  = intersector->getIntersections().begin()->getWorldIntersectPoint();
				//map.fromWorld(_mapNode->getMapSRS(), v);

				for (osgUtil::LineSegmentIntersector::Intersections::iterator h = intersector->getIntersections().begin(); h != intersector->getIntersections().end(); ++h)
				{
					bool isPlace = false;
					for (int i = 0; i < h->nodePath.size(); i++)
					{
						PlaceProvider* pla = dynamic_cast<PlaceProvider*>(h->nodePath[i]);
						if (pla) {
							isPlace = true;
							break;
						}
					}

					if (isPlace) {
						continue;
					}
					osg::Vec3d  v = h->getWorldIntersectPoint();
					map.fromWorld(_mapNode->getMapSRS(), v);

					osgUtil::LineSegmentIntersector::Intersection intersection = *h;
					{
						osg::NodePath& nodePath = intersection.nodePath;

						if (std::find(nodePath.begin(), nodePath.end(), _mapNode) != nodePath.end())
							continue;

						for (size_t i = 0; i < nodePath.size(); ++i)
						{
							osg::Node* node = nodePath[nodePath.size() - i - 1];
							strUp = node->getName();

							// check if picked object is a representation of an IfcProduct
							if (strUp.length() == 0)
							{
								continue;
							}
							if (strUp.compare(0, 9, "intersect") == 0)
							{
								continue;
							}

							if (strUp.find("::") != std::string::npos)
							{
								continue;
							}

							/*if (strUp.find(":") == std::string::npos)
								{
									continue;
								}*/

							osg::Geode* group = dynamic_cast<osg::Geode*>(node);
							if (!group)
							{
								continue;
							}

							osg::Node* tmpNd = group;
							while (tmpNd->getParent(0) != 0)
							{
								tmpNd = tmpNd->getParent(0);
								osg::PagedLOD* pg = dynamic_cast<osg::PagedLOD*>(tmpNd);
								if (pg)
								{
									std::string strtmp = strUp;
									strUp = pg->getName() + "-";
									strUp += strtmp;
									break;
								}
							}

							if (m_selGroup.valid()) {
								if (m_material_previous.valid()) {
									osg::ref_ptr<osg::StateSet> stateset = m_selGroup->getOrCreateStateSet();
									stateset->setAttribute(m_material_previous, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
								}
								else {
									osg::ref_ptr<osg::StateSet> stateset = m_selGroup->getOrCreateStateSet();
									stateset->removeAttribute(m_material_selected);
								}
							}

							m_selGroup = group;

							if (m_bSelectedCol){
								osg::ref_ptr<osg::StateSet> stateset = group->getOrCreateStateSet();
								osg::Material* material_previous = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
								if (material_previous)
								{
									m_material_previous = material_previous;
								}
								stateset->setAttribute(m_material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

								stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
								stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
								stateset->setRenderBinDetails(11, "DepthSortedBin");
							}
							break;
						}
					}
					//osg::Vec3d  v = h->getWorldIntersectPoint();

					//map.fromWorld(_mapNode->getMapSRS(), v);

					break;
				}
			}

			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i){
				//std::string str = strUp.substr(0,strUp.find(":"));
				i->get()->mouseUp(map, strUp);
			}
			
		}
		else if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG)
		{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->mouseMove(map);
		}
		else if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->mouseDoubleClick(map);

		}
	}

	return false;
}

void MyMeasureToolHandler::toggleFullscreen(osgViewer::GraphicsWindow *window)
{
	osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (wsi == NULL)
	{
		OSG_NOTICE << "Error, no WindowSystemInterface available, cannot toggle window fullscreen." << std::endl;
		return;
	}

	osgViewer::GraphicsWindowWin32::WindowData* pWindowData = dynamic_cast<osgViewer::GraphicsWindowWin32::WindowData*>(window->getTraits()->inheritedWindowData.get());

	if (!pWindowData) return;
	HWND hwnd = pWindowData->_hwnd;
	
	unsigned int    screenWidth;
	unsigned int    screenHeight;

	wsi->getScreenResolution(*(window->getTraits()), screenWidth, screenHeight);

	int x;
	int y;
	int width;
	int height;

	window->getWindowRectangle(x, y, width, height);

	bool    isFullScreen = x == 0 && y == 0 && width == (int)screenWidth && height == (int)screenHeight;

	if (isFullScreen)
	{
		//window->setWindowDecoration(true);
		::SetParent(hwnd, _hParent);

		//MoveWindow使用相对窗口坐标，所以必须先转化为相对坐标
		POINT LeftTop;
		LeftTop.x = _x;
		LeftTop.y = _y;
		::ScreenToClient(hwnd, &LeftTop);
		//::ScreenToClient(hwnd, &BottomRight);

		//::MoveWindow(hwnd, LeftTop.x, LeftTop.y, _width, _height, TRUE);
		//::SetWindowPos(m_videoMgr.GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		window->setWindowRectangle(LeftTop.x, LeftTop.y, _width, _height);
	}
	else
	{
		if (_x == 0 && _y == 0 && _width == 0 && _height == 0)
		{
			_x = x;
			_y = y;
			_width = width;
			_height = height;
		}
		window->setWindowDecoration(false);
		//window->setWindowRectangle(0, 0, screenWidth, screenHeight); lijp modified

		//lijp added 2018/12/04
		_hParent = ::SetParent(hwnd, ::GetDesktopWindow());
		window->setWindowRectangle(0, 0, screenWidth, screenHeight);
		//::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_DEFERERASE);
	}
	window->grabFocusIfPointerInWindow();
}

bool MyMeasureToolHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return false;

	osgViewer::ViewerBase* viewer = view->getViewerBase();

	if (viewer == NULL)
	{
		return false;
	}

	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYUP)
	{
		if (ea.getKey() == 'm')
		{

			// sleep to allow any viewer rendering threads to complete before we
			// resize the window
			OpenThreads::Thread::microSleep(100000);

			osgViewer::Viewer::Windows windows;
			viewer->getWindows(windows);
			for (osgViewer::Viewer::Windows::iterator itr = windows.begin();
				itr != windows.end();
				++itr)
			{
				toggleFullscreen(*itr);
			}

			aa.requestRedraw();
			return true;
		}
	}

	if (_bValid) {
		if (ea.getHandled())
		{
			return false;
		}


		osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
		if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == _mouseButton)
		{
			_mouseDown = true;
			_mouseDownX = ea.getX();
			_mouseDownY = ea.getY();
		}
		else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == _mouseButton)
		{
			float eps = 1.0f;
			_mouseDown = false;
			if (osg::equivalent(ea.getX(), _mouseDownX) && osg::equivalent(ea.getY(), _mouseDownY,eps))
			{
				double lon, lat;
				if (getLocationAt(view, ea.getX(), ea.getY(), lon, lat))
				{
					if (!_gotFirstLocation)
					{
						_finished = false;
						clear();
						_gotFirstLocation = true;
						_feature->getGeometry()->push_back(osg::Vec3d(lon, lat, 10));

						if (m_lblStart == NULL)
						{
							m_lblStart = new osgEarthXCore::LabelProvider();
							m_lblStart->setLocalText("起点");
							m_lblStart->initMapNode(getMapNode());
							osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new AddGroupObjectOperation1(m_refLabels, m_lblStart));
						}
						osgEarth::GeoPoint pnt(getMapNode()->getMapSRS(), lon, lat, 10);
						m_lblStart->setPosition(pnt);
					}
					else
					{
						if (_lastPointTemporary)
						{
							_feature->getGeometry()->back() = osg::Vec3d(lon, lat, 10);
							_lastPointTemporary = false;
						}
						else
						{
							_feature->getGeometry()->push_back(osg::Vec3d(lon, lat, 10));
						}
						_featureNode->init();

						//_gotFirstLocation = false;
						//_finished = true;
						if (_finished || !_isPath) {
							_gotFirstLocation = false;
						}

						if ((_isArea && _feature->getGeometry()->size()>=3) || !_isArea)
						{
							fireDistanceChanged();
						}
						
						aa.requestRedraw();
					}
				}
			}
		}
		else if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK) {
			if (_gotFirstLocation)
			{
				//_gotFirstLocation = false;
				_finished = true;
				if (_isArea)
				{
					fireDistanceChanged();
				}
				aa.requestRedraw();
				return true;
			}
		}
		else if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
		{
			if (_gotFirstLocation)
			{
				double lon, lat;
				if (getLocationAt(view, ea.getX(), ea.getY(), lon, lat))
				{
					if (!_lastPointTemporary)
					{
						_feature->getGeometry()->push_back(osg::Vec3d(lon, lat, 10));
						_lastPointTemporary = true;
					}
					else
					{
						_feature->getGeometry()->back() = osg::Vec3d(lon, lat, 10);
					}
					_featureNode->init();
				/*	if (!_isArea)
					{
						fireDistanceChanged();
					}*/
					aa.requestRedraw();
				}
			}
		}
		return false;
	}
	else
		return false;
}

class PlaneCallback :public osg::NodeCallback
{
public:
	PlaneCallback(osgEarth::Util::EarthManipulator* manip, osgEarth::MapNode *mapNode) :_manip(manip),
		_mapNode(mapNode),_dHitch(0),_dHitchgap(0), _dAutoHead(0){
		osgEarth::Util::Viewpoint vp = _manip->getViewpoint();
		_dHead = vp.getHeading();
		_dBent = 0;
		_dEvl = vp.z();
		_dBentgap = 0;
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor *nv)
	{
		osg::Switch* switchNd = dynamic_cast<osg::Switch*>(node);
		if (switchNd != NULL)
		{
			if (switchNd->getValue(0)) {
				osgEarth::Util::Viewpoint vp = _manip->getViewpoint();
				if (_dHead - vp.getHeading() > 1)
				{
					_dHead = vp.getHeading();
					_dBentgap = 0.1;
				}
				else if (_dHead - vp.getHeading() < -1)
				{
					_dHead = vp.getHeading();
					_dBentgap = -0.1;
				}
				if (_dBent > 5) {
					_dBentgap = -0.1;
				}
				else if (_dBent < -5) {
					_dBentgap = 0.1;
				}
				_dBent += _dBentgap;

				if (fabs(_dBent) < 0.1)
					_dBentgap = 0;

				if (_dEvl - vp.z() > 1)
				{
					_dEvl = vp.z();
					_dHitchgap = -0.1;
				}
				else if (_dEvl - vp.z() < -1)
				{
					_dEvl = vp.z();
					_dHitchgap = 0.1;
				}
				if (_dHitch > 5) {
					_dHitchgap = -0.1;
				}
				else if (_dHitch < -5) {
					_dHitchgap = 0.1;
				}
				_dHitch += _dHitchgap;

				if (fabs(_dHitch) < 0.1)
					_dHitchgap = 0;

				for(int i=0;i<switchNd->getNumChildren();i++){
					osgEarth::Annotation::ModelNode* transNode = dynamic_cast<osgEarth::Annotation::ModelNode*>(switchNd->getChild(i));
					if (transNode != NULL) {


						transNode->setPosition(osgEarth::GeoPoint(_mapNode->getMapSRS()->getGeographicSRS(), vp.x(), vp.y(), vp.z(), osgEarth::ALTMODE_ABSOLUTE));

						//transNode->setScale(osg::Vec3f(0.003, 0.003, 0.003));

						//osg::Quat modelRot(osg::DegreesToRadians(-vp.getPitch()), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(dbent), osg::Vec3d(0, 1, 0), osg::DegreesToRadians(180 - vp.getHeading()), osg::Vec3d(0, 0, 1));
						if (i == 0) {
							osg::Quat modelRot(osg::DegreesToRadians(-_dHitch), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(_dBent), osg::Vec3d(0, 1, 0), osg::DegreesToRadians(-90-vp.getHeading()), osg::Vec3d(0, 0, 1));
							transNode->setLocalRotation(modelRot);
						}
						else {
							osg::Quat modelRot(osg::DegreesToRadians(-_dHitch), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(_dBent), osg::Vec3d(0, 1, 0), osg::DegreesToRadians((float)_dAutoHead), osg::Vec3d(0, 0, 1));
							transNode->setLocalRotation(modelRot);
							_dAutoHead += 18;
							if (_dAutoHead > 360) _dAutoHead = 18;
						}
					}
				}
			}
		}
	}

	//virtual void operator()(osg::Node* node, osg::NodeVisitor *nv)
	//{
	//	osg::Switch* switchNd = dynamic_cast<osg::Switch*>(node);
	//	if (switchNd != NULL)
	//	{
	//		if (switchNd->getValue(0)) {
	//			for (int i = 0; i<switchNd->getNumChildren(); i++) {
	//				osgEarth::Annotation::ModelNode* transNode = dynamic_cast<osgEarth::Annotation::ModelNode*>(switchNd->getChild(i));
	//				if (transNode != NULL) {
	//					osgEarth::Util::Viewpoint vp = _manip->getViewpoint();

	//					transNode->setPosition(osgEarth::GeoPoint(_mapNode->getMapSRS()->getGeographicSRS(), vp.x(), vp.y(), vp.z(), osgEarth::ALTMODE_ABSOLUTE));

	//					//transNode->setScale(osg::Vec3f(0.003, 0.003, 0.003));

	//					if (_dHead - vp.getHeading() > 1)
	//					{
	//						_dHead = vp.getHeading();
	//						_dBentgap = 0.1;
	//					}
	//					else if (_dHead - vp.getHeading() < -1)
	//					{
	//						_dHead = vp.getHeading();
	//						_dBentgap = -0.1;
	//					}
	//					if (_dBent > 5) {
	//						_dBentgap = -0.1;
	//					}
	//					else if (_dBent < -5) {
	//						_dBentgap = 0.1;
	//					}
	//					_dBent += _dBentgap;

	//					if (fabs(_dBent) < 0.1)
	//						_dBentgap = 0;

	//					if (_dEvl - vp.z() > 1)
	//					{
	//						_dEvl = vp.z();
	//						_dHitchgap = -0.1;
	//					}
	//					else if (_dEvl - vp.z() < -1)
	//					{
	//						_dEvl = vp.z();
	//						_dHitchgap = 0.1;
	//					}
	//					if (_dHitch > 5) {
	//						_dHitchgap = -0.1;
	//					}
	//					else if (_dHitch < -5) {
	//						_dHitchgap = 0.1;
	//					}
	//					_dHitch += _dHitchgap;

	//					if (fabs(_dHitch) < 0.1)
	//						_dHitchgap = 0;

	//					//osg::Quat modelRot(osg::DegreesToRadians(-vp.getPitch()), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(dbent), osg::Vec3d(0, 1, 0), osg::DegreesToRadians(180 - vp.getHeading()), osg::Vec3d(0, 0, 1));
	//					osg::Quat modelRot(osg::DegreesToRadians(-_dHitch), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(_dBent), osg::Vec3d(0, 1, 0), osg::DegreesToRadians(180 - vp.getHeading()), osg::Vec3d(0, 0, 1));
	//					transNode->setLocalRotation(modelRot);
	//				}
	//			}
	//		}
	//	}
	//}
private:
	osgEarth::MapNode *_mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator > _manip;
	double _dHead;
	double _dBent;
	double _dBentgap;
	double _dEvl;
	double _dHitch;
	double _dHitchgap;
	int _dAutoHead;
};

MapViewer::SetMapViewerDataOperation::SetMapViewerDataOperation( MapViewer* pMapViewer, BaseMap* pBaseMap )
{
	m_refMapViewer = pMapViewer;
	m_refBaseMap = pBaseMap;
}

MapViewer::SetMapViewerDataOperation::~SetMapViewerDataOperation()
{
	m_refMapViewer = NULL;
	m_refBaseMap = NULL;
}

void MapViewer::SetMapViewerDataOperation::operator()( osg::Object* pObject)
{
	m_refMapViewer->_setMap( m_refBaseMap );
}

MapViewer::MapViewer( osgViewer::Viewer* pViewer /* = NULL */ ) : ViewerProviderT< osgViewer::Viewer >( pViewer )
{
}

MapViewer::~MapViewer()
{
	std::cout << "MapViewer distructing..." << std::endl;
	stop();//停止渲染
	_setMap( NULL );
	m_refViewer = NULL;
}

void MapViewer::initialized()
{
	//地球操纵器
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	keyswitchManipulator->addMatrixManipulator('1', "Earth", getOrCreateEarthManipulator());
	keyswitchManipulator->addMatrixManipulator('2', "Walk", new WalkManipulator(m_refMap->getMapNode()));
	//m_refTerrainManipulator = new osgGA::TerrainManipulator();
	//keyswitchManipulator->addMatrixManipulator('3', "Terrain", m_refTerrainManipulator.get());
	getOrCreateViewer()->setCameraManipulator(keyswitchManipulator.get());
	//getOrCreateViewer()->setCameraManipulator( getOrCreateEarthManipulator() );
	//getOrCreateEarthManipulator()->getSettings()->setMouseSensitivity(1.5);

	//测试bim数据-----------------------------------begin
	//osg::ref_ptr<osg::PagedLOD> model1 = new osg::PagedLOD;
	//model1->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf1.osgb");
	//model1->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model2 = new osg::PagedLOD;
	//model2->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf2.osgb");
	//model2->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model3 = new osg::PagedLOD;
	//model3->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf3.osgb");
	//model3->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model4 = new osg::PagedLOD;
	//model4->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf4.osgb");
	//model4->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model5 = new osg::PagedLOD;
	//model5->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf5.osgb");
	//model5->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model6 = new osg::PagedLOD;
	//model6->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf6.osgb");
	//model6->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model7 = new osg::PagedLOD;
	//model7->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf7.osgb");
	//model7->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model8 = new osg::PagedLOD;
	//model8->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf8.osgb");
	//model8->setRange(0, 0, 100000);
	//osg::ref_ptr<osg::PagedLOD> model9 = new osg::PagedLOD;
	//model9->setFileName(0, "E:\\lijp\\bim\\osg\\x\\ifcf9.osgb");
	//model9->setRange(0, 0, 100000);

	//osg::MatrixTransform* mt = new osg::MatrixTransform();
	//mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.0), 1.0f, 0.0f, 0.0f));
	////mt->addChild(grp);

	//mt->addChild(model1);
	//mt->addChild(model2);
	//mt->addChild(model3);
	//mt->addChild(model4);
	//mt->addChild(model5);
	//mt->addChild(model6);
	//mt->addChild(model7);
	//mt->addChild(model8);
	//mt->addChild(model9);

	//for (int i = 0; i < 5; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		osgEarth::GeoTransform* xform = new osgEarth::GeoTransform();
	//		//xform->setTerrain(mapNode->getTerrain());
	//		osgEarth::GeoPoint point(m_refMap->getMapNode()->getMapSRS(), 116.460453677 + i*0.002, 40.107267857 + j*0.002, 33);
	//		xform->setPosition(point);
	//		xform->addChild(mt);
	//		m_refMap->m_refRoot->addChild(xform);
	//	}
	//}
	//测试bim数据--------------------------------end

	////osg 事件处理器
	getOrCreateViewer()->addEventHandler(new osgViewer::StatsHandler());
	//m_refViewer->addEventHandler(new osgViewer::StatsHandler());
	//getOrCreateViewer()->addEventHandler(new osgViewer::WindowSizeHandler());
	//m_refViewer->addEventHandler(new osgViewer::ThreadingHandler());
	getOrCreateViewer()->addEventHandler(new osgViewer::LODScaleHandler());
	//m_refViewer->addEventHandler(new osgGA::StateSetManipulator(m_Viewer.getCamera()->getOrCreateStateSet()));
	//m_refViewer->addEventHandler(new osgViewer::RecordCameraPathHandler());

	getOrCreateViewer()->getCamera()->setNearFarRatio(0.00002);
	getOrCreateViewer()->getCamera()->setSmallFeatureCullingPixelSize( 0.0f );

	//自动调节geocentric地球上剪裁平面的远近
	//getOrCreateViewer()->getCamera()->addCullCallback( new osgEarth::Util::AutoClipPlaneCullCallback( m_refMap->getMapNode() ) );
}

void MapViewer::run()
{
	osgX::StaticOSGViewerAssistant::setViewerBase( getOrCreateViewer() );	
	initialized();
	    
	getOrCreateViewer()->addEventHandler(getOrCreateMeasureTool());
	getOrCreateViewer()->addEventHandler(getOrCreateMouseCoordsTool());
	//if (m_refMap) {
	//	AnnotationMap* pMap = dynamic_cast<AnnotationMap*>(m_refMap.get());
	//	if (pMap) {
	//		pMap->installUpdateTipCallback(getOrCreateViewer());
	//	}
	//}
	//if (m_refMap.get()) {
	//	AnnotationMap* pMap = dynamic_cast<AnnotationMap*>(m_refMap.get());
	//	if (pMap) {
	//		m_refTerrainManipulator->setNode(pMap->getModels());
	//	}
	//}

	if (m_refFlyHandler.get() == NULL) {
		m_refFlyHandler = new FlyToViewpointHandler(getOrCreateEarthManipulator(), m_refMap->getMapNode());
	}
	getOrCreateViewer()->addEventHandler(m_refFlyHandler);

	if (m_refFlybylineHandler.get() == NULL) {
		m_refFlybylineHandler = new FlybylineHandler(getOrCreateEarthManipulator(), m_refMap->getMapNode());
	}
	getOrCreateViewer()->addEventHandler(m_refFlybylineHandler);

	if (m_refRotateHandler.get() == NULL) {
		m_refRotateHandler = new RotateByViewpointHandler(getOrCreateEarthManipulator(), m_refMap->getMapNode());
	}
	getOrCreateViewer()->addEventHandler(m_refRotateHandler);

	m_refPlane = new osg::Switch();
	//osg::Node* pla = osgDB::readNodeFile(osgDB::findLibraryFile("resource/cessna.osg"));
	osg::Group* pla = dynamic_cast<osg::Group*>(osgDB::readNodeFile(osgDB::findLibraryFile("resource/helicopter.osgb")));
	if (pla) {
		for (int i = 0; i < pla->getNumChildren(); i++) {
			osgEarth::Symbology::Style style;
			style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(pla->getChild(i));
			osgEarth::Annotation::ModelNode* mdl = new osgEarth::Annotation::ModelNode(m_refMap->getMapNode(), style);
			m_refPlane->addChild(mdl);
		}
		m_refPlane->addUpdateCallback(new PlaneCallback(getOrCreateEarthManipulator(), m_refMap->getMapNode()));

	}
	m_refPlane->setValue(0, false);
	m_refMap->getMapNode()->addChild(m_refPlane.get());
	addCompass();

	if (m_refPhotoCallback.get() == NULL) {
		m_refPrintImage = new osg::Image;
		m_refPhotoCallback = new PhotoCallback(m_refPrintImage.get());
		getOrCreateViewer()->getCamera()->setPostDrawCallback(m_refPhotoCallback.get());
	}



	ViewerProviderT< osgViewer::Viewer >::run();
}

void MapViewer::stop()
{
	ViewerProviderT< osgViewer::Viewer >::stop();
	osgX::StaticOSGViewerAssistant::setViewerBase( NULL );
}

osgEarth::Util::EarthManipulator* MapViewer::getOrCreateEarthManipulator()
{
	if ( m_refEarthManipulator.get() == NULL )
	{
		m_refEarthManipulator= new osgEarth::Util::EarthManipulator();
	}
	return m_refEarthManipulator;
}

MyMeasureToolHandler* MapViewer::getOrCreateMeasureTool()
{
	if (m_refMeasureTool.get() == NULL)
	{
		m_refMeasureTool = new MyMeasureToolHandler(m_refMap->getMapRoot(), m_refMap->getMapNode());
		//m_refMeasureTool->setIntersectionMask(0x1);
		m_refMeasureTool->setIsPath(true);
	}
	return m_refMeasureTool;
}

MouseCoordsToolX* MapViewer::getOrCreateMouseCoordsTool()
{
	if (m_refMouseCoordsTool.get() == NULL)
	{
		osgEarthXCore::AnnotationMap* pMap = dynamic_cast<osgEarthXCore::AnnotationMap*>( m_refMap.get());
		if (pMap) {
			m_refMouseCoordsTool = new MouseCoordsToolX(m_refMap->getMapNode(),pMap->getPlaces() );
			if (m_refMouseCoordsCallback != NULL)
				m_refMouseCoordsTool->addCallback(m_refMouseCoordsCallback.get());
		}
	}
	return m_refMouseCoordsTool;
}

void osgEarthXCore::MapViewer::installMouseCoordsToolCallBack(MouseCoordsToolX::MouseCoordsCallback* ca)
{
	if (ca != NULL) {
		m_refMouseCoordsCallback = ca;
	}
}


void osgEarthXCore::MapViewer::beginFly()
{
	m_refPlane->setValue(0, true);
	m_refPlane->setValue(1, true);
	if (m_refFlyHandler.get())
		m_refFlyHandler->Play(true);
}


void osgEarthXCore::MapViewer::stopFly()
{
	m_refPlane->setValue(0, false);
	m_refPlane->setValue(1, false);
	if (m_refFlyHandler.get())
		m_refFlyHandler->Play(false);
}

void MapViewer::print(std::string filePath)
{
	if (m_refPhotoCallback.get() != NULL) {
		m_refPrintImage->setUserValue("Capture", true);
		m_refPhotoCallback->setFileName(filePath);
	}
}

void osgEarthXCore::MapViewer::beginFlybyline(osgEarth::Symbology::LineString* lin, double dEve, double dPitch, double dSpeed)
{
	if (m_refFlybylineHandler.get()) {
		m_refFlybylineHandler->init();

		m_refFlybylineHandler->setLine(lin);
		m_refFlybylineHandler->setElevation(dEve);
		m_refFlybylineHandler->setHitch(dPitch);
		m_refFlybylineHandler->setSpeed(dSpeed);
	    m_refFlybylineHandler->Play(true);

		m_refPlane->setValue(0, true);
		m_refPlane->setValue(1, true);
	}
}


void osgEarthXCore::MapViewer::stopFlybyline()
{
	if (m_refFlybylineHandler.get()) {
		m_refPlane->setValue(0, false);
		m_refPlane->setValue(1, false);
		m_refFlybylineHandler->Play(false);
	}
}

void MapViewer::beginRotate(double dlon, double dlat, double dele, double dRange)
{
	if (m_refRotateHandler.get())
	{
		m_refRotateHandler->init(dlon, dlat, dele, dRange);
		m_refRotateHandler->Play(true);
	}
}

void MapViewer::beginRotateEx(double dlon, double dlat, double dele, double dMaxRange, double dMinRange,
	double dMaxPitch, double dMinPitch)
{
	if (m_refRotateHandler.get())
	{
		m_refRotateHandler->init(dlon, dlat, dele, dMaxRange,dMinRange,dMaxPitch,dMinPitch);
		m_refRotateHandler->Play(true);
	}
}

void MapViewer::stopRotate()
{
	if (m_refRotateHandler.get())
		m_refRotateHandler->Play(false);
}

void MapViewer::setMap( BaseMap* pMap )
{
	//osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new SetMapViewerDataOperation( this, pMap ) );
	_setMap(pMap);
}

void MapViewer::_setMap( BaseMap* pMap )
{
	if( m_refMap.valid() )
	{
		m_refMap->initMapViewer( NULL );
	}

	if( pMap != NULL )
	{
		getOrCreateViewer()->setSceneData( pMap->getMapRoot() );
		pMap->initMapViewer( this );

	}else{
		getOrCreateViewer()->setSceneData( NULL );
	}

	m_refMap = pMap;
}

osg::MatrixTransform* createCompassPart(const std::string& image, float radius, float height)
{
	osg::Vec3 center(-radius, -radius, height);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(
		createTexturedQuadGeometry(center, osg::Vec3(radius*2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius*2.0f, 0.0f)));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(image));

	osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
	part->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	part->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	part->addChild(geode.get());
	return part.release();
}

void osgEarthXCore::MapViewer::addCompass()
{
	osg::ref_ptr<Compass> compass = new Compass;
	osg::Camera* cam = getOrCreateViewer()->getCamera();
	int h = cam->getViewport()->height();
	compass->setViewport(0.0, h-128, 128.0, 128.0);
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	std::string strCompass = osgDB::findLibraryFile("resource/compass.png");
	compass->setPlate(createCompassPart(strCompass, 1.5f, -1.0f));
	//compass->setNeedle(createCompassPart("compass_needle.png", 1.5f, 0.0f));
	compass->setMainCamera(cam);

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	m_refMap->getMapRoot()->addChild(compass.get());
}

void MapViewer::setSelectedColor(osg::Vec4f col, bool bCol)
{
	if (m_refMouseCoordsTool.get()!= NULL)
	{
		m_refMouseCoordsTool->setSelectedColor(col, bCol);
	}
}
