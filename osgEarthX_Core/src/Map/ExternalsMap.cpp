#include <Map/ExternalsMap.h>
#include <osgParticle/PrecipitationEffect>

using namespace osgEarthXCore;

#include <ThreadPool/LockMutex.h>

//OSGX
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>



//Operations
void SetSkyProviderOperation::operator()( osg::Object* pObject )
{
	m_pExternalsMap->_setSkyProvider( m_pSkyProvider );
}

void RemoveSkyProviderOperation::operator()( osg::Object* pObject )
{
	m_pExternalsMap->_removeSkyProvider();
}

void SetGraticuleProviderOperation::operator() ( osg::Object* pObject )
{
	m_pExternalsMap->_setGraticuleProvider( m_pGraticuleProvider );
}

void RemoveGraticuleProviderOperation::operator()( osg::Object* pObject )
{
	m_pExternalsMap->_removeGraticuleProvider();
}

void SetCasterOperation::operator()(osg::Object* pObject)
{
	m_pExternalsMap->_setCaster();
}

void ResetCasterOperation::operator()(osg::Object* pObject)
{
	m_pExternalsMap->_resetCaster();
}

ExternalsMap::ExternalsMap()
{

}

ExternalsMap::~ExternalsMap()
{

}

void ExternalsMap::initMapViewer( MapViewer* pMapViewer )
{
	BaseMap::initMapViewer( pMapViewer );

	if ( m_refSkyProvider != NULL && pMapViewer != NULL )
	{
		m_refSkyProvider->getSkyNode()->attach( pMapViewer->getOrCreateViewer() );
	}
}

void ExternalsMap::setSkyProvider( SkyProvider* pSkyProvider )
{
	if ( pSkyProvider == NULL || pSkyProvider == m_refSkyProvider.get() )// NULL or equal
	{
		return;
	}

	_removeSkyProvider();//remove old sky.
	m_refSkyProvider = pSkyProvider;
	m_refSkyProvider->initSky( m_refMapNode );

	if ( m_pMapViewer != NULL )
	{
		m_refSkyProvider->getSkyNode()->attach( m_pMapViewer->getOrCreateViewer() );
	}

	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new SetSkyProviderOperation( this, pSkyProvider ) );
}

void ExternalsMap::_setSkyProvider( SkyProvider* pSkyProvider )
{
	SCOPEDLOCK_WRITE_MUTEX( m_MapMutex );
	m_refRoot->replaceChild( m_refMapNode, m_refSkyProvider->getSkyNode() );
}

void ExternalsMap::_setCaster()
{
	SCOPEDLOCK_WRITE_MUTEX(m_MapMutex);
	m_refRoot->replaceChild(m_refSkyProvider->getSkyNode(), m_refCaster);
}

void ExternalsMap::_resetCaster()
{
	SCOPEDLOCK_WRITE_MUTEX(m_MapMutex);
	m_refRoot->replaceChild(m_refCaster,m_refSkyProvider->getSkyNode());
}


void ExternalsMap::setupViewShed(osg::Vec3d pos, osg::Vec3 dir)
{
	//double xw, yw, zw;
	//m_refMapNode->getMap()->getSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::DegreesToRadians(pos.y()), osg::DegreesToRadians(pos.x()), pos.z(), xw, yw, zw);
	//osg::Vec3d lightPosW(xw, yw, zw); //空间直角坐标系下的坐标。

	//osg::Matrixd matLocalToWorld;
	//m_refMapNode->getMap()->getSRS()->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(pos.y()), osg::DegreesToRadians(pos.x()), pos.z(), matLocalToWorld);

	//osg::Vec3 localDir(3.0f, 0.0f, -1.0f);
	//osg::Vec3 transDir = osg::Matrixd::transform3x3(localDir, matLocalToWorld);
	//transDir.normalize();

	//int unit;
	//if (m_refMapNode->getTerrainEngine()->getTextureCompositor()->reserveTextureImageUnit(unit))
	//{
	//	if (!m_refCaster.valid()) {
	//		m_refCaster = new osgEarth::Util::ShadowCaster();
	//		m_refCaster->setBlurFactor(0.0);
	//		m_refCaster->setShadowColor(0.0);
	//		m_refCaster->setTextureImageUnit(unit);

	//		m_refCaster->getShadowCastingGroup()->addChild(m_refModels);
	//		m_refCaster->addChild(m_refMapNode);
	//	}

	//	m_refCaster->setLightPosition(lightPosW);
	//	osg::ref_ptr<osg::Light> light = new osg::Light;
	//	light->setLightNum(2);
	//	light->setDiffuse(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	//	light->setDirection(transDir);
	//	m_refCaster->setLight(light);
	//	m_refCaster->buildLightGeometry();
	//	
	//}
}

