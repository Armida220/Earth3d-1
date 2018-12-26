/**
	@brief 用于浏览三维地图的Viewer
	@author Hao Luo
	@date 2014/12/15
*/

#ifndef OSGEARTHX_CORE_MAP_VIEWER_H
#define OSGEARTHX_CORE_MAP_VIEWER_H 1

#include <osgEarthXCoreExport/Export.h>
#include <Viewer/ViewerProviderT.h>
#include <Provider/ExternalProvider/ViewpointProvider.h>
#include <Provider/AnnotationProvider/LabelProvider.h>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/MeasureTool>
#include <osgViewer/Viewer>
#include <osg/Switch>
#include <osgEarth/GeoMath>
//#include <osgEarthUtil/Controls>
#include "PhotoCallback.h"

#include <Windows.h>

class AddGroupObjectOperation1 : public osg::Operation
{
public:
	AddGroupObjectOperation1(osg::Group* pGroup, osg::Node* pNode)
	{
		m_refGroup = pGroup;
		m_refNode = pNode;
	}

	virtual void operator()(osg::Object*){
		if (m_refGroup.get() == NULL || m_refNode.get() == NULL)
		{
			return;
		}

		m_refGroup->addChild(m_refNode);
	}

protected:
	virtual ~AddGroupObjectOperation1() {}

protected:
	osg::ref_ptr<osg::Group> m_refGroup;
	osg::ref_ptr<osg::Node> m_refNode;

private:
};

struct MyMeasureToolHandler : public osgEarth::Util::MeasureToolHandler
{
public:
	MyMeasureToolHandler(osg::Group* group, osgEarth::MapNode* mapNode) :osgEarth::Util::MeasureToolHandler(group, mapNode), 
	_bValid(false),
	m_lblStart(NULL),
	_x(0),
	_y(0),
	_width(0),
	_height(0)
	{
		m_refLabels = new osg::Group();
		group->addChild(m_refLabels);
	}

	virtual ~MyMeasureToolHandler() {}
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void clearAll()
	{
		clear();
		m_refLabels->removeChildren(0, m_refLabels->getNumChildren());
		m_lblStart = 0;
	}

	void setValid(bool bValid)
	{
		_bValid = bValid;
	}

protected:
	void toggleFullscreen(osgViewer::GraphicsWindow *window);
private:
	bool _bValid;
	osg::ref_ptr<osgEarthXCore::LabelProvider> m_lblStart;
	osg::ref_ptr<osg::Group> m_refLabels;
	int _x;
	int _y;
	int _width;
	int _height;
	HWND _hParent;
};

class MouseCoordsToolX : public osgGA::GUIEventHandler
{
public:
	struct MouseCoordsCallback : public osg::Referenced
	{
		// called when valid map coordinates are found under the mouse
		virtual void mouseMove(const osgEarth::GeoPoint& coords) = 0;
		virtual void mouseDown(const osgEarth::GeoPoint& coords) = 0;
		virtual void mouseUp(const osgEarth::GeoPoint& coords,const std::string strUp) = 0;
		virtual void mouseDoubleClick(const osgEarth::GeoPoint& coords) = 0;
		virtual ~MouseCoordsCallback() { }
	};

public:
	MouseCoordsToolX(osgEarth::MapNode* mapNode,osg::Node* nd);

	virtual ~MouseCoordsToolX() { }

	/**
	* Adds a readout callback.
	*/
	void addCallback(MouseCoordsCallback* callback);


public: // GUIEventHandler

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void setSelectedColor(osg::Vec4f col, bool bCol);
protected:
	osgEarth::MapNode*      _mapNode;
	osg::NodePath _mapNodePath;
	typedef std::vector< osg::ref_ptr<MouseCoordsCallback> > Callbacks;
	Callbacks _callbacks;
	osg::ref_ptr<osg::Node> _places;

	osg::ref_ptr<osg::Group> m_selGroup;
	osg::ref_ptr<osg::Material> m_material_previous;
	osg::ref_ptr<osg::Material> m_material_selected;
	bool m_bSelectedCol;
};

namespace osgEarthXCore
{
	struct RotateByViewpointHandler : public osgGA::GUIEventHandler
	{
		RotateByViewpointHandler(osgEarth::Util::EarthManipulator* manip, osgEarth::MapNode* mapNode) :
			_manip(manip), _mapNode(mapNode), _bPlay(false), _bStart(false), _dSpeed(80.0) { }

		void init(double dlon, double dlat, double dele, double dRadius);
		void init(double dlon, double dlat, double dele, double dMaxRadius, double dMinRadius, double dMaxPitch, double dMinPitch);

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void Play(bool bPlay) {
			if (!bPlay) _bStart = false;
			_bPlay = bPlay;
		}

		bool _bPlay;
		bool _bStart;
		osg::observer_ptr<osgEarth::Util::EarthManipulator> _manip;
		osg::ref_ptr< osgEarth::MapNode > _mapNode;
		double _dSpeed;
		double _dlon;
		double _dlat;
		double _dele;
		double _dRadius;
		double _dMinRadius;
		double _dMaxRadius;
		double _dHead;
		double _dPitch;
		double _dMinAngle;
		double _dMaxAngle;
		osg::Timer_t _startFrameTick;
	};

	struct FlyToViewpointHandler : public osgGA::GUIEventHandler
	{
		FlyToViewpointHandler(osgEarth::Util::EarthManipulator* manip, osgEarth::MapNode* mapNode) : 
			_manip(manip), _mapNode(mapNode), _bPlay(false), _bStart(false), dSpeed(80.0),_dEle(500),_dSp(0.0),_dSin(0.0),_dCos(0.0) { }

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
		
