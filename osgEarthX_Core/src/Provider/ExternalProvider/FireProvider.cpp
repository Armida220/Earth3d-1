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
	// �������Ӷ������������Բ���������ϵͳʹ�á�
	osgParticle::Particle particleTempalte;

	particleTempalte.setShape(osgParticle::Particle::QUAD);
	particleTempalte.setLifeTime(1.5);                             // ��λ����
	particleTempalte.setSizeRange(osgParticle::rangef(3.0f, 1.0f));   // ��λ����
	particleTempalte.setAlphaRange(osgParticle::rangef(1, 0));
	particleTempalte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0f, 0.2f, 0.0f, 1.0f),
		osg::Vec4(0.1f, 0.1f, 0.1f, 0)));

	particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setMass(0.1f);                               //��λ��ǧ��
	particleTempalte.setRadius(0.2f);

	particleTempalte.setSizeInterpolator(new osgParticle::LinearInterpolator);
	particleTempalte.setAlphaInterpolator(new osgParticle::LinearInterpolator);
	particleTempalte.setColorInterpolator(new osgParticle::LinearInterpolator);

	// ��������ʼ������ϵͳ��
	osg::ref_ptr<osgParticle::ParticleSystem> particleSystem = new osgParticle::ParticleSystem;
	particleSystem->setDataVariance(osg::Node::STATIC);

	// ���ò��ʣ��Ƿ�������ӣ��Լ��Ƿ�ʹ�ù��ա�
	particleSystem->setDefaultAttributes(osgDB::findLibraryFile("resource/smoke.rgb"), true, false);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(particleSystem);
	fireNode->addChild(geode);

	//����Ϊ����ϵͳ��ȱʡ���Ӷ���
	particleSystem->setDefaultParticleTemplate(particleTempalte);

	//��ȡ���伫��ȱʡ�������ľ��������ÿ֡���ӵ���������Ŀ
	osg::ref_ptr<osgParticle::RandomRateCounter> particleGenerateRate = new osgParticle::RandomRateCounter();
	particleGenerateRate->setRateRange(100, 500);

	// ÿ�������ɵ����ӷ�Χ
	particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);

	// �Զ���һ�������������ﴴ������ʼ��һ��������
	osg::ref_ptr<osgParticle::BoxPlacer> particlePlacer = new osgParticle::BoxPlacer;
	particlePlacer->setCenter(osg::Vec3(0.0f, 0.0f, 10.0f));
	particlePlacer->setXRange(-1, 1);
	particlePlacer->setYRange(-1, 1);
	particlePlacer->setZRange(0, 1);
	particlePlacer->setDataVariance(osg::Node::DYNAMIC);

	// �Զ���һ�����ȷ�����
	osg::ref_ptr<osgParticle::RadialShooter> particleShooter = new osgParticle::RadialShooter;

	// ���÷�����������
	particleShooter->setDataVariance(osg::Node::DYNAMIC);
	particleShooter->setThetaRange(0.0, osg::PI_4);

	// ����ֵ����Z ��н�
	//	particleShooter->setPhiRange(0.0f,osg::PI_2);
	particleShooter->setInitialSpeedRange(10.0f, 20.0f);                          //��λ����/��

																				  //������׼���伫����
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setDataVariance(osg::Node::DYNAMIC);
	emitter->setCullingActive(false);
	emitter->setStartTime(2.3);

	// �����伫����������ϵͳ������
	emitter->setParticleSystem(particleSystem);

	// ���ü�����
	emitter->setCounter(particleGenerateRate);

	// ���÷�����
	emitter->setPlacer(particlePlacer);

	// ���÷�����
	emitter->setShooter(particleShooter);

	// �ѷ��伫���Ϊ�任�ڵ�
	fireNode->addChild(emitter);

	// ��Ӹ���������ʵ��ÿ֡�����ӹ���
	osg::ref_ptr<osgParticle::ParticleSystemUpdater> particleSystemUpdater = new osgParticle::ParticleSystemUpdater;

	// ��������������ϵͳ���������
	particleSystemUpdater->addParticleSystem(particleSystem);

	// ���������ڵ���ӵ������С�
	fireNode->addChild(particleSystemUpdater);

	// ������׼���������������ϵͳ�������
	osg::ref_ptr<osgParticle::ModularProgram> particleMoveProgram = new osgParticle::ModularProgram;
	particleMoveProgram->setParticleSystem(particleSystem);

	// ��󣬽��������ӵ������С�
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
	//������������
	osgParticle::Particle particleTempalte;
	//�������ӵ���״��������POINT��QUAD��QUAD_TRIANGLESTRIP
	particleTempalte.setShape(osgParticle::Particle::QUAD);
	//�������ӵ�����ʱ����
	particleTempalte.setLifeTime(10);
	//�������ӵĴ�С�仯��Χ���ӿ�ʼ1.5f������1.2f
	particleTempalte.setSizeRange(osgParticle::rangef(1.5f, 1.2f));
	//����Alphaֵ�ķ�Χ
	particleTempalte.setAlphaRange(osgParticle::rangef(0.1f, 1.0f));
	//������ɫ�仯��Χ
	particleTempalte.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0f, 0.8f, 0.2f, 1.0f),
		osg::Vec4(1.0f, 0.3f, 0.2f, 0.0f)));
	//�⼸�����Բ��ùܵ�����
	particleTempalte.setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setVelocity(osg::Vec3(0.0f, 0.0f, 0.0f));
	particleTempalte.setMass(0.1);   //���ӵ�����
	particleTempalte.setRadius(10);

	//��������ϵͳ���Զ��������ӣ����������������������ֵ����
	osgParticle::ParticleSystem* particleSystem = new osgParticle::ParticleSystem;
	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(particleSystem);
	nd->addChild(geode);
	particleSystem->setDefaultParticleTemplate(particleTempalte);

	//����ÿ�����ӷ���ĸ���
	osgParticle::RandomRateCounter* particleGenerateRate = new osgParticle::RandomRateCounter;
	particleGenerateRate->setRateRange(600, 600);
	particleGenerateRate->setDataVariance(osg::Node::DYNAMIC);

	//�������ӵķ�������
	osgParticle::SectorPlacer* particlePlacer = new osgParticle::SectorPlacer;
	//�������ĵ������(��������)
	particlePlacer->setCenter(0, 0, 0);
	//���������ĵ����꣬0��25�ķ�Χ����z������Ϊ������
	particlePlacer->setRadiusRange(0, 25);
	//������x��ļнǣ�0->2PI��ʾ��һ��Բ
	particlePlacer->setPhiRange(0, 2 * osg::PI);

	//���ӷ�����
	osgParticle::RadialShooter* particleShooter = new osgParticle::RadialShooter;
	particleShooter->setDataVariance(osg::Node::DYNAMIC);
	//����������z��ļн�
	particleShooter->setThetaRange(0, osg::PI / 4);
	//
	particleShooter->setPhiRange(-osg::PI, 0);
	//�������ӵĳ�ʼ�ٶ�
	particleShooter->setInitialSpeedRange(6, 7);

	//���ñ�׼���伫
	osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
	emitter->setDataVariance(osg::Node::DYNAMIC);
	emitter->setCullingActive(false);
	emitter->setParticleSystem(particleSystem);
	emitter->setCounter(particleGenerateRate);
	emitter->setPlacer(particlePlacer);
	emitter->setShooter(particleShooter);
	nd->addChild(emitter);

	//�������Ӹ���
	osgParticle::ParticleSystemUpdater* particleSystemUpdater = new osgParticle::ParticleSystemUpdater;
	particleSystemUpdater->addParticleSystem(particleSystem);
	nd->addChild(particleSystemUpdater);

	//������������ӵ��������Եȣ�������û������
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