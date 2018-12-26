#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ModularProgram>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/Placer>
#include <osgParticle/SectorPlacer>
#include <osgParticle/LinearInterpolator>
#include <osgParticle/BoxPlacer>

#include <Provider/ExternalProvider/FireProvider.h>

using namespace osgEarthXCore;

#include <osgEarth/MapNode>

FireProvider::FireProvider()
{
	_fireNode = new osg::Group;
	addChild(_fireNode);
}

FireProvider::~FireProvider()
{

}


void FireProvider::initMapNode( osgEarth::MapNode* pMapNode )
{
	_mapNode = pMapNode;
	//creatFire();
	//creatFireInside(this);
	createFireBall(this);
}

void FireProvider::createFireBall(osg::MatrixTransform* fireNode)
{
	// 创建粒子对象，设置其属性并交由粒子系统使用。
	osgParticle::Particle particleTempalte;

	particleTempalte.setShape(osgParticle::Particle::QUAD);
	particleTempalte.setLifeTime(1.5);                             // 单位：秒
	particleTempalte.setSizeRange(osgParticle::rangef(3.0f, 1.0f));   // 单位：米
	particleTempalte.setAlphaRange(osgParticle::rangef(1, 0));
	particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0f, 0.2f, 0.0f, 1.0f),
		osg::Vec4(0.1f, 0.1f, 0.1f, 0)));

	particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setMass(0.1f);                               //单位：千克
	particleTempalte.setRadius(0.2f);

	particleTempalte.setSizeInterpolator(new osgParticle::LinearInterpolator);
	particleTempalte.setAlphaInterpolator(new osgParticle::LinearInterpolator);
	particleTempalte.setColorInterpolator(new osgParticle::LinearInterpolator);

	// 创建并初始化粒子系统。
	osg::ref_ptr<osgParticle::ParticleSystem> particleSystem = new osgParticle::ParticleSystem;
	particleSystem->setDataVariance(osg::Node::STATIC);

	// 设置材质，是否放射粒子，以及是否使用光照。
	particleSystem->setDefaultAttributes(osgDB::findLibraryFile("resource/smoke.rgb"), true, false);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(particleSystem);
	fireNode->addChild(geode);

	//设置为粒子系统的缺省粒子对象。
	particleSystem->setDefaultParticleTemplate(particleTempalte);

	//获取放射极中缺省计数器的句柄，调整每帧增加的新粒子数目
	osg::ref_ptr<osgParticle::RandomRateCounter> particleGenerateRate = new osgParticle::RandomRateCounter();
	particleGenerateRate->setRateRange(100, 500);

	// 每秒新生成的粒子范围
	particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);

	// 自定义一个放置器，这里创建并初始化一个放置器
	osg::ref_ptr<osgParticle::BoxPlacer> particlePlacer = new osgParticle::BoxPlacer;
	particlePlacer->setCenter(osg::Vec3(0.0f, 0.0f, 10.0f));
	particlePlacer->setXRange(-1, 1);
	particlePlacer->setYRange(-1, 1);
	particlePlacer->setZRange(0, 1);
	particlePlacer->setDataVariance(osg::Node::DYNAMIC);

	// 自定义一个弧度发射器
	osg::ref_ptr<osgParticle::RadialShooter> particleShooter = new osgParticle::RadialShooter;

	// 设置发射器的属性
	particleShooter->setDataVariance(osg::Node::DYNAMIC);
	particleShooter->setThetaRange(0.0, osg::PI_4);

	// 弧度值，与Z 轴夹角
	//	particleShooter->setPhiRange(0.0f,osg::PI_2);
	particleShooter->setInitialSpeedRange(10.0f, 20.0f);                          //单位：米/秒

																				  //创建标准放射极对象
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setDataVariance(osg::Node::DYNAMIC);
	emitter->setCullingActive(false);
	emitter->setStartTime(2.3);

	// 将放射极对象与粒子系统关联。
	emitter->setParticleSystem(particleSystem);

	// 设置计数器
	emitter->setCounter(particleGenerateRate);

	// 设置放置器
	emitter->setPlacer(particlePlacer);

	// 设置发射器
	emitter->setShooter(particleShooter);

	// 把放射极添加为变换节点
	fireNode->addChild(emitter);

	// 添加更新器，以实现每帧的粒子管理。
	osg::ref_ptr<osgParticle::ParticleSystemUpdater> particleSystemUpdater = new osgParticle::ParticleSystemUpdater;

	// 将更新器与粒子系统对象关联。
	particleSystemUpdater->addParticleSystem(particleSystem);

	// 将更新器节点添加到场景中。
	fireNode->addChild(particleSystemUpdater);

	// 创建标准编程器对象并与粒子系统相关联。
	osg::ref_ptr<osgParticle::ModularProgram> particleMoveProgram = new osgParticle::ModularProgram;
	particleMoveProgram->setParticleSystem(particleSystem);

	// 最后，将编程器添加到场景中。
	fireNode->addChild(particleMoveProgram);
	fireNode->getOrCreateStateSet()->setRenderBinDetails(101, "RenderBin");
}