		void Play(bool bPlay) {
			if (!bPlay) _bStart = false;			
			_bPlay = bPlay;
		}

		bool _bPlay;
		bool _bStart;
		osg::observer_ptr<osgEarth::Util::EarthManipulator> _manip;
		osg::ref_ptr< osgEarth::MapNode > _mapNode;
		double dSpeed;
		double _dEle;
		double _dSp;
		double _dSin;
		double _dCos;
	};

	struct FlybylineHandler : public osgGA::GUIEventHandler
	{
		FlybylineHandler(osgEarth::Util::EarthManipulator* manip, osgEarth::MapNode* mapNode) : _manip(manip), _mapNode(mapNode),
			_bPlay(false), _curIndex(0),_dEle(500),_dHitch(-4),_dLength(0)
		{
			setSpeed(80.0);
		}

		void init() {
			_curIndex = 0;
			_dEle = 300; 
			_dHitch = -4;
			_dLength = 0;
		}
		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void Play(bool bPlay) {
			_bPlay = bPlay;
		}
		void setHitch(double dHitch) {
			_dHitch = dHitch;
		}
		void setElevation(double dEle) {
			_dEle = dEle;
		}
		void setLine(osgEarth::Symbology::LineString* lin) {
			_line = lin;
			_dHeads.clear();
			_dsin.clear();
			_dcos.clear();
			for (int i = 0; i < _line->size() - 1; i++)
			{
				double dHead = osgEarth::GeoMath::bearing(osg::DegreesToRadians(lin->at(i).y()), osg::DegreesToRadians(lin->at(i).x()),
					osg::DegreesToRadians(lin->at(i + 1).y()),osg::DegreesToRadians(lin->at(i + 1).x()));
				_dHeads.push_back(dHead);
				_dsin.push_back(sin(dHead));
				_dcos.push_back(cos(dHead));
			}
		}

		void setSpeed(double dSpeed)
		{
			_dGap = dSpeed*1000.0 / 3600.0; //换算成米每秒
			_dGap = _dGap / 50;//如果帧率是50，米每帧
			_dGap = _dGap * 360 / 40000000;//度每帧
		}

		bool _bPlay;

		double _dSpeed;
		double _dEle;
		double _dHitch;
		double _curX;
		double _curY;
		int _curIndex;
		double _dGap;
		double _dLength;
		osg::observer_ptr<osgEarth::Util::EarthManipulator> _manip;
		osg::ref_ptr< osgEarth::MapNode > _mapNode;
		osg::ref_ptr< osgEarth::Symbology::LineString > _line;
		std::vector<double> _dHeads;
		std::vector<double> _dsin;
		std::vector<double> _dcos;
	};

	class BaseMap;

	class OSGEARTHX_CORE_EXPORT MapViewer:
		virtual public osg::Referenced,
		public ViewerProviderT< osgViewer::Viewer >
	{
	public:
		friend BaseMap;

	protected:
		osg::ref_ptr< BaseMap > m_refMap;
		osg::ref_ptr< osgEarth::Util::EarthManipulator > m_refEarthManipulator;
		//osg::ref_ptr< osgGA::TerrainManipulator > m_refTerrainManipulator;
		osg::ref_ptr< MouseCoordsToolX > m_refMouseCoordsTool;
		osg::ref_ptr< osg::Switch > m_refPlane;
		osg::ref_ptr< FlyToViewpointHandler > m_refFlyHandler;
		osg::ref_ptr< FlybylineHandler > m_refFlybylineHandler;
		osg::ref_ptr<RotateByViewpointHandler> m_refRotateHandler;
		//osg::ref_ptr< osgEarth::Util::Controls::ControlCanvas > m_ctrlCanvas;
	public:
		MapViewer( osgViewer::Viewer* pViewer = NULL );
		virtual ~MapViewer();

		osgEarth::Util::EarthManipulator* getOrCreateEarthManipulator();
		MyMeasureToolHandler* getOrCreateMeasureTool();
		MouseCoordsToolX* getOrCreateMouseCoordsTool();
		void installMouseCoordsToolCallBack(MouseCoordsToolX::MouseCoordsCallback* ca);
		void setSelectedColor(osg::Vec4f col, bool bCol);
		void setMap( BaseMap* pMap );

		virtual void run();
		virtual void stop();

		void beginFly();
		void stopFly();
		void print(std::string filePath);

		void beginFlybyline(osgEarth::Symbology::LineString* lin,double dEve,double dPitch,double dSpeed);
		void stopFlybyline();

		void beginRotate(double dlon,double dlat,double dele,double dRange);
		void beginRotateEx(double dlon, double dlat, double dele, double dMaxRange,double dMinRange,double dMaxPitch,double dMinPitch);
		void stopRotate();
	protected:
		virtual void initialized();
		void _setMap( BaseMap* pMap );
		void addCompass();

		osg::ref_ptr<MyMeasureToolHandler> m_refMeasureTool;
		osg::ref_ptr<MouseCoordsToolX::MouseCoordsCallback> m_refMouseCoordsCallback;
		osg::ref_ptr<PhotoCallback> m_refPhotoCallback;
		osg::ref_ptr<osg::Image> m_refPrintImage;

	protected:

		class SetMapViewerDataOperation : public osg::Operation
		{
		public:
			SetMapViewerDataOperation( MapViewer* pMapViewer, BaseMap* pBaseMap );
			void operator()( osg::Object* pObject);
		protected:
			virtual ~SetMapViewerDataOperation();

		private:
			osg::ref_ptr< MapViewer > m_refMapViewer;
			osg::ref_ptr< BaseMap > m_refBaseMap;
		};

	};
}

#endif