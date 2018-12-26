#include <Provider/AnnotationProvider/ModelProvider.h>
using namespace osgEarthXCore;


#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgEarthSymbology/AltitudeSymbol>
#include <osgUtil/Optimizer>

osgCal::Model* makeModel(osgCal::CoreModel* cm,
	osgCal::BasicMeshAdder* ma,
	int animNum = -1)
{
	osgCal::Model* model = new osgCal::Model();

	model->load(cm, ma);

	if (animNum != -1)
	{
		model->blendCycle(animNum, 1.0f, 0);
	}

	return model;
}

ModelProvider::ModelProvider()
{
	m_iCurAction = 3;
}

ModelProvider::ModelProvider(const ModelProvider& modelProvider) :
	osgEarth::Annotation::ModelNode(modelProvider),
	osgEarth::Symbology::ModelSymbol(modelProvider)
{
	m_iCurAction = 3;
}

ModelProvider::ModelProvider(const osgEarth::Annotation::ModelNode& modelNode) :
	osgEarth::Annotation::ModelNode(modelNode)
{
	m_iCurAction = 3;
}

ModelProvider::ModelProvider(const osgEarth::Symbology::ModelSymbol& modelSymbol) :
	osgEarth::Symbology::ModelSymbol(modelSymbol)
{
	m_iCurAction = 3;
}

ModelProvider::~ModelProvider()
{
}

void ModelProvider::initMapNode(osgEarth::MapNode* pMapNode)
{
	ModelNode::initMapNode(pMapNode);
	_style.addSymbol(this);
	//_style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//_style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//_style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_VERTEX;

	ModelNode::init();
}

void ModelProvider::setModelPath(const std::string& modelPath)
{
	std::string ext = osgDB::getLowerCaseFileExtension(modelPath);
	if (ext == "cfg") {
		std::vector< std::string > animationNames;
		osg::ref_ptr< osgCal::CoreModel > coreModel(new osgCal::CoreModel());
		osg::ref_ptr< osgCal::BasicMeshAdder > meshAdder(new osgCal::DefaultMeshAdder);
		osg::ref_ptr< osgCal::MeshParameters > p(new osgCal::MeshParameters);
		coreModel->load(modelPath, p.get());
		m_ndPeople = makeModel(coreModel.get(), meshAdder.get(), m_iCurAction);

		//osgEarth::Symbology::Style st;
		//st.getOrCreate<ModelSymbol>()->setModel(m_ndPeople);
		//setStyle(st);

		setModel(m_ndPeople);
		//m_ndPeople->blendCycle(m_iCurAction, 1.0f, 1.0);
	}
	else {
		setModel(osgDB::readNodeFile(modelPath));
	}
	m_modelPath = modelPath;
}

void ModelProvider::setAction(int iAct)
{
	m_iCurAction = iAct;
	if (m_ndPeople) {
		if (m_iCurAction != -1)
		{
			m_ndPeople->clearCycle(m_iCurAction, 1.0); // clear in 1sec
		}
		m_ndPeople->blendCycle(m_iCurAction, 1.0f, 1.0);
	}
}

int osgEarthXCore::ModelProvider::getAction()
{
	return 0;
}

const std::string& ModelProvider::getModelPath()
{
	return m_modelPath;
}

void ModelProvider::setEditor(osg::Group* con)
{
	m_Editors = con;
}

void ModelProvider::startEdit()
{
	if (m_refEditor.get() == NULL) {
		m_refEditor = new PlaceNodeEditor(this);
	}
	if (!m_Editors->containsNode(m_refEditor.get()))
		m_Editors->addChild(m_refEditor.get());
	setCullingActive(false);
}


void ModelProvider::stopEdit()
{
	setCullingActive(true);
	if (m_refEditor.get())
		if (m_Editors->containsNode(m_refEditor.get()))
			m_Editors->removeChild(m_refEditor.get());
}

void ModelProvider::clearAttachedEditor()
{
	if (m_Editors.get() && m_refEditor.get())
		if (m_Editors->containsNode(m_refEditor.get()))
			m_Editors->removeChild(m_refEditor.get());
}

void osgEarthXCore::ModelProvider::splitShow()
{
	if (!m_Components.valid()) {
		m_Components = new osg::Group;

		osg::ref_ptr<osg::Group> grp = dynamic_cast<osg::Group*>(_node.get());
		if (grp) {
			if (grp->getNumChildren() == 1){
				grp = dynamic_cast<osg::Group*>(grp->getChild(0));
			}
			if (grp) {
				for (int i = 0; i<grp->getNumChildren(); i++)
				{
					m_Components->addChild(grp->getChild(i));
				}
			}
			m_Bimgrw = new Bimgrow(m_Components);
			grp->setUpdateCallback(m_Bimgrw);
		}
	}
	m_Bimgrw->start();
}

void osgEarthXCore::ModelProvider::setPlay(bool bPlay)
{
	m_bPlay = bPlay;
}

bool osgEarthXCore::ModelProvider::getPlay()
{
	return m_bPlay;
}

void osgEarthXCore::ModelProvider::setSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
}

double osgEarthXCore::ModelProvider::getSpeed()
{
	return m_dSpeed;
}

void osgEarthXCore::ModelProvider::setPath(osgEarth::Symbology::LineString * lin)
{
	m_pathLine = lin;
}

osgEarth::Symbology::LineString * osgEarthXCore::ModelProvider::getPath()
{
	return m_pathLine.get();
}
