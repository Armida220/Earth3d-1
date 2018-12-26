#include <Annotation/Model.h>
#include <osgDB/ReadFile>
#include <osgDB/DatabasePager>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/XmlParser>
#include <osgEarth/ElevationQuery>
#include <osgEarthAnnotation/HighlightDecoration>

using namespace osgEarthX;

Model::Model()
{
	m_ObjectType = MODEL;
}

Model::~Model()
{

}

bool Model::isEnabled()
{
	return true;
}

bool Model::isVisible()
{
	return getNodeMask();
}

void Model::setVisible( bool bVisible )
{
	if (bVisible) {
		setNodeMask(1);
	}
	else {
		setNodeMask(0);
	}
}

ModelSet::ModelSet()
{
	m_ObjectType = MODELSET;
	_group = new osg::Group();
	_group->setCullingActive(false);
}

ModelSet::~ModelSet()
{

}

bool ModelSet::isEnabled()
{
	return true;
}

bool ModelSet::isVisible()
{
	return true;
}

void ModelSet::setVisible(bool bVisible)
{

}

void osgEarthX::ModelSet::initMapNode(osgEarth::MapNode * pMapNode)
{
	_mapNode = pMapNode;
}

void osgEarthX::ModelSet::setModelPath(const std::string & modelPath)
{
	_strModelFile = modelPath;
}

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();
	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
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

void osgEarthX::ModelSet::OpenModelFile(const std::string & url)
{
	std::string ext = osgDB::getFileExtension(url);
	std::ifstream fin(url.c_str());
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
						double rotX, rotY, rotZ;
						std::string modelUrl;
						std::string modelName;
						std::string isSel;
						for (osgDB::XmlNode::Children::iterator itr2 = itr1->get()->children.begin();
							itr2 != itr1->get()->children.end();
							++itr2)
						{
							if (itr2->get()->name == "Url")
							{
								modelUrl = osgDB::getSimpleFileName(itr2->get()->contents);
							}

							if (itr2->get()->name == "name")
							{
								modelName = itr2->get()->contents.c_str();
								ConvertUtf8ToGBK(modelName);
							}

							if (itr2->get()->name == "IsShowTip")
							{
								isSel = itr2->get()->contents.c_str();

							}

							if (itr2->get()->name == "RotX")
							{
								sscanf(itr2->get()->contents.c_str(), "%lf", &rotX);
							}
							if (itr2->get()->name == "RotY")
							{
								sscanf(itr2->get()->contents.c_str(), "%lf", &rotY);
							}
							if (itr2->get()->name == "RotZ")
							{
								sscanf(itr2->get()->contents.c_str(), "%lf", &rotZ);
							}

							if (itr2->get()->name == "SphereCoord")
							{
								for (osgDB::XmlNode::Children::iterator itr3 = itr2->get()->children.begin();
									itr3 != itr2->get()->children.end();
									++itr3)
								{
									if (itr3->get()->name == "Latitude")
									{
										for (osgDB::XmlNode::Children::iterator itr4 = itr3->get()->children.begin();
											itr4 != itr3->get()->children.end();
											++itr4)
										{
											if (itr4->get()->name == "Degrees")
											{
												sscanf(itr4->get()->contents.c_str(), "%lf", &lat);
											}
										}
									}

									if (itr3->get()->name == "Longitude")
									{
										for (osgDB::XmlNode::Children::iterator itr4 = itr3->get()->children.begin();
											itr4 != itr3->get()->children.end();
											++itr4)
										{
											if (itr4->get()->name == "Degrees")
											{
												sscanf(itr4->get()->contents.c_str(), "%lf", &lon);
											}
										}
									}

									if (itr3->get()->name == "Altitude")
									{
										sscanf(itr3->get()->contents.c_str(), "%lf", &alt);
									}


								}
							}//SphereCoord



						}
						_modelMap.insert(make_pair(modelUrl, osg::Vec3d(lon, lat, alt)));
						_rotMap.insert(make_pair(modelUrl, osg::Vec3d(rotX, rotY, osg::PI + rotZ)));
						_nameMap.insert(make_pair(modelUrl, modelName));
						_isSelected.insert(make_pair(modelUrl, isSel));
					}//GeoModel
				}
			}
		}

	}
	else if (ext == "csv")
	{
		std::string s;
		while (std::getline(fin, s))
		{
			std::vector<std::string> subStrs = split(s, ",");
			if (subStrs.size() < 11)
				continue;
			std::string fileName = subStrs[0];

			double lon;
			sscanf(subStrs[1].c_str(), "%lf", &lon);
			double lat;
			sscanf(subStrs[2].c_str(), "%lf", &lat);
			double alt;
			sscanf(subStrs[3].c_str(), "%lf", &alt);

			osg::Vec3d vPos(lon, lat, alt);
			_modelMap.insert(make_pair(fileName, vPos));

			double rotX;
			sscanf(subStrs[4].c_str(), "%lf", &rotX);
			double rotY;
			sscanf(subStrs[5].c_str(), "%lf", &rotY);
			double rotZ;
			sscanf(subStrs[6].c_str(), "%lf", &rotZ);

			osg::Vec3d vRot(rotX, rotY, rotZ);
			_rotMap.insert(make_pair(fileName, vRot));
		}

	}
	fin.close();
}

