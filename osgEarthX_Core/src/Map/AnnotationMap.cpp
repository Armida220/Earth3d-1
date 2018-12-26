#include <Map/AnnotationMap.h>
#include <osgDB/FileUtils>
using namespace osgEarthXCore;

#include <boost/algorithm/string.hpp>
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>
#include <osgEarthAnnotation/ScaleDecoration>
//#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarth/Decluttering>
#include <osgEarthAnnotation/ModelNode>
#include <Provider/AnnotationProvider/PlaceProvider.h>
#include <Provider\AnnotationProvider\ModelProvider.h>
#include <Provider/AnnotationProvider/FeaturePrimitiveProvider.h>
#include <Provider/AnnotationProvider/CircleProvider.h>
#include <Provider/AnnotationProvider/ImageOverlayProvider.h>
#include <osgEarthAnnotation/AnnotationData>

osgEarth::DeclutteringOptions g_dcOptions;

double getLocationZ(osgEarth::MapNode* mapNode, osgViewer::Viewer* vw,double dlon, double dlat)
{
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
	vw->getCamera()->accept(iv);

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

#include <Windows.h>
class PeopleCallback : public osg::NodeCallback
{
public:
	PeopleCallback(osgViewer::Viewer* vw,std::vector<osgEarth::GeoPoint> pnts,double dSpeed) :
		m_refViewer(vw),
		_pnts(pnts),
		_dSpeed(dSpeed),
		_index(0),
		_dStartTime(-1.0)
	{
		for (int i = 0; i < (int)_pnts.size() - 1; i++) {
			double dis = osgEarth::GeoMath::distance(osg::DegreesToRadians(_pnts[i].y()), osg::DegreesToRadians(_pnts[i].x()),
				osg::DegreesToRadians(_pnts[i + 1].y()), osg::DegreesToRadians(_pnts[i + 1].x()));
			_times.push_back(dis / _dSpeed);
		}
	}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
		if (_pnts.size()<2 || nv->getVisitorType() != osg::NodeVisitor::UPDATE_VISITOR) {
			NodeCallback::traverse(node, nv);
			return;
		}
		double dCurTime = nv->getFrameStamp()->getSimulationTime();
		osgEarth::Annotation::ModelNode* track = dynamic_cast<osgEarth::Annotation::ModelNode*>(node);
		if (track == NULL) return;
		if (_dStartTime < 0)
		{
			_dStartTime = dCurTime;
			double fw = osgEarth::GeoMath::bearing(osg::DegreesToRadians(_pnts[0].y()), osg::DegreesToRadians(_pnts[0].x()),
				osg::DegreesToRadians(_pnts[1].y()), osg::DegreesToRadians(_pnts[1].x()));
			fw = osg::RadiansToDegrees(fw) + 180;
			osg::Quat ori = osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(0, 1, 0)) *
				osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(1, 0, 0)) *
				osg::Quat(osg::DegreesToRadians(fw), osg::Vec3(0, 0, -1));
			track->setPosition(_pnts[0]);
			track->setLocalRotation(ori);
		}
		if (dCurTime - _dStartTime < _times[_index]) {
			double t = (dCurTime - _dStartTime) / _times[_index];
			osg::Vec3d pos;
		    osgEarth::GeoMath::interpolate(
				osg::DegreesToRadians(_pnts[_index].y()), osg::DegreesToRadians(_pnts[_index].x()),
				osg::DegreesToRadians(_pnts[_index + 1].y()), osg::DegreesToRadians(_pnts[_index + 1].x()),
				t,
				pos.y(), pos.x());

			//double dz = getLocationZ(track->getMapNode(), m_refViewer.get(), 
			//	osg::RadiansToDegrees(pos.x()), osg::RadiansToDegrees(pos.y()));

			//char message[256];
			//sprintf(message, "dz=%.2f\n", dz);
			//OutputDebugStringA(message);

			 osgEarth::GeoPoint geo(
				track->getMapNode()->getMapSRS(),
				osg::RadiansToDegrees(pos.x()),
				osg::RadiansToDegrees(pos.y()),
				10,osgEarth::ALTMODE_RELATIVE);

			// update the position label.
			track->setPosition(geo);
		}
		else
		{
			_index++;
			if (_index == _pnts.size() - 1) {
				_index = 0;
			}
			_dStartTime = dCurTime;
			double fw = osgEarth::GeoMath::bearing(osg::DegreesToRadians(_pnts[_index].y()), osg::DegreesToRadians(_pnts[_index].x()),
				osg::DegreesToRadians(_pnts[_index + 1].y()), osg::DegreesToRadians(_pnts[_index + 1].x()));
			fw = osg::RadiansToDegrees(fw) + 180;
			osg::Quat ori = osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(0, 1, 0)) *
				osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(1, 0, 0)) *
				osg::Quat(osg::DegreesToRadians(fw), osg::Vec3(0, 0, -1));
			track->setLocalRotation(ori);
		}
		NodeCallback::traverse(node, nv);
	}