void ExternalsMap::UpdateShadowCaster(const osg::Vec3d& pos, double heading, double tilt, double range)
{
	//if (!m_refMapNode.valid()) 
	//	return;

	//if (!m_refCaster.valid())
	//{
	//	osg::Vec3d local(0.0f, 1.0f, 0.0f);
	//	setupViewShed(pos, local);
	//}
	//double xw, yw, zw;
	//m_refMapNode->getMap()->getSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::DegreesToRadians(pos.y()), osg::DegreesToRadians(pos.x()), pos.z(), xw, yw, zw);
	//osg::Vec3d lightPosW(xw, yw, zw); //空间直角坐标系下的坐标。
	//								  //caster->setLightPosition(lightPosW);

	//osg::Matrixd matLocalToWorld;
	//m_refMapNode->getMap()->getSRS()->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(pos.y()), osg::DegreesToRadians(pos.x()), pos.z(), matLocalToWorld);

	//osg::Vec3d localDir(0.0f, 1.0f, 0.0f); //指向本地坐标系正北
	//									   //osg::Matrixd matRotZ;
	//									   //matRotZ.makeRotate(osg::Quat(heading, osg::Vec3(0, 0, 1)));
	//									   //osg::Matrixd matRotX;
	//									   //matRotX.makeRotate(osg::Quat(heading, osg::Vec3(1, 0, 0)));

	//osg::Matrix matRot = osg::Matrix::rotate(osg::DegreesToRadians(tilt), osg::Vec3d(1, 0, 0), 0, osg::Vec3d(0, 1, 0), osg::DegreesToRadians(heading), osg::Vec3d(0, 0, 1));

	////osg::Matrixd matRot = matRotZ * matRotX;

	//osg::Vec3 transDir = osg::Matrixd::transform3x3(localDir, matRot * matLocalToWorld);
	//transDir.normalize();
	////caster->getLight()->setDirection(transDir);
	//m_refCaster->UpdateLight(lightPosW, transDir, range);
	//osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new SetCasterOperation(this));
}

void ExternalsMap::stopViewShed()
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new ResetCasterOperation(this));
	//m_refRoot->replaceChild(m_refCaster, m_refSkyProvider->getSkyNode());
}

void osgEarthXCore::ExternalsMap::setFog(bool bFog)
{
	if (bFog) {
		if (!m_refFogEffect.valid()) {
				m_refFogEffect = new osgEarth::Util::FogEffect;
				m_refFogEffect->attach(m_refRoot->getOrCreateStateSet());
			}

			float maxDensity = 0.000125;
			float fogStartHeight = 10000.0f;

			// Setup a Fog state attribute
			osg::Vec4 fogColor(0.66f, 0.7f, 0.81f, 1.0f);
			if (!m_refFog.valid()) {
				m_refFog = new osg::Fog;
				m_refFog->setColor(fogColor); //viewer.getCamera()->getClearColor() );    
			}
									 // Get the height above the terrain
			osg::Vec3d eye, center, up;
			m_pMapViewer->getOrCreateViewer()->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			osgEarth::GeoPoint map;
			map.fromWorld(m_refMapNode->getMapSRS(), eye);

			// Compute the fog density based on the camera height
			float ratio = ((fogStartHeight - map.z()) / fogStartHeight);
			ratio = osg::clampBetween(ratio, 0.0f, 1.0f);
			float density = ratio * maxDensity;
			m_refFog->setDensity(density);

			//fog->setDensity(0);
			m_refRoot->getOrCreateStateSet()->setAttributeAndModes(m_refFog, osg::StateAttribute::ON);
	}
	else {
		if(m_refFog.valid())
			m_refRoot->getOrCreateStateSet()->setAttributeAndModes(m_refFog, osg::StateAttribute::OFF);
	}
}