//osg::Node* ModelSet::batchLoadingModels()
//{
//	std::string fileRoot = osgDB::getFilePath(_strModelFile);
//	std::string gmdFolderRoot = osgDB::getFilePath(fileRoot);
//
//
//	OpenModelFile(_strModelFile);
//
//
//	const osgEarth::SpatialReference* latLong = _mapNode->getMapSRS()->getGeographicSRS();
//	osg::ref_ptr<osg::Group> group = new osg::Group();
//	for (std::map<std::string, osg::Vec3d>::iterator veitr = _modelMap.begin(); veitr != _modelMap.end(); ++veitr)
//	{
//		std::string fileName = veitr->first;
//		osg::Vec3d pos = veitr->second;
//		std::string fileNameOsgb = fileName.substr(0, fileName.find_last_of('.'));
//		fileNameOsgb += ".osgb";
//		if ((osgDB::findDataFile(fileRoot + "\\LOD1\\OSGB\\" + fileNameOsgb)).empty())
//			continue;
//		osg::ref_ptr<osg::LOD> plod = new osg::LOD;
//		group->addChild(plod.get());
//
//		std::string strName = _nameMap[fileName];
//		osg::Vec3 rot = _rotMap[fileName];
//
//		if ((osgDB::findDataFile(fileRoot + "\\LOD0\\OSGB\\" + fileNameOsgb)).empty())
//			continue;
//		//osg::Node* myModelDetail = osgDB::readNodeFile(fileRoot + "\\GMD_1_1\\OSGBVBO\\" + fileNameOsgb);
//		//osgEarth::Annotation::Style styleDetail;
//		//styleDetail.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(myModelDetail);
//		//osgEarth::Annotation::ModelNode* modelDetail = new osgEarth::Annotation::ModelNode(_mapNode.get(), styleDetail);
//		Model *modelDetail = new Model();
//		modelDetail->setModelPath(fileRoot + "\\LOD0\\OSGB\\" + fileNameOsgb);
//		modelDetail->setPosition(osgEarth::GeoPoint(latLong, pos.x(), pos.y(),2, osgEarth::ALTMODE_RELATIVE));
//		osg::Quat modelRot(rot.x(), osg::Vec3d(1, 0, 0), rot.y(), osg::Vec3d(0, 1, 0), rot.z(), osg::Vec3d(0, 0, 1));
//		modelDetail->setLocalRotation(modelRot);
//		modelDetail->initMapNode(_mapNode.get());
//		modelDetail->installDecoration("hover", new osgEarth::Annotation::HighlightDecoration());
//		modelDetail->setName(strName);
//		plod->addChild(dynamic_cast<osg::Node*>(modelDetail), 0,1000);
//
//		Model *model = new Model();
//		model->setModelPath(fileRoot + "\\LOD1\\OSGB\\" + fileNameOsgb);
//		model->setPosition(osgEarth::GeoPoint(latLong, pos.x(), pos.y(),2, osgEarth::ALTMODE_RELATIVE));
//		model->setLocalRotation(modelRot);
//		model->initMapNode(_mapNode.get());
//		model->installDecoration("hover", new osgEarth::Annotation::HighlightDecoration());
//		model->setName(strName);
//		plod->addChild(dynamic_cast<osg::Node*>(model), 1000, 200000);
//	}
//	group->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	return group.release();
//}