protected:
	int _index;
	std::vector<osgEarth::GeoPoint> _pnts;
	std::vector<double> _times;
	double _dSpeed;
	double _dStartTime;
	osg::ref_ptr< osgViewer::Viewer > m_refViewer;
};

class TipCallback :public osg::NodeCallback
{
public:
	TipCallback(osgViewer::Viewer* vw):m_refViewer(vw){}

	virtual void operator()(osg::Node* nd,osg::NodeVisitor* nv)
	{
		PlaceProvider* pv = dynamic_cast<PlaceProvider*>(nd);
		if (pv) {
			/*if (pv->getFlash()) {
				int iframeCount = nv->getTraversalNumber();
				if (iframeCount%10 == 0) {
					unsigned int iMask = pv->getNodeMask()==0 ? 1 :0;
					pv->setNodeMask(iMask);
				}
			}*/
			
			pv->UpdteTip(m_refViewer.get());
		}
		//traverse(nd, nv);
	}
protected:
	osg::ref_ptr< osgViewer::Viewer > m_refViewer;
};

class SingleTipCallback :public osg::NodeCallback
{
public:
	SingleTipCallback(AnnotationMap* vw) :m_refMap(vw){}

	virtual void operator()(osg::Node* nd, osg::NodeVisitor* nv)
	{
		//osgEarth::Util::Controls::Control* pv = dynamic_cast<osgEarth::Util::Controls::Control*>(nd);
		//if (pv) {
			m_refMap->updateSingleTip();
		//}
		//traverse(nd, nv);
	}
protected:
	osg::ref_ptr< AnnotationMap > m_refMap;
};

class FlashCallback :public osg::NodeCallback
{
public:
	FlashCallback():_bFlash(false){}

	virtual void operator()(osg::Node* nd, osg::NodeVisitor* nv)
	{
		PlaceProvider* pv = dynamic_cast<PlaceProvider*>(nd);
		if (pv) {
			if (pv->getFlash()) {
				int iframeCount = nv->getTraversalNumber();
				if (iframeCount % 20 == 0) {
					_bFlash = !_bFlash;
				}
				if (_bFlash) {
					traverse(nd, nv);
				}
			}
			else {
				traverse(nd, nv);
			}
		}

	}
protected:
	bool _bFlash;
};

class AddTipOperation : public osg::Operation
{
public:
	AddTipOperation(osgEarth::Util::Controls::ControlCanvas* canvas, osgEarth::Util::Controls::Control* ctrl) : _refCanvas(canvas), _singleTip(ctrl) {}

	virtual void operator()(osg::Object*)
	{
		_refCanvas->addControl(_singleTip.get());
	}

protected:
	virtual ~AddTipOperation() {}

protected:
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> _refCanvas;
	osg::ref_ptr< osgEarth::Util::Controls::Control > _singleTip;

private:
};

AnnotationMap::AnnotationMap() :m_bContentChanged(false)
{
	m_refAnnotations = new osg::Group();
	m_refAnnotations->setDataVariance(osg::Object::DYNAMIC);
	osgEarth::Decluttering::setEnabled(m_refAnnotations->getOrCreateStateSet(), true);
	m_refRoot->addChild( m_refAnnotations );

	m_refVIPAnnotations = new osg::Group();
	m_refVIPAnnotations->setDataVariance(osg::Object::DYNAMIC);
	m_refRoot->addChild(m_refVIPAnnotations);

	g_dcOptions = osgEarth::Decluttering::getOptions();
	g_dcOptions.inAnimationTime() = 1.0f;
	g_dcOptions.outAnimationTime() = 1.0f;
	g_dcOptions.sortByPriority() = true;
	osgEarth::Decluttering::setOptions(g_dcOptions);

	//m_refModels = new osg::Group();
	//m_refRoot->addChild(m_refModels);
	//m_refModels->setCullingActive(false);

	m_refFeatures = new osg::Group();
	m_refRoot->addChild(m_refFeatures);

	m_refImgs = new osg::Group();
	m_refRoot->addChild(m_refImgs);

	m_refParticles = new osg::Group();
	m_refRoot->addChild(m_refParticles);

	m_refBims = new osg::Group();
	m_refRoot->addChild(m_refBims);

	m_refEditors = new osg::Group();
	m_refRoot->addChild(m_refEditors);

	m_refCB = new osgEarth::Util::AnnotationEventCallback();
}

