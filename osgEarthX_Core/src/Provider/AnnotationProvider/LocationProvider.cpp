#include <Provider/AnnotationProvider/LocationProvider.h>
#include <strstream>
using namespace osgEarthXCore;

#include <ConvertChar/ConvertChar.h>
#include <osgDB/ReadFile>
#include <osgDB/DatabasePager>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/XmlParser>
#include <OSGEarth/NodeUtils>

#include <osgFX/Cartoon>
#include <osgFX/Outline>
#include <osgFX/Scribe>

#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>

//#include <ifcpp/geometry/Carve/GeometryConverter.h>
//#include <ifcpp/geometry/Carve/ConverterOSG.h>
//#include <ifcpp/model/BuildingModel.h>
//#include <ifcpp/model/BuildingException.h>
//#include <ifcpp/model/BuildingGuid.h>
//#include <ifcpp/reader/ReaderSTEP.h>
//#include <ifcpp/reader/ReaderUtil.h>
//#include <ifcpp/writer/WriterSTEP.h>
//#include <ifcpp/IFC4/include/IfcProduct.h>
//#include <ifcpp/IFC4/include/IfcSite.h>
//#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
//#include <ifcpp/IFC4/include/IfcOpeningElement.h>
//#include <ifcpp/IFC4/include/IfcOwnerHistory.h>
//#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
//
//#include <osgUtil\Optimizer>

#include "download.h"

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

#include <Windows.h>
void ConvertUtf8ToGBK(std::string &strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len];
	memset(wszGBK, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}

LocationProvider::LocationProvider()
{
}

LocationProvider::~LocationProvider()
{

}

void LocationProvider::OpenModelFile(const std::string & url)
{
	std::string urlSource = url;
	std::string urlLocal = url;
	if (osgDB::containsServerAddress(url)) {
		char lpTempPathBuffer[MAX_PATH];

		CDownloader murl;
		DLIO mDlWork;

		DWORD dwRetVal = GetTempPathA(MAX_PATH,          // length of the buffer
			lpTempPathBuffer); // buffer for path 
		if (dwRetVal > MAX_PATH || (dwRetVal == 0))
		{
			return;
		}

		char filename[256];

		murl.GetFileNameFormUrl(filename, url.c_str());
		std::string nm = osgDB::getNameLessExtension(filename);
		urlLocal = lpTempPathBuffer;
		urlLocal += filename;

		if (!osgDB::fileExists(urlLocal)) {
			strcpy(mDlWork.url, urlSource.c_str());
			strcpy(mDlWork.filePath, lpTempPathBuffer);
			murl.AddDownloadWork(mDlWork);      //添加到下载任务中  
	
			murl.StartDownloadThread();         //开启下载线程  
			while (1)
			{
				if (murl.IsDownloadBegin())
				{
					Sleep(50);
				}
				if (murl.IsDownloadEnd()) break;
			}
		}	
	}

	std::string ext = osgDB::getFileExtension(urlLocal);
	std::ifstream fin(urlLocal.c_str());
	if (ext == "xml")
	{
		osgDB::XmlNode::Input input;
		input.attach(fin);
input.readAllDataIntoBuffer();

osg::ref_ptr<osgDB::XmlNode> doc = new osgDB::XmlNode;

doc->read(input);

for (osgDB::XmlNode::Children::iterator itr = doc->children.begin();
	itr != doc->children.end();
	++itr)
{
	if (itr->get()->name == "GeoDocument")
	{
		for (osgDB::XmlNode::Children::iterator itr1 = itr->get()->children.begin();
			itr1 != itr->get()->children.end();
			++itr1)
		{
			if (itr1->get()->name == "GeoModel")
			{
				double lat, lon, alt;
				double rotX, rotY, rotZ, range;
				std::string modelUrl;
				std::string modelName;
				std::string isSel;
				for (osgDB::XmlNode::Children::iterator itr2 = itr1->get()->children.begin();
					itr2 != itr1->get()->children.end();
					++itr2)
				{
					if (itr2->get()->name == "Url")
					{
						modelUrl = itr2->get()->contents.c_str();;
					}

					if (itr2->get()->name == "name")
					{
						modelName = itr2->get()->contents.c_str();
						ConvertUtf8ToGBK(modelName);
					}

					if (itr2->get()->name == "Range")
					{
						sscanf(itr2->get()->contents.c_str(), "%lf", &range);
					}
				}
				_nameMap.insert(make_pair(modelUrl, modelName));
				_rangeMap.insert(make_pair(modelUrl, range));
			}//GeoModel
		}
	}
}

	}
	//fin.close();
}