void osgEarthXCore::ExternalsMap::setSnow(bool bSnow)
{
	if (!m_refSnow.valid()) {
		//创建雪花对象  
		m_refSnow = new osgParticle::PrecipitationEffect();
		//设置雪花浓度  
		m_refSnow->snow(0.5);
		//设置雪花颜色  
		m_refSnow->setParticleColor(osg::Vec4(1, 1, 1, 1));
		m_refSnow->setUseFarLineSegments(true);
		//设置雪花方向  
		m_refSnow->setWind(osg::Vec3(2, 0, 0));
	}
	if (bSnow) {
		addGroupObject(m_refMapNode, m_refSnow);
		//m_refMapNode->addChild(precipationEffect);
	}else {
		removeGroupObjet(m_refMapNode, m_refSnow);
	}
}

void osgEarthXCore::ExternalsMap::setRain(bool bRain)
{
	if (!m_refRain.valid()) {
		//创建雨对象  
		m_refRain = new osgParticle::PrecipitationEffect();
		//设置雨浓度  
		m_refRain->rain(0.6);
		//设置雨颜色  
		m_refRain->setParticleColor(osg::Vec4(1, 1, 1, 1));
		m_refRain->setUseFarLineSegments(true);
		//设置雨落方向  
		//precipationEffect->setWind(osg::Vec3(2, 0, 0));
	}
	if (bRain) {
		addGroupObject(m_refMapNode, m_refRain);
		//m_refMapNode->addChild(precipationEffect);
	}
	else {
		removeGroupObjet(m_refMapNode, m_refRain);
	}
}

SkyProvider* ExternalsMap::getSkyProvider()
{
	return m_refSkyProvider;
}

void ExternalsMap::removeSkyProvider()
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new RemoveSkyProviderOperation( this ) );
}

void ExternalsMap::_removeSkyProvider()
{
	if ( m_refSkyProvider == NULL )
	{
		return;
	}

	SCOPEDLOCK_WRITE_MUTEX(m_MapMutex);
	m_refRoot->replaceChild( m_refSkyProvider->getSkyNode(), m_refMapNode );
	m_refSkyProvider = NULL;
}

GraticuleProvider* ExternalsMap::getGraticuleProvider()
{
	return m_refGraticuleProvider;
}

void ExternalsMap::setGraticuleProvider( GraticuleProvider* pGraticuleProvider )
{
	if( pGraticuleProvider == NULL || pGraticuleProvider == m_refGraticuleProvider )
	{
		return;
	}

	_removeGraticuleProvider();
	m_refGraticuleProvider = pGraticuleProvider;
	m_refGraticuleProvider->initGeodeticGraticule( m_refMapNode );
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new SetGraticuleProviderOperation( this, m_refGraticuleProvider ) );
}

void ExternalsMap::_setGraticuleProvider( GraticuleProvider* pGraticuleProvider )
{
	SCOPEDLOCK_WRITE_MUTEX(m_MapMutex);
	m_refRoot->addChild( pGraticuleProvider->getGeodeticGraticule() );
}

void ExternalsMap::removeGraticuleProvider()
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation( new RemoveGraticuleProviderOperation( this ) );
}

void ExternalsMap::_removeGraticuleProvider()
{
	if ( m_refGraticuleProvider == NULL )
	{
		return;
	}

	SCOPEDLOCK_WRITE_MUTEX(m_MapMutex);
	m_refRoot->removeChild( m_refGraticuleProvider->getGeodeticGraticule() );
	m_refGraticuleProvider = NULL;
}

using namespace osgEarth::Util;
using namespace osgEarth::Symbology;
namespace ui = osgEarth::Util::Controls;
using namespace ui;

#define SET_FLOAT( effect, func ) \
struct func : public ui::ControlEventHandler { \
    App& _app; \
    func (App& app) : _app(app) { } \
    void onValueChanged(ui::Control*, float value) { \
        _app. effect -> func (value); \
    } \
};

#define TOGGLE( effect ) \
struct Toggle : public ui::ControlEventHandler { \
    App& _app; \
    Toggle (App& app) : _app(app) { } \
    void onValueChanged(ui::Control*, bool value) { \
        if ( value ) _app.engine->addEffect( _app. effect .get() ); \
        else _app.engine->removeEffect( _app. effect .get() ); \
    } \
};

