/**
	@brief Model Object
	@author Hao Luo
	@date 2015/10/24
*/

#ifndef OSGEARTHX_MODEL_H
#define OSGEARTHX_MODEL_H 1

#include <osgEarthXExport/Export.h>
#include <Object/Object.h>
#include <osgEarthUtil/AnnotationEvents>

//osgEarthX_Core
#include <Provider/AnnotationProvider/ModelProvider.h>

namespace osgEarthX
{
	class OSGEARTHX_EXPORT Model :
		public Object,
		public osgEarthXCore::ModelProvider
	{
	public:
		Model();

		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible(bool bVisible);

	protected:
		virtual ~Model();

	private:
	};

	class OSGEARTHX_EXPORT ModelSet :
		public Object
	{
	public:
		ModelSet();

		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible(bool bVisible);
		virtual void initMapNode(osgEarth::MapNode* pMapNode);
		void setModelPath(const std::string& modelPath);
		void OpenModelFile(const std::string & modelPath);
		osg::Node* batchLoadingModels();
		osg::Group* getNodes();
		void setAnnnotationEventCallback(osgEarth::Util::AnnotationEventCallback* cb);
	protected:
		virtual ~ModelSet();

	private:
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		std::map<std::string, osg::Vec3d> _modelMap;
		std::map<std::string, osg::Vec3d> _rotMap;
		std::map<std::string, std::string> _nameMap;
		std::map<std::string, std::string> _isSelected;

		std::string _strModelFile;

		osg::ref_ptr<osg::Group> _group;
		osg::ref_ptr<osgEarth::Util::AnnotationEventCallback> m_refCB;
	};
}

#endif