osg::Node* ModelSet::batchLoadingModels()
{
	std::string fileRoot = osgDB::getFilePath(_strModelFile);
	std::string gmdFolderRoot = osgDB::getFilePath(fileRoot);


	OpenModelFile(_strModelFile);


	const osgEarth::SpatialReference* latLong = _mapNode->getMapSRS()->getGeographicSRS();
	//osg::ref_ptr<osg::Group> group = new osg::Group();
	
	int delay = 0;
	char* strDelay = getenv("OSG_PAGEDLOD_DELAY");
	if (strDelay != 0)
	{
		delay = atoi(strDelay);
		OSG_NOTICE << "Pagelod delay time = " << delay << std::endl;
	}

	int length = 5000;
	char* strLength = getenv("OSG_PAGEDLOD_LENGTH");
	if (strLength != 0)
	{
		length = atoi(strLength);
	}

	for (std::map<std::string, osg::Vec3d>::iterator veitr = _modelMap.begin(); veitr != _modelMap.end(); ++veitr)
	{
		std::string fileName = veitr->first;
		osg::Vec3d pos = veitr->second;
		std::string fileNameOsgb = fileName.substr(0, fileName.find_last_of('.'));
		fileNameOsgb += ".osgb";
		if ((osgDB::findDataFile(fileRoot + "\\LOD1\\OSGB\\" + fileNameOsgb)).empty())
			continue;
		osg::ref_ptr<osg::PagedLOD> plod = new osg::PagedLOD;
		plod->setFileName(0, fileRoot + "\\LOD1\\OSGB\\" + fileNameOsgb);
		plod->setRange(0, length, 200000);
		plod->setPriorityOffset(0, 0);
		plod->setPriorityScale(0, 1.0);
		plod->setFileName(1, fileRoot + "\\LOD0\\OSGB\\" + fileNameOsgb);
		plod->setRange(1, 0, length);
		plod->setPriorityOffset(1, 1.0);
		plod->setPriorityScale(1, 1.0);

		if(delay != 0)
			plod->setMinimumExpiryTime(1, delay);
		osg::Vec3 rot = _rotMap[fileName];
		std::string strName = _nameMap[fileName];
		
		bool bSel = _isSelected[fileName] == "true" ? true : false;

		Model *modelDetail = new Model();
		if (bSel) {
			modelDetail->addEventCallback(m_refCB);
		}
		modelDetail->setModel(plod);
		modelDetail->initMapNode(_mapNode.get());
		
		modelDetail->setPosition(osgEarth::GeoPoint(latLong, pos.x(), pos.y(), pos.z()+2, osgEarth::ALTMODE_ABSOLUTE));
		osg::Quat modelRot(rot.x(), osg::Vec3d(1, 0, 0), rot.y(), osg::Vec3d(0, 1, 0), rot.z(), osg::Vec3d(0, 0, 1));
		modelDetail->setLocalRotation(modelRot);
		//modelDetail->installDecoration("hover", new osgEarth::Annotation::HighlightDecoration());
		modelDetail->setName(strName.c_str());
		
		_group->addChild(modelDetail);
	}
	//group->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return _group.get();
}

osg::Group * osgEarthX::ModelSet::getNodes()
{
	return _group.get();
}

void ModelSet::setAnnnotationEventCallback(osgEarth::Util::AnnotationEventCallback* cb)
{
	m_refCB = cb;
}