struct ContourMapController {
	TOGGLE(contourMap);
	SET_FLOAT(contourMap, setOpacity);
	ContourMapController(App& app, ui::Grid* grid) {
		int r = grid->getNumRows();
		grid->setControl(0, r, new ui::LabelControl("ContourMap"));
		grid->setControl(1, r, new ui::CheckBoxControl(false, new Toggle(app)));
		++r;
		grid->setControl(0, r, new ui::LabelControl("   opacity:"));
		grid->setControl(1, r, new ui::HSliderControl(0.0, 1.0, 0.5, new setOpacity(app)));
		grid->setControl(2, r, new ui::LabelControl(grid->getControl(1, r)));
	}
};

struct LODBlendingController {
	TOGGLE(lodBlending);
	SET_FLOAT(lodBlending, setDelay);
	SET_FLOAT(lodBlending, setDuration);
	SET_FLOAT(lodBlending, setVerticalScale);
	LODBlendingController(App& app, ui::Grid* grid) {
		int r = grid->getNumRows();
		grid->setControl(0, r, new ui::LabelControl("LOD Blending"));
		grid->setControl(1, r, new ui::CheckBoxControl(false, new Toggle(app)));
		++r;
		grid->setControl(0, r, new ui::LabelControl("   delay:"));
		grid->setControl(1, r, new ui::HSliderControl(0.0, 2.0, 1.0, new setDelay(app)));
		grid->setControl(2, r, new ui::LabelControl(grid->getControl(1, r)));
		++r;
		grid->setControl(0, r, new ui::LabelControl("   duration:"));
		grid->setControl(1, r, new ui::HSliderControl(0.0, 5.0, 1.0, new setDuration(app)));
		grid->setControl(2, r, new ui::LabelControl(grid->getControl(1, r)));
		++r;
		grid->setControl(0, r, new ui::LabelControl("   vertical scale:"));
		grid->setControl(1, r, new ui::HSliderControl(0.0, 5.0, 1.0, new setVerticalScale(app)));
		grid->setControl(2, r, new ui::LabelControl(grid->getControl(1, r)));
	}
};

struct NormalMapController {
	TOGGLE(normalMap);
	NormalMapController(App& app, ui::Grid* grid) {
		int r = grid->getNumRows();
		grid->setControl(0, r, new ui::LabelControl("NormalMap"));
		grid->setControl(1, r, new ui::CheckBoxControl(false, new Toggle(app)));
	}
};

struct VerticalScaleController {
	TOGGLE(verticalScale);
	SET_FLOAT(verticalScale, setScale);
	VerticalScaleController(App& app, ui::Grid* grid) {
		int r = grid->getNumRows();
		grid->setControl(0, r, new ui::LabelControl("VerticalScale"));
		grid->setControl(1, r, new ui::CheckBoxControl(false, new Toggle(app)));
		++r;
		grid->setControl(0, r, new ui::LabelControl("   scale:"));
		grid->setControl(1, r, new ui::HSliderControl(0.0, 10.0, 1.0, new setScale(app)));
		grid->setControl(2, r, new ui::LabelControl(grid->getControl(1, r)));
	}
};


// Build a slider to adjust the vertical scale
ui::Control* createUI(App& app)
{
	ui::Grid* grid = new ui::Grid();
	grid->setAbsorbEvents(true);
	grid->setControl(0, 0, new LabelControl("Terrain Effects", Color::Yellow));
	grid->setControl(1, 0, new LabelControl(""));
	grid->getControl(1, 0)->setHorizFill(true, 250);

	ContourMapController(app, grid);
	LODBlendingController(app, grid);
	NormalMapController(app, grid);
	VerticalScaleController(app, grid);

	return grid;
}

void osgEarthXCore::ExternalsMap::setTerrainEffect(bool bEffect)
{
	if (!m_refGridTerrain.valid()) {
		m_refGridTerrain = (ui::Grid*)createUI(m_Terrain);
		m_Terrain.engine = m_refMapNode->getTerrainEngine();
	}
	if (!m_refCanvas.valid())
		m_refCanvas = osgEarth::Util::Controls::ControlCanvas::getOrCreate(m_pMapViewer->getOrCreateViewer());
	if (bEffect) {
		m_refGridTerrain->setBackColor(Color(Color::Black, 0.8));
		m_refGridTerrain->setHorizAlign(Control::ALIGN_RIGHT);
		m_refGridTerrain->setVertAlign(Control::ALIGN_BOTTOM);
		m_refCanvas->addControl(m_refGridTerrain.get());
	}
	else {
		m_refCanvas->removeControl(m_refGridTerrain);
	}
}