struct PagedNodeOperation : public osgEarth::NodeOperation
{
	virtual void operator()(osg::Node* node)
	{
		std::string strName = node->getName();
	}
};



void LocationProvider::setModelPath(const std::string& str)
{
	setDataVariance(osg::Object::STATIC);
	OpenModelFile(str);
	osg::ref_ptr<osgDB::Options> options = new osgDB::Options;
	options->setObjectCacheHint(osgDB::Options::CACHE_NODES);

	// osg::ref_ptr<osgEarth::RefNodeOperationVector> nov = new osgEarth::RefNodeOperationVector;
	// nov->push_back(new PagedNodeOperation());

	for (std::map<std::string, std::string>::iterator iter = _nameMap.begin(); iter != _nameMap.end(); ++iter)
	{
		std::string fileName = iter->first;
		double range = _rangeMap[fileName];
		osg::ref_ptr<osg::PagedLOD> mdl = new osg::PagedLOD;
		//osg::ref_ptr<osgEarth::PagedLODWithNodeOperations> mdl = new osgEarth::PagedLODWithNodeOperations(nov.get());
		mdl->setDatabaseOptions(options.get());
		mdl->setFileName(0, fileName);
		mdl->setName(iter->second);
		mdl->setRange(0, 0, range);
		if (_mt.valid()) {
			_mt->addChild(mdl);
		}
		else {
			addChild(mdl);
		}
	}
}

void LocationProvider::setModelXML(const std::string& str)
{
	setDataVariance(osg::Object::STATIC);
	OpenModelXML(str);
	osg::ref_ptr<osgDB::Options> options = new osgDB::Options;
	options->setObjectCacheHint(osgDB::Options::CACHE_NODES);

	// osg::ref_ptr<osgEarth::RefNodeOperationVector> nov = new osgEarth::RefNodeOperationVector;
	// nov->push_back(new PagedNodeOperation());

	for (std::map<std::string, std::string>::iterator iter = _nameMap.begin(); iter != _nameMap.end(); ++iter)
	{
		std::string fileName = iter->first;
		double range = _rangeMap[fileName];
		osg::ref_ptr<osg::PagedLOD> mdl = new osg::PagedLOD;
		//osg::ref_ptr<osgEarth::PagedLODWithNodeOperations> mdl = new osgEarth::PagedLODWithNodeOperations(nov.get());
		mdl->setDatabaseOptions(options.get());
		mdl->setFileName(0, fileName);
		mdl->setName(iter->second);
		mdl->setRange(0, 0, range);
		if (_mt.valid()) {
			_mt->addChild(mdl);
		}
		else {
			addChild(mdl);
		}
	}
}

void osgEarthXCore::LocationProvider::makeRotate(double ax, double ay, double az)
{
	if (!_mt.valid()) {
		_mt = new osg::MatrixTransform();
		addChild(_mt);
	}
	_mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(ax), osg::Vec3d(1.0f, 0.0f, 0.0f), osg::inDegrees(ay), osg::Vec3d(0.0f, 1.0f, 0.0f), osg::inDegrees(az), osg::Vec3d(0.0f, 0.0f, 1.0f)));
}

class FindNodeVisitor : public osg::NodeVisitor
{
public:

	FindNodeVisitor(std::string strID) :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _strID(strID), _objFind(nullptr)
	{
	}

	virtual void apply(osg::Group& nd)
	{
		if (nd.getName() == _strID) {
			_objFind = &nd;
		}
		else
			traverse(nd);
	}

	virtual void apply(osg::Node& nd)
	{
		traverse(nd);
	}
	
	osg::Node* getFindResult() {
		return _objFind;
	}
protected:
	std::string _strID;
	osg::Node*  _objFind;
};

class FindNodesVisitor : public osg::NodeVisitor
{
public:

	FindNodesVisitor(std::vector<const std::string> &names) :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _names(names)
	{
	}

	virtual void apply(osg::Group& nd)
	{
		std::vector<const std::string>::iterator iter = std::find(_names.begin(), _names.end(), nd.getName());
		if (iter != _names.end()){
			_names.erase(iter);
			_objsFind.push_back(&nd);
		}
		if (_names.size() != 0)
			traverse(nd);
	}

	virtual void apply(osg::Node& nd)
	{
		traverse(nd);
	}

	std::vector<osg::Node*> getFindResult() {
		return _objsFind;
	}
protected:
	std::vector<const std::string> &_names;
	std::vector<osg::Node*>  _objsFind;
};