//osg::Node* ModelSet::batchLoadingModels()
//{
//	std::string fileRoot = osgDB::getFilePath(_strModelFile);
//	std::string gmdFolderRoot = osgDB::getFilePath(fileRoot);
//
//
//	OpenModelFile(_strModelFile);
//
//
//	const osgEarth::SpatialReference* latLong = _mapNode->getMapSRS()->getGeographicSRS();
//	osg::ref_ptr<osg::Group> group = new osg::Group();
//	for (std::map<std::string, osg::Vec3d>::iterator veitr = _modelMap.begin(); veitr != _modelMap.end(); ++veitr)
//	{
//		std::string fileName = veitr->first;
//		osg::Vec3d pos = veitr->second;
//		std::string fileNameOsgb = fileName.substr(0, fileName.find_last_of('.'));
//		fileNameOsgb += ".osgb";
//		if ((osgDB::findDataFile(fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb)).empty())
//			continue;
//		osg::ref_ptr<osg::PagedLOD> plod = new osg::PagedLOD;
//		plod->setFileName(0, fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb);
//		plod->setRange(0, 1000, 200000);
//		plod->setFileName(1, fileRoot + "\\GMD_1_1\\OSGBVBO\\" + fileNameOsgb);
//		plod->setRange(1, 0, 1000);
//
//		//For Debug
//		/*
//		OSG_WARN<< "===============Read Model Start : " << fileNameOsgb << "============================" << std::endl;
//		osg::Timer_t readStartTime = osg::Timer::instance()->tick();
//		osg::ref_ptr<osg::Node> plod2 = osgDB::readNodeFile(fileRoot + "\\GMD_1_1\\OSGBVBO\\" + fileNameOsgb);
//		osg::Timer_t readEndTime = osg::Timer::instance()->tick();
//		OSG_WARN<< "===============Read Model Time : " << fileNameOsgb << " : " <<  readEndTime - readStartTime << std::endl;
//		*/
//
//		osgEarth::GeoPoint point(latLong, pos.x(), pos.y(), 0.0, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
//		osgEarth::ElevationQuery eleQuery(_mapNode->getMap());
//		double ele;
//		eleQuery.getElevation(point, ele);
//		point.alt() = ele + 3.0;
//
//		osg::Vec3 rot = _rotMap[fileName];
//		osg::Matrix matRot = osg::Matrix::rotate(rot.x(), osg::Vec3d(1, 0, 0), rot.y(), osg::Vec3d(0, 1, 0), rot.z(), osg::Vec3d(0, 0, 1));
//
//		osg::Matrix matrix;
//		point.createLocalToWorld(matrix);
//		osg::ref_ptr<osg::MatrixTransform> myMatrixTransform = new osg::MatrixTransform;
//		myMatrixTransform->setMatrix(matRot * matrix);
//		myMatrixTransform->addChild(plod.get());
//		group->addChild(myMatrixTransform.get());
//
//
//		//osg::Matrix marRot;
//		//osg::ref_ptr<osg::MatrixTransform> matTrans = new osg::MatrixTransform;
//		////matTrans->setMatrix(matRot);
//		//matTrans->setMatrix(osg::Matrix::identity());
//		//matTrans->addChild(plod.get());
//
//		//osg::Node* myModel = osgDB::readNodeFile(fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb);
//
//		//osgEarth::Drivers::SimpleModelOptions smOpt;
//		//smOpt.url = fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb;
//		//smOpt.paged = true;
//		//smOpt.minRange = 0;
//		//smOpt.maxRange = 200000;
//
//		//osgEarth::Annotation::Style style;
//		//style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(myModel);
//		//osgEarth::Annotation::ModelNode* model = new osgEarth::Annotation::ModelNode(_mapNode.get(), style);
//		//osg::Vec3d outPt(pos.x(), pos.y(),10.0);
//		//model->setPosition(osgEarth::GeoPoint(latLong, outPt, osgEarth::ALTMODE_ABSOLUTE));
//		//group->addChild(model);
//	}
//	group->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	return group.release();
//}