void AnnotationMap::addAnnotations(osg::Node* pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	addGroupObject(m_refModels, pNode);
}

void AnnotationMap::addFire(FireProvider* pFire)
{
	if (pFire == NULL)
	{
		return;
	}

	pFire->initMapNode(m_refMapNode);
	addGroupObject(m_refParticles, pFire);
}

void AnnotationMap::addAnnotation( osgEarth::Annotation::AnnotationNode* pAnnotationNode )
{
	if ( pAnnotationNode == NULL )
	{
		return;
	}

	pAnnotationNode->initMapNode( m_refMapNode );
	//pAnnotationNode->installDecoration("hover", new osgEarth::Annotation::HighlightDecoration());
	//pAnnotationNode->installDecoration("hover", new osgEarth::Annotation::ScaleDecoration(1.1f));

	osgEarth::Annotation::ModelNode* mdl = dynamic_cast<osgEarth::Annotation::ModelNode *>(pAnnotationNode);
	if (NULL != mdl)
	{
		addGroupObject(m_refModels, mdl);
		osgEarthXCore::ModelProvider* mdlprovider = dynamic_cast<osgEarthXCore::ModelProvider *>(pAnnotationNode);
		if (mdlprovider) {
			osgEarth::Symbology::LineString * path = mdlprovider->getPath();
				std::vector<osgEarth::GeoPoint> pnts;			
			if (path) {
				for (int i = 0; i < path->size(); i++)
				{
					osgEarth::GeoPoint pnt(mdl->getMapNode()->getMapSRS(), path->at(i).x(), path->at(i).y(), 10, osgEarth::ALTMODE_RELATIVE);
					pnts.push_back(pnt);
				}
			}
			mdlprovider->setUpdateCallback(new PeopleCallback(m_pMapViewer->getOrCreateViewer(),pnts, mdlprovider->getSpeed()));
		}
	}
	else {
		osgEarthXCore::PlaceProvider* pla = dynamic_cast<osgEarthXCore::PlaceProvider *>(pAnnotationNode);
		if (pla) {
			osgEarth::Annotation::AnnotationData* data = new osgEarth::Annotation::AnnotationData();
			data->setPriority(pla->getPriority());
			pla->setAnnotationData(data);

			pla->addCullCallback(new FlashCallback());
			pla->setCullingNodes(m_refModels);
			if(pla->getCulling())
				pla->setOcclusionCulling(true, m_refModels);
			pla->setEditor(m_refEditors);
			if(!m_refCanvas.valid())
				m_refCanvas = osgEarth::Util::Controls::ControlCanvas::getOrCreate(m_pMapViewer->getOrCreateViewer());
			pla->setControlCanvas(m_refCanvas.get());
			if(m_pMapViewer)
				pla->setUpdateCallback(new TipCallback(m_pMapViewer->getOrCreateViewer()));
			if (m_tipCallback) {
				PlayClickHandler* tipHandler = new PlayClickHandler(pla);
				tipHandler->addCallback(m_tipCallback);
				pla->setTipHandler(tipHandler);
			}
			//pla->getOrCreateStateSet()->setRenderBinDetails(101, "RenderBin");
			if (pla->getPriority() > 100.0) {
				pAnnotationNode->getOrCreateStateSet()->setRenderBinDetails(101, "RenderBin");
				addGroupObject(m_refVIPAnnotations, pAnnotationNode);
			}
			else {
				addGroupObject(m_refAnnotations, pAnnotationNode);
			}
		}
		else {
			osgEarthXCore::FeaturePrimitiveProvider* fp = dynamic_cast<osgEarthXCore::FeaturePrimitiveProvider *>(pAnnotationNode);
			osgEarthXCore::CircleProvider* fp1 = dynamic_cast<osgEarthXCore::CircleProvider *>(pAnnotationNode);
			if (fp || fp1) {
				addGroupObject(m_refFeatures, pAnnotationNode);
			}
			else {
				//pAnnotationNode->getOrCreateStateSet()->setRenderBinDetails(101, "RenderBin");
				osgEarthXCore::ImageOverlayProvider* img = dynamic_cast<osgEarthXCore::ImageOverlayProvider *>(pAnnotationNode);
				if (img) {
					addGroupObject(m_refImgs, pAnnotationNode);
				}
				else {
					addGroupObject(m_refAnnotations, pAnnotationNode);
				}
			}
		}

	}
}