void FireProvider::creatFire()
{
	osgParticle::Particle particleTempalte;
	particleTempalte.setShape(osgParticle::Particle::QUAD);
	particleTempalte.setLifeTime(60);
	particleTempalte.setSizeRange(osgParticle::rangef(1.0f, 3.0f));
	particleTempalte.setAlphaRange(osgParticle::rangef(0.1f, 1.0f));
	particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0f, 0.3f, 0.2f, 0.0f),osg::Vec4(1.0f, 0.8f, 0.2f, 1.0f)	));
	particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setMass(0.1);
	particleTempalte.setRadius(2);


	osgParticle::ParticleSystem* particleSystem = new osgParticle::ParticleSystem;
	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(particleSystem);
	_fireNode->addChild(geode);
	particleSystem->setDefaultParticleTemplate(particleTempalte);

	osgParticle::RandomRateCounter* particleGenerateRate = new osgParticle::RandomRateCounter;
	particleGenerateRate->setRateRange(1000, 2000);
	particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);

	osgParticle::SectorPlacer* particlePlacer = new osgParticle::SectorPlacer;
	particlePlacer->setCenter(0, 0, 0);
	particlePlacer->setRadiusRange(0, 10);
	particlePlacer->setPhiRange(0, 2 * osg::PI);

	osgParticle::RadialShooter* particleShooter = new osgParticle::RadialShooter;
	particleShooter->setDataVariance(osg::Node::DYNAMIC);
	particleShooter->setThetaRange(0, osg::PI / 8);
	particleShooter->setPhiRange(-osg::PI, 0);
	particleShooter->setInitialSpeedRange(10, 50);

	osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
	emitter->setDataVariance(osg::Node::DYNAMIC);
	emitter->setCullingActive(false);
	emitter->setParticleSystem(particleSystem);
	emitter->setCounter(particleGenerateRate);
	emitter->setPlacer(particlePlacer);
	emitter->setShooter(particleShooter);
	_fireNode->addChild(emitter);

	osgParticle::ParticleSystemUpdater* particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
	particleSystemUpdater->addParticleSystem(particleSystem);
	_fireNode->addChild(particleSystemUpdater);

	osgParticle::ModularProgram* particleMoveProgram = new osgParticle::ModularProgram;
	particleMoveProgram->setParticleSystem(particleSystem);
	_fireNode->addChild(particleMoveProgram);
}

void FireProvider::creatFireInside(osg::MatrixTransform* nd)
{
	//创建粒子属性
	osgParticle::Particle particleTempalte;
	//创建粒子的形状，可以是POINT、QUAD、QUAD_TRIANGLESTRIP
	particleTempalte.setShape(osgParticle::Particle::QUAD);
	//创建粒子的生存时间秒
	particleTempalte.setLifeTime(10);
	//创建粒子的大小变化范围，从开始1.5f到结束1.2f
	particleTempalte.setSizeRange(osgParticle::rangef(1.5f, 1.2f));
	//设置Alpha值的范围
	particleTempalte.setAlphaRange(osgParticle::rangef(0.1f, 1.0f));
	//设置颜色变化范围
	particleTempalte.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0f, 0.8f, 0.2f, 1.0f),
		osg::Vec4(1.0f, 0.3f, 0.2f, 0.0f)));
	//这几个可以不用管的属性
	particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setMass(0.1);   //粒子的重量
	particleTempalte.setRadius(10);

	//创建粒子系统，自动管理粒子，会根据粒子生命周期来插值处理
	osgParticle::ParticleSystem* particleSystem = new osgParticle::ParticleSystem;
	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(particleSystem);
	nd->addChild(geode);
	particleSystem->setDefaultParticleTemplate(particleTempalte);

	//设置每秒粒子发射的个数
	osgParticle::RandomRateCounter* particleGenerateRate = new osgParticle::RandomRateCounter;
	particleGenerateRate->setRateRange(600, 600);
	particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);

	//设置粒子的放置器，
	osgParticle::SectorPlacer* particlePlacer = new osgParticle::SectorPlacer;
	//设置中心点的坐标(世界坐标)
	particlePlacer->setCenter(0, 0, 0);
	//设置以中心点坐标，0到25的范围，以z轴正向为法向量
	particlePlacer->setRadiusRange(0, 25);
	//设置以x轴的夹角，0->2PI表示是一个圆
	particlePlacer->setPhiRange(0, 2 * osg::PI);

	//粒子发射器
	osgParticle::RadialShooter* particleShooter = new osgParticle::RadialShooter;
	particleShooter->setDataVariance(osg::Node::DYNAMIC);
	//设置粒子与z轴的夹角
	particleShooter->setThetaRange(0, osg::PI / 4);
	//
	particleShooter->setPhiRange(-osg::PI, 0);
	//设置粒子的初始速度
	particleShooter->setInitialSpeedRange(6, 7);

	//设置标准放射极
	osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
	emitter->setDataVariance(osg::Node::DYNAMIC);
	emitter->setCullingActive(false);
	emitter->setParticleSystem(particleSystem);
	emitter->setCounter(particleGenerateRate);
	emitter->setPlacer(particlePlacer);
	emitter->setShooter(particleShooter);
	nd->addChild(emitter);

	//设置粒子更新
	osgParticle::ParticleSystemUpdater* particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
	particleSystemUpdater->addParticleSystem(particleSystem);
	nd->addChild(particleSystemUpdater);

	//这个是设置粒子的物理属性等，但这里没有设置
	osgParticle::ModularProgram* particleMoveProgram = new osgParticle::ModularProgram;
	particleMoveProgram->setParticleSystem(particleSystem);
	nd->addChild(particleMoveProgram);
	nd->getOrCreateStateSet()->setRenderBinDetails(101, "RenderBin");
}

osg::Group* FireProvider::getFireNode()
{
	return _fireNode.get();
}

void FireProvider::setPosition(const osgEarth::GeoPoint& pos)
{
	_mapPoint = pos;
	osg::Matrix matrix;
	pos.createLocalToWorld(matrix);
	setMatrix(matrix);
}

osgEarth::GeoPoint FireProvider::getPosition()
{
	return _mapPoint;
}

void FireProvider::initialized()
{
	
}