#include <osgEarthUtil/LinearLineOfSight>
#include <osgEarthUtil/RadialLineOfSight>

void osgEarthXCore::ExternalsMap::LineOfSight(double dlon, double dlat, double dAlt, double dlonEnd, double dlatEnd, double dAltEnd)
{
	const SpatialReference* mapSRS = m_refMapNode->getMapSRS();
	const SpatialReference* geoSRS = mapSRS->getGeographicSRS();
	LinearLineOfSightNode* relativeLOS = new LinearLineOfSightNode(
		m_refMapNode,
		GeoPoint(geoSRS, dlon, dlat, dAlt+10, ALTMODE_ABSOLUTE),
		GeoPoint(geoSRS, dlonEnd, dlatEnd, dAltEnd+10, ALTMODE_ABSOLUTE));
	addGroupObject(m_refAnalysis, relativeLOS);
	relativeLOS->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

}

void osgEarthXCore::ExternalsMap::RadialLineOfSight(double dlon, double dlat, double dAlt, double dr)
{
	RadialLineOfSightNode* radial = new RadialLineOfSightNode(m_refMapNode);
	const SpatialReference* mapSRS = m_refMapNode->getMapSRS();
	const SpatialReference* geoSRS = mapSRS->getGeographicSRS();
	radial->setCenter(GeoPoint(geoSRS, dlon, dlat, dAlt +20, ALTMODE_ABSOLUTE));
	radial->setRadius(dr);
	radial->setNumSpokes(100);
	radial->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	addGroupObject(m_refAnalysis, radial);
}

//Creates a simple HUD camera
osg::Camera* createHud(double width, double height)
{
	osg::Camera* hud = new osg::Camera;
	hud->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
	hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hud->setViewMatrix(osg::Matrix::identity());
	hud->setClearMask(GL_DEPTH_BUFFER_BIT);
	hud->setRenderOrder(osg::Camera::POST_RENDER);
	hud->setAllowEventFocus(false);
	hud->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	hud->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	hud->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	return hud;
}

void osgEarthXCore::ExternalsMap::TerrainProfile(double dlon, double dlat, double dlonEnd, double dlatEnd, bool bShow)
{
	if (!m_refProfileCamera.valid()) {
		double backgroundWidth = 500;
		double backgroundHeight = 500;

		double graphWidth = 200;
		double graphHeight = 100;

		//Add the hud
		m_refProfileCamera = createHud(backgroundWidth, backgroundHeight);

		m_refCalculator = new TerrainProfileCalculator(m_refMapNode,
			GeoPoint(m_refMapNode->getMapSRS(), dlon, dlat),
			GeoPoint(m_refMapNode->getMapSRS(), dlonEnd, dlatEnd)
		);

		osg::ref_ptr<TerrainProfileGraph> refProfile = new TerrainProfileGraph(m_refCalculator.get(), graphWidth, graphHeight);
		m_refProfileCamera->addChild(refProfile.get());

	}

	if(bShow){
		removeGroupObjet(m_refRoot, m_refProfileCamera.get());
		m_refCalculator->setStartEnd(GeoPoint(m_refMapNode->getMapSRS(), dlon, dlat),
			GeoPoint(m_refMapNode->getMapSRS(), dlonEnd, dlatEnd));
		addGroupObject(m_refRoot, m_refProfileCamera.get());		
	}
	else {
		removeGroupObjet(m_refRoot, m_refProfileCamera.get());
	}
}

#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/agglite/AGGLiteOptions>

//#include <osgEarthAnnotation/ModelNode>
//#include <osgEarthSymbology\Geometry>
//
//#include <osgCal/CoreModel>
//#include <osgCal/Model>
//
//osg::Node*
//makeModel1(osgCal::CoreModel* cm,
//	osgCal::BasicMeshAdder* ma,
//	int animNum = -1)
//{
//	osgCal::Model* model = new osgCal::Model();
//
//	model->load(cm, ma);
//
//	if (animNum != -1)
//	{
//		model->blendCycle(animNum, 1.0f, 0);
//	}
//
//	return model;
//}
//
//osg::Node* createPeople(osgEarth::MapNode* mapNode)
//{
//	std::vector< std::string > animationNames;
//	osg::ref_ptr< osgCal::CoreModel > coreModel(new osgCal::CoreModel());
//	int         animNum = 3;
//	osg::ref_ptr< osgCal::BasicMeshAdder > meshAdder(new osgCal::DefaultMeshAdder);
//	osg::ref_ptr< osgCal::MeshParameters > p(new osgCal::MeshParameters);
//	//coreModel->load("C:/prjs/osgEarthX/test_models/paladin/paladin.cfg", p.get());
//	coreModel->load("C:/prjs/osgEarthX/test_models/cally/cal3d.cfg", p.get());
//	//coreModel->load(calfile, p.get());
//	osg::ref_ptr< osg::Node > ndPeople = makeModel1(coreModel.get(), meshAdder.get(), animNum);
//	//animationNames = coreModel->getAnimationNames();
//
//	Style st;
//	st.getOrCreate<ModelSymbol>()->setModel(ndPeople);
//
//	//make a ModelNode:
//	osgEarth::Annotation::ModelNode* modelNode = new osgEarth::Annotation::ModelNode(mapNode, st);
//
//	const SpatialReference* mapSRS = mapNode->getMapSRS();
//	const SpatialReference* geoSRS = mapSRS->getGeographicSRS();
//	modelNode->setPosition(GeoPoint(geoSRS, 117.176553772, 39.15775072669, 15.0, ALTMODE_RELATIVE));
//	//vector<GeoPoint> pnts;
//	//pnts.push_back(GeoPoint(geoSRS, 120.363588, 40.286867, 5.0, ALTMODE_RELATIVE));
//	//pnts.push_back(GeoPoint(geoSRS, 120.361942, 40.292635, 5.0, ALTMODE_RELATIVE));
//	//pnts.push_back(GeoPoint(geoSRS, 120.358584, 40.292331, 5.0, ALTMODE_RELATIVE));
//	//pnts.push_back(GeoPoint(geoSRS, 120.357691, 40.300731, 5.0, ALTMODE_RELATIVE));
//
//	//modelNode->setUpdateCallback(new PeopleCallback(pnts));
//
//	//viewer.addEventHandler(new AnimationToggleHandler((osgCal::Model*)ndPeople.get(), animationNames));
//	return modelNode;
//}

void osgEarthXCore::ExternalsMap::ClearCalResult()
{
	m_refAnalysis->removeChildren(0, m_refAnalysis->getNumChildren());

	//osg::Node* nd = createPeople(m_refMapNode);
	//m_refRoot->addChild(nd);

	//osgEarth::Drivers::OGRFeatureOptions featureOptions;
	//featureOptions.url() = "C:\\resuo\\budaiditu\\road.shp";
	//
	//Style style;
	//LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
	//ls->stroke()->color() = Color::Yellow;
	//ls->stroke()->width() = 2.0f;

	//style.getOrCreateSymbol<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
	//style.getOrCreateSymbol<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

	//osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	//geomOptions.featureOptions() = featureOptions;
	//geomOptions.styles() = new StyleSheet();
	//geomOptions.styles()->addStyle(style);

	//osgEarth::Symbology::Style style;
	//style.setName("default");
	//osgEarth::Symbology::LineSymbol* lineSymbol = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	//lineSymbol->stroke()->color() = osgEarth::Symbology::Color::White;
	//lineSymbol->stroke()->width() = 20.5f;
	//lineSymbol->stroke()->widthUnits() = osgEarth::Units::METERS;

	//// Clamp the lines to the terrain.
	//osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	//alt->clamping() = alt->CLAMP_TO_TERRAIN;
	//alt->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

	//geomOptions.styles() = new osgEarth::Symbology::StyleSheet();
	//geomOptions.styles()->addStyle(style);

	//osgEarth::Drivers::AGGLiteOptions rasterOptions;
	//rasterOptions.featureOptions() = featureOptions;
	//rasterOptions.styles() = new StyleSheet();
	//rasterOptions.styles()->addStyle(style);
	//osgEarth::Map* map = m_refMapNode->getMap();
	//map->addImageLayer(new ImageLayer("my features", rasterOptions));

	//geomOptions.enableLighting() = false;
	//osgEarth::Map* map = m_refMapNode->getMap();
	//ModelLayerOptions layerOptions("my features", geomOptions);
	//map->addModelLayer(new ModelLayer(layerOptions));
}

void osgEarthXCore::ExternalsMap::ClearScene()
{
	int n = m_refRoot->getNumChildren();
	m_refRoot->removeChildren(0, n);
}