osg::Node* osgEarthXCore::LocationProvider::findAnnotation(std::string strID)
{
	    FindNodeVisitor fplv(strID);
		accept(fplv);
		return fplv.getFindResult();
}

class ShowNodeVisitor : public osg::NodeVisitor
{
public:

	ShowNodeVisitor(bool bShow) :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _bShow(bShow)
	{
	}

	virtual void apply(osg::Geode& nd)
	{
		int iNum = nd.getNumDrawables();
		for (int i = 0; i < iNum; i++)
		{
			osg::Geometry* nGeo = dynamic_cast<osg::Geometry*>(nd.getDrawable(i));
			if (nGeo)
			{
				if (_bShow) {
					nGeo->setNodeMask(1);
				}
				else {
					nGeo->setNodeMask(0);
				}
			}
		}
		traverse(nd);
	}

	virtual void apply(osg::Node& nd)
	{
		traverse(nd);
	}
protected:
	bool _bShow;
};

class ShowNodesVisitor : public osg::NodeVisitor
{
public:

	ShowNodesVisitor(std::vector<const std::string> &names,bool bShow) :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _names(names),_bShow(bShow)
	{
	}

	virtual void apply(osg::Geode& nd)
	{
		if (std::find(_names.begin(), _names.end(), nd.getName()) != _names.end()){
			int iNum = nd.getNumDrawables();
			for (int i = 0; i < iNum; i++)
			{
				osg::Geometry* nGeo = dynamic_cast<osg::Geometry*>(nd.getDrawable(i));
				if (nGeo)
				{
					if (_bShow) {
						nGeo->setNodeMask(1);
					}
					else {
						nGeo->setNodeMask(0);
					}
				}
			}
		}
		traverse(nd);
	}

	virtual void apply(osg::Node& nd)
	{
		traverse(nd);
	}
protected:
	bool _bShow;
	std::vector<const std::string> &_names;
};

void osgEarthXCore::LocationProvider::showNode(const std::string & name, bool visible)
{
	osg::Node* nd = findAnnotation(name);
	if (nd){
		ShowNodeVisitor sv(visible);
		nd->accept(sv);
	}
	/*if (nd) {
		if (visible) {
			nd->setNodeMask(1);
		}
		else {
			nd->setNodeMask(0);
		}
	}*/
}

void osgEarthXCore::LocationProvider::showNode(bool visible)
{
	ShowNodeVisitor sv(visible);
	accept(sv);
}

class SetColorOperation : public osg::Operation
{
public:
	SetColorOperation(osg::Node* nd, osg::Vec4f col) : _nd(nd), _col(col) {}

	virtual void operator()(osg::Object*)
	{
		osg::ref_ptr<osg::Material> material_selected = new osg::Material();
		material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, _col);
		material_selected->setSpecular(osg::Material::FRONT_AND_BACK, _col);
		material_selected->setShininess(osg::Material::FRONT_AND_BACK, 35.0);
		material_selected->setColorMode(osg::Material::SPECULAR);

		osg::ref_ptr<osg::StateSet> stateset = _nd->getOrCreateStateSet();
		osg::Material* material_previous = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
		if (material_previous)
		{
			if (!_nd->getUserData())
			{
				_nd->setUserData(material_previous);
			}
		}
		stateset->setAttribute(material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		stateset->setRenderBinDetails(11, "DepthSortedBin");
	}

protected:
	virtual ~SetColorOperation() {}

protected:
	osg::ref_ptr<osg::Node> _nd;
	osg::Vec4f _col;

private:
};

class SetColorsOperation : public osg::Operation
{
public:
	SetColorsOperation(std::vector<osg::Node*> nds, osg::Vec4f col) : _nds(nds), _col(col) {}

	virtual void operator()(osg::Object*)
	{
		osg::ref_ptr<osg::Material> material_selected = new osg::Material();
		material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, _col);
		material_selected->setSpecular(osg::Material::FRONT_AND_BACK, _col);
		material_selected->setShininess(osg::Material::FRONT_AND_BACK, 35.0);
		material_selected->setColorMode(osg::Material::SPECULAR);
		for (int i = 0; i < _nds.size(); i++){
			osg::ref_ptr<osg::StateSet> stateset = _nds[i]->getOrCreateStateSet();
			osg::Material* material_previous = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
			if (material_previous)
			{
				if (!_nds[i]->getUserData())
				{
					_nds[i]->setUserData(material_previous);
				}
			}
			stateset->setAttribute(material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

			stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			stateset->setRenderBinDetails(11, "DepthSortedBin");
		}
	}

protected:
	virtual ~SetColorsOperation() {}