//void osgEarthX::ModelSet::OpenModelFile(const std::string & url)
//{
//	std::string ext = osgDB::getFileExtension(url);
//	if (ext == "xml")
//	{
//		std::ifstream fin(url.c_str());
//
//		osgDB::XmlNode::Input input;
//		input.attach(fin);
//		input.readAllDataIntoBuffer();
//
//		osg::ref_ptr<osgDB::XmlNode> doc = new osgDB::XmlNode;
//		doc->read(input);
//
//		for (osgDB::XmlNode::Children::iterator itr = doc->children.begin();
//			itr != doc->children.end();
//			++itr)
//		{
//			if (itr->get()->name == "GeoDocument")
//			{
//				for (osgDB::XmlNode::Children::iterator itr1 = itr->get()->children.begin();
//					itr1 != itr->get()->children.end();
//					++itr1)
//				{
//					if (itr1->get()->name == "GeoModel")
//					{
//						double lat, lon, alt;
//						double rotX, rotY, rotZ;
//						std::string modelName;
//						for (osgDB::XmlNode::Children::iterator itr2 = itr1->get()->children.begin();
//							itr2 != itr1->get()->children.end();
//							++itr2)
//						{
//							if (itr2->get()->name == "Url")
//							{
//								modelName = osgDB::getSimpleFileName(itr2->get()->contents);
//							}
//
//							if (itr2->get()->name == "RotX")
//							{
//								sscanf(itr2->get()->contents.c_str(), "%lf", &rotX);
//							}
//							if (itr2->get()->name == "RotY")
//							{
//								sscanf(itr2->get()->contents.c_str(), "%lf", &rotY);
//							}
//							if (itr2->get()->name == "RotZ")
//							{
//								sscanf(itr2->get()->contents.c_str(), "%lf", &rotZ);
//							}
//
//							if (itr2->get()->name == "SphereCoord")
//							{
//								for (osgDB::XmlNode::Children::iterator itr3 = itr2->get()->children.begin();
//									itr3 != itr2->get()->children.end();
//									++itr3)
//								{
//									if (itr3->get()->name == "Latitude")
//									{
//										for (osgDB::XmlNode::Children::iterator itr4 = itr3->get()->children.begin();
//											itr4 != itr3->get()->children.end();
//											++itr4)
//										{
//											if (itr4->get()->name == "Degrees")
//											{
//												sscanf(itr4->get()->contents.c_str(), "%lf", &lat);
//											}
//										}
//									}
//
//									if (itr3->get()->name == "Longitude")
//									{
//										for (osgDB::XmlNode::Children::iterator itr4 = itr3->get()->children.begin();
//											itr4 != itr3->get()->children.end();
//											++itr4)
//										{
//											if (itr4->get()->name == "Degrees")
//											{
//												sscanf(itr4->get()->contents.c_str(), "%lf", &lon);
//											}
//										}
//									}
//
//									if (itr3->get()->name == "Altitude")
//									{
//										sscanf(itr3->get()->contents.c_str(), "%lf", &alt);
//									}
//
//
//								}
//							}//SphereCoord
//
//
//
//						}
//						_modelMap.insert(make_pair(modelName, osg::Vec3d(lon, lat, alt)));
//						_rotMap.insert(make_pair(modelName, osg::Vec3d(rotX, rotY, rotZ)));
//					}//GeoModel
//				}
//			}
//		}
//
//	}
//}
//
//osg::Node* ModelSet::batchLoadingModels()
//{
//	std::string fileRoot = osgDB::getFilePath(_strModelFile);
//	std::string gmdFolderRoot = osgDB::getFilePath(fileRoot);
//	std::ifstream fin(_strModelFile);
//
//	if (!fin.is_open())
//	{
//		std::cout << "Error opening file";
//	}
//	std::string s;
//
//	OpenModelFile(_strModelFile);
//	fin.close();
//
//	const osgEarth::SpatialReference* latLong = _mapNode->getMapSRS()->getGeographicSRS();
//	osg::ref_ptr<osg::Group> group = new osg::Group();
//	for (std::map<std::string, osg::Vec3d>::iterator veitr = _modelMap.begin(); veitr != _modelMap.end(); ++veitr)
//	{
//		std::string fileName = veitr->first;
//		osg::Vec3d pos = veitr->second;
//		std::string fileNameOsgb = fileName.substr(0, fileName.find_last_of('.'));
//		fileNameOsgb += ".osgb";
//		if ((osgDB::findDataFile(fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb)).empty())
//			continue;
//		osg::ref_ptr<osg::PagedLOD> plod = new osg::PagedLOD;
//		plod->setFileName(0, fileRoot + "\\GMD_1_16\\OSGBVBO\\" + fileNameOsgb);
//		plod->setRange(0, 1000, 200000);
//		plod->setFileName(1, fileRoot + "\\GMD_1_1\\OSGBVBO\\" + fileNameOsgb);
//		plod->setRange(1, 0, 1000);
//		osgEarth::GeoPoint point(latLong, pos.x(), pos.y(), 10.0, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
//
//		osg::Vec3 rot = _rotMap[fileName];
//		osg::Matrix matRot = osg::Matrix::rotate(rot.x(), osg::Vec3d(1, 0, 0), rot.y(), osg::Vec3d(0, 1, 0), -rot.z(), osg::Vec3d(0, 0, 1));
//
//		osg::Matrix matrix;
//		point.createLocalToWorld(matrix);
//		osg::ref_ptr<osg::MatrixTransform> myMatrixTransform = new osg::MatrixTransform;
//		myMatrixTransform->setMatrix(matRot * matrix);
//		myMatrixTransform->addChild(plod.get());
//		group->addChild(myMatrixTransform.get());
//	}
//	group->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	//_view->getDatabasePager()->registerPagedLODs(group);
//	return group.release();
//}