void AnnotationMap::removeFire(FireProvider* pFire)
{
	if (pFire == NULL)
	{
		return;
	}
	removeGroupObjet(m_refParticles, pFire);
}

void osgEarthXCore::AnnotationMap::addLocationNode(LocationProvider * loc)
{
	if (loc == NULL)
	{
		return;
	}

	//loc->setTerrain(m_refMapNode->getTerrain());
	addGroupObject(m_refBims, loc);
	m_pMapViewer->getOrCreateViewer()->getCamera()->setSmallFeatureCullingPixelSize(10.0f); 
}

void osgEarthXCore::AnnotationMap::removeLocationNode(LocationProvider * loc)
{
	if (loc == NULL)
	{
		return;
	}
	removeGroupObjet(m_refBims, loc);
}

void AnnotationMap::clearScene()
{
	//m_refAnnotations->removeChildren(0, m_refAnnotations->getNumChildren());
	//m_refVIPAnnotations->removeChildren(0, m_refVIPAnnotations->getNumChildren());
	//m_refFeatures->removeChildren(0, m_refFeatures->getNumChildren());
	//m_refParticles->removeChildren(0, m_refParticles->getNumChildren());
	//m_refImgs->removeChildren(0, m_refImgs->getNumChildren());
	//m_refAnalysis->removeChildren(0, m_refAnalysis->getNumChildren());
	//m_refModels->removeChildren(0, m_refModels->getNumChildren());
	//m_refBims->removeChildren(0, m_refBims->getNumChildren());

	removeAllGroupObjet(m_refAnnotations);
	removeAllGroupObjet(m_refVIPAnnotations);
	removeAllGroupObjet(m_refFeatures);
	removeAllGroupObjet(m_refParticles);
	removeAllGroupObjet(m_refImgs);
	removeAllGroupObjet(m_refAnalysis);
	removeAllGroupObjet(m_refModels);
	removeAllGroupObjet(m_refBims);
}

void AnnotationMap::showSingleTip(double lon, double lat, double alt, const std::string strContent)
{
	if (strContent.empty()) return;

	m_dTipLon = lon;
	m_dTipLat = lat;
	m_dTipAlt = alt;
	if (m_strSingleTipContent != strContent){
		m_strSingleTipContent = strContent;
		m_bContentChanged = true;
	}
	if (!m_refCanvas.valid()){
		m_refCanvas = osgEarth::Util::Controls::ControlCanvas::getOrCreate(m_pMapViewer->getOrCreateViewer());
		//osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new AddGroupObjectOperation(m_refRoot.get(), m_refCanvas.get()));
		//getMapRoot()->addChild(m_refCanvas.get());
	}
	if (m_singleTip.get() == NULL)
	{
		osgText::Font *fntKai = osgText::readFontFile("simkai.ttf");
		m_singleTip = new osgEarth::Util::Controls::VBox();
		m_singleTip->setPosition(20, 20);
		//m_refTip->setPadding(10);

		osgEarth::Util::Controls::VBox* pBox = ((osgEarth::Util::Controls::VBox*)m_singleTip.get());

		osgEarth::Util::Controls::VBox *vbox = new osgEarth::Util::Controls::VBox();
		vbox->setBorderColor(1, 1, 1, 1);
		vbox->setBackColor(.6, .5, .4, 0.8);

		//m_singleTipcontent = new osgEarth::Util::Controls::LabelControl(strContent, osg::Vec4(1, 1, 1, 1), 22);
		//m_singleTipcontent->setEncoding(osgText::String::ENCODING_UTF8);
		//m_singleTipcontent->setFont(fntKai);
		//vbox->addControl(m_singleTipcontent.get());
		createContent();
		vbox->addControl(m_refContent.get());
		pBox->addControl(vbox);
		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(osgDB::findLibraryFile("resource/arrowd.png"));
		if (image.valid())
		{
			osgEarth::Util::Controls::ImageControl *imgArrow = new osgEarth::Util::Controls::ImageControl(image.get());
			imgArrow->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
			imgArrow->setFixSizeForRotation(true);
			pBox->addControl(imgArrow);
		}
		//m_refCanvas->addControl(m_singleTip.get());
		//m_singleTip->setUpdateCallback(new SingleTipCallback(this));
		m_refAnalysis->setUpdateCallback(new SingleTipCallback(this));
		osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new AddTipOperation(m_refCanvas.get(), m_singleTip.get()));
	}
	m_singleTip->setNodeMask(1);
}