	std::vector<osg::Node*> _nds;
	osg::Vec4f _col;
};

void osgEarthXCore::LocationProvider::setColor(osg::Node* nd, osg::Vec4f& col)
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new SetColorOperation(nd, col));
	//osg::ref_ptr<osg::Material> material_selected = new osg::Material();
	//material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, col);
	//material_selected->setSpecular(osg::Material::FRONT_AND_BACK, col);
	//material_selected->setShininess(osg::Material::FRONT_AND_BACK, 35.0);
	//material_selected->setColorMode(osg::Material::SPECULAR);

	//osg::ref_ptr<osg::StateSet> stateset = nd->getOrCreateStateSet();
	//osg::Material* material_previous = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
	//if (material_previous)
	//{
	//	if (!nd->getUserData())
	//	{
	//		nd->setUserData(material_previous);
	//	}
	//}
	//stateset->setAttribute(material_selected, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	//stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//stateset->setRenderBinDetails(11, "DepthSortedBin");
}

class RemoveColorOperation : public osg::Operation
{
public:
	RemoveColorOperation(osg::Node* nd) : _nd(nd) {}

	virtual void operator()(osg::Object*)
	{
		osg::Material* material_previous = dynamic_cast<osg::Material*>(_nd->getUserData());
		if (material_previous) {
			osg::ref_ptr<osg::StateSet> stateset = _nd->getOrCreateStateSet();
			stateset->setAttribute(material_previous, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
		else {
			osg::ref_ptr<osg::StateSet> stateset = _nd->getOrCreateStateSet();
			osg::Material* material_now = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
			stateset->removeAttribute(material_now);
		}
	}

protected:
	virtual ~RemoveColorOperation() {}

	osg::ref_ptr<osg::Node> _nd;
};

class RemoveColorsOperation : public osg::Operation
{
public:
	RemoveColorsOperation(std::vector<osg::Node*> nds) : _nds(nds) {}

	virtual void operator()(osg::Object*)
	{
		for (int i = 0; i < _nds.size(); i++){
			osg::Material* material_previous = dynamic_cast<osg::Material*>(_nds[i]->getUserData());
			if (material_previous) {
				osg::ref_ptr<osg::StateSet> stateset = _nds[i]->getOrCreateStateSet();
				stateset->setAttribute(material_previous, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			}
			else {
				osg::ref_ptr<osg::StateSet> stateset = _nds[i]->getOrCreateStateSet();
				osg::Material* material_now = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
				stateset->removeAttribute(material_now);
			}
		}
	}

protected:
	virtual ~RemoveColorsOperation() {}
	std::vector<osg::Node*> _nds;
};

void osgEarthXCore::LocationProvider::removeColor(osg::Node* nd)
{
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new RemoveColorOperation(nd));
	/*osg::Material* material_previous = dynamic_cast<osg::Material*>(nd->getUserData());
	if (material_previous) {
		osg::ref_ptr<osg::StateSet> stateset = nd->getOrCreateStateSet();
		stateset->setAttribute(material_previous, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
	else {
		osg::ref_ptr<osg::StateSet> stateset = nd->getOrCreateStateSet();
		osg::Material* material_now = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
		stateset->removeAttribute(material_now);
	}*/
}

void LocationProvider::OpenModelXML(const std::string& strXml)
{
	    std::istrstream strin(strXml.c_str());
		osgDB::XmlNode::Input input;
		input.attach(strin);
		input.readAllDataIntoBuffer();

		osg::ref_ptr<osgDB::XmlNode> doc = new osgDB::XmlNode;

		doc->read(input);

		for (osgDB::XmlNode::Children::iterator itr = doc->children.begin();
			itr != doc->children.end();
			++itr)
		{
			if (itr->get()->name == "GeoDocument")
			{
				for (osgDB::XmlNode::Children::iterator itr1 = itr->get()->children.begin();
					itr1 != itr->get()->children.end();
					++itr1)
				{
					if (itr1->get()->name == "GeoModel")
					{
						double lat, lon, alt;
						double rotX, rotY, rotZ, range;
						std::string modelUrl;
						std::string modelName;
						std::string isSel;
						for (osgDB::XmlNode::Children::iterator itr2 = itr1->get()->children.begin();
							itr2 != itr1->get()->children.end();
							++itr2)
						{
							if (itr2->get()->name == "Url")
							{
								modelUrl = itr2->get()->contents.c_str();;
							}

							if (itr2->get()->name == "name")
							{
								modelName = itr2->get()->contents.c_str();
								ConvertUtf8ToGBK(modelName);
							}

							if (itr2->get()->name == "Range")
							{
								sscanf(itr2->get()->contents.c_str(), "%lf", &range);
							}
						}
						_nameMap.insert(make_pair(modelUrl, modelName));
						_rangeMap.insert(make_pair(modelUrl, range));
					}//GeoModel
				}
			}
		}
}

void osgEarthXCore::LocationProvider::setColor(osg::Vec4f col)
{
	setColor(this, col);
}

void osgEarthXCore::LocationProvider::removeColor()
{
	removeColor(this);
}

void osgEarthXCore::LocationProvider::setColor(const std::string& name, osg::Vec4 col)
{
	osg::Node* nd = findAnnotation(name);
	if (nd)
		setColor(nd, col);
}

void osgEarthXCore::LocationProvider::removeColor(const std::string& name)
{
	osg::Node* nd = findAnnotation(name);
	if (nd)
		removeColor(nd);
}

void LocationProvider::setColor(std::vector<const std::string>& name, osg::Vec4f col)
{
	FindNodesVisitor fplv(name);
	accept(fplv);
	std::vector<osg::Node*> nds = fplv.getFindResult();
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new SetColorsOperation(nds,col));
}

void LocationProvider::removeColor(std::vector<const std::string>& name)
{
	FindNodesVisitor fplv(name);
	accept(fplv);
	std::vector<osg::Node*> nds = fplv.getFindResult();
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new RemoveColorsOperation(nds));
}

void LocationProvider::getCenter(const std::string& name,double &lat,double &lon ,double &height)
{
	osg::Node* nd = findAnnotation(name);
	if (nd)
	{
		osg::Vec3 v = nd->getBound().center();
		osg::Vec3 vv;
		if (_mt.valid()) {
			vv = v * _mt->getMatrix() * this->getMatrix();
		} else
		{
			vv = v * this->getMatrix();
		}
		osg::ref_ptr<osg::EllipsoidModel> em = new osg::EllipsoidModel();
		double dlat, dlon;
		em->convertXYZToLatLongHeight(vv.x(), vv.y(), vv.z(), dlat, dlon, height);
		lat = osg::RadiansToDegrees(dlat);
		lon = osg::RadiansToDegrees(dlon);
	}
}

void LocationProvider::showChilds(std::vector<const std::string> &name, bool vis)
{
	ShowNodesVisitor sv(name,vis);
	accept(sv);
}

class AddEffectOperation : public osg::Operation
{
public:
	AddEffectOperation(osg::Group* par, osgFX::Effect* eff) : _par(par), _eff(eff) {}

	virtual void operator()(osg::Object*)
	{
		osgEarth::insertGroup(_eff,_par);
	}

protected:
	virtual ~AddEffectOperation() {}

protected:
	osg::ref_ptr<osg::Group> _par;
	osg::ref_ptr<osgFX::Effect> _eff;

private:
};

class ReplaceEffectOperation : public osg::Operation
{
public:
	ReplaceEffectOperation(osgFX::Effect* o, osgFX::Effect* n) : _old(o), _new(n) {}

	virtual void operator()(osg::Object*)
	{
		osgEarth::replaceGroup(_old, _new);
	}

protected:
	virtual ~ReplaceEffectOperation() {}

protected:
	osg::ref_ptr<osgFX::Effect> _old;
	osg::ref_ptr<osgFX::Effect> _new;

private:
};


void osgEarthXCore::LocationProvider::setEffect(unsigned int uiEffect)
{
	osg::Group* grp = _mt.valid() ? _mt : this;
	osg::ref_ptr<osgFX::Effect> eff;
	switch (uiEffect) {
	case 1:
	{
		eff = new osgFX::Scribe();
		osgFX::Scribe* ol = (osgFX::Scribe*)eff.get();
		ol->setWireframeColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	}
		break;
	case 2:
		eff = new osgFX::Cartoon();
		break;
	case 3:
	{
		eff = new osgFX::Outline();
		osgFX::Outline* ol = (osgFX::Outline*)eff.get();
		ol->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	}
		break;
	default:
		break;
	}

	if (!eff.valid()) return;
	if (_effect) {
		osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new ReplaceEffectOperation(_effect, eff.get()));
	}
	else {
		osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new AddEffectOperation(grp, eff.get()));
	}
	_effect = eff;
}