void AnnotationMap::clearSingleTip()
{
	if (m_singleTip.get() != NULL)
	{
		m_singleTip->setNodeMask(0);
	}
}

void AnnotationMap::updateSingleTip()
{
	if (m_singleTip->getNodeMask() == 0) return;
	osgEarth::GeoPoint pnt(getSRS(), m_dTipLon,m_dTipLat,m_dTipAlt);
	osg::Vec3d vWorld;
	pnt.toWorld(vWorld);
	osg::Camera* cam = m_pMapViewer->getOrCreateViewer()->getCamera();
	osg::Matrix VPW = cam->getViewMatrix() *
		cam->getProjectionMatrix() *
		cam->getViewport()->computeWindowMatrix();
	osg::Vec3d vWin = vWorld * VPW;
	float y = cam->getViewport()->height() - vWin.y();
	osgEarth::Util::Controls::ControlContext cx;
	osg::Vec2f vh;
	if (m_bContentChanged)
	{
		createContent();
		m_bContentChanged = false;
	}	
	
	if (m_singleTip.valid()){
		m_singleTip->calcSize(cx, vh);
		m_singleTip->setPosition(vWin.x() - 12, y - vh.y());
	}

}

void AnnotationMap::createContent()
{
	m_fields.clear();
	//std::vector <std::string> fields;
	boost::split(m_fields, m_strSingleTipContent, boost::is_any_of("&"));

	osgText::Font *fntKai = osgText::readFontFile("simkai.ttf");
	if (m_refContent.get() == NULL){
		m_refContent = new osgEarth::Util::Controls::Grid();
		m_refContent->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
		m_refContent->setBorderColor(1, 1, 0, 1);
	}
	else {
		m_refContent->clearControls();
	}

	for (int i = 0; i < m_fields.size(); i += 2)
	{
		osgEarth::Util::Controls::ControlVector vecCtrls;

		std::string ext = m_fields[i].size()>4 ? m_fields[i].substr(m_fields[i].size() - 4, 4) : "";
		if (ext == ".png" || ext == ".jpg" || ext == ".gif" || ext == "jpeg") {
			osg::ref_ptr<osg::Image> imgQzx = osgDB::readImageFile(m_fields[i]);
			osgEarth::Util::Controls::RoundedFrame* ctrl = new osgEarth::Util::Controls::RoundedFrame();
			ctrl->setImage(imgQzx.get());
			vecCtrls.push_back(ctrl);
		}
		else {
			//osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(osgX::ConvertChar::convertLocalStrToUTF8Str(m_fields[i]), osg::Vec4(1, 1, 1, 1), 22);
			osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(m_fields[i], osg::Vec4(1, 1, 1, 1), 22);
			content->setEncoding(osgText::String::ENCODING_UTF8);
			content->setFont(fntKai);
			vecCtrls.push_back(content);
		}

		ext = m_fields[i + 1].size()>4 ? m_fields[i + 1].substr(m_fields[i + 1].size() - 4, 4) : "";
		if (ext == ".png" || ext == ".jpg" || ext == ".gif" || ext == "jpeg") {
			osg::ref_ptr<osg::Image> imgQzx = osgDB::readImageFile(m_fields[i + 1]);
			osgEarth::Util::Controls::RoundedFrame* ctrl = new osgEarth::Util::Controls::RoundedFrame();
			ctrl->setImage(imgQzx.get());
			vecCtrls.push_back(ctrl);
		}
		else {
			//osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(osgX::ConvertChar::convertLocalStrToUTF8Str(m_fields[i+1]), osg::Vec4(1, 1, 1, 1), 22);
			osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(m_fields[i + 1], osg::Vec4(1, 1, 1, 1), 22);
			content->setEncoding(osgText::String::ENCODING_UTF8);
			content->setFont(fntKai);
			vecCtrls.push_back(content);
		}
		m_refContent->addControls(vecCtrls);
	}
}

void AnnotationMap::removeAnnotation( osgEarth::Annotation::AnnotationNode* pAnnotationNode )
{
	if ( pAnnotationNode == NULL )
	{
		return;
	}
	osgEarth::Annotation::ModelNode* mdl = dynamic_cast<osgEarth::Annotation::ModelNode *>(pAnnotationNode);
	if (NULL != mdl)
	{
		removeGroupObjet(m_refModels, pAnnotationNode);
	}
	else {
		osgEarthXCore::FeaturePrimitiveProvider* fp = dynamic_cast<osgEarthXCore::FeaturePrimitiveProvider *>(pAnnotationNode);
		osgEarthXCore::CircleProvider* fp1 = dynamic_cast<osgEarthXCore::CircleProvider *>(pAnnotationNode);
		if (fp || fp1) {
			removeGroupObjet(m_refFeatures, pAnnotationNode);
		}
		else {
			osgEarthXCore::ImageOverlayProvider* over = dynamic_cast<osgEarthXCore::ImageOverlayProvider *>(pAnnotationNode);
			if (over) {
				removeGroupObjet(m_refImgs, pAnnotationNode);
			}
			else {
				osgEarthXCore::PlaceProvider* pla = dynamic_cast<osgEarthXCore::PlaceProvider *>(pAnnotationNode);
				if (pla && pla->getPriority() > 100.0) {
					removeGroupObjet(m_refVIPAnnotations, pAnnotationNode);
				}else
					removeGroupObjet(m_refAnnotations, pAnnotationNode);
			}
		}
	}
}

void AnnotationMap::removeAllAnnotations()
{
	if (m_refAnnotations) {
		removeAllGroupObjet(m_refAnnotations);
		removeAllGroupObjet(m_refVIPAnnotations);
	}
}

class FindAnnotationVisitor : public osg::NodeVisitor
{
public:

	FindAnnotationVisitor(std::string strID) :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),_strID(strID),_objFind(nullptr)
	{
	}

	virtual void apply(osgEarth::Annotation::AnnotationNode& nd)
	{
		if (nd.getName() == _strID) {
			_objFind = &nd;
		}else
			traverse(nd);
	}
	osgEarth::Annotation::AnnotationNode* getFindResult() {
		return _objFind;
	}
protected:
	std::string _strID;
	osgEarth::Annotation::AnnotationNode* _objFind;
};

osgEarth::Annotation::AnnotationNode * AnnotationMap::findAnnotation(std::string strID)
{
	if (m_refAnnotations)
	{
		FindAnnotationVisitor fplv(strID);
		m_refAnnotations->accept(fplv);
		return fplv.getFindResult();
	}
	return nullptr;
}

void AnnotationMap::installPickHover(osgEarth::Util::AnnotationEventHandler * evtHandler)
{
	// install an event handler for picking and hovering.
	
	//osgEarth::Util::AnnotationEventCallback* cbModels = new osgEarth::Util::AnnotationEventCallback();
	//cbModels->addHandler(evtHandler);
	//m_refModels->addEventCallback(cbModels);

	//if(!m_refCB.valid())
	//	m_refCB = new osgEarth::Util::AnnotationEventCallback();
	//osgEarth::Util::AnnotationEventCallback* cb = new osgEarth::Util::AnnotationEventCallback();
	m_refCB->addHandler(evtHandler);
	m_refAnnotations->addEventCallback(m_refCB);
	m_refVIPAnnotations->addEventCallback(m_refCB);
	m_refImgs->addEventCallback(m_refCB);
	m_refFeatures->addEventCallback(m_refCB);

	for (int i = 0; i < m_refModels->getNumChildren(); i++) {
		osgEarth::Annotation::ModelNode* mdl = dynamic_cast<osgEarth::Annotation::ModelNode *>(m_refModels->getChild(i));
		if (mdl) {
			mdl->addEventCallback(m_refCB);
		}
	}
}

osg::Group* osgEarthXCore::AnnotationMap::getModels()
{
	return m_refModels.get();
}

osg::Group* osgEarthXCore::AnnotationMap::getPlaces()
{
	return m_refAnnotations.get();
}

void osgEarthXCore::AnnotationMap::setTipCallback(osgEarthXCore::PlayClickHandler::PlayClickCallback* cb)
{
	m_tipCallback = cb;
}

osgEarth::Util::AnnotationEventCallback* AnnotationMap::getAnnnotationEventCallback()
{
	return m_refCB.get();
}
//void AnnotationMap::installUpdateTipCallback(osgViewer::Viewer* vw)
//{
//    m_refAnnotations->addUpdateCallback(new TipCallback(vw));
//}
