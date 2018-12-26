/**
	@brief Base Class Of Map
	@author Hao Luo
	@date 2015/01/07
*/

#ifndef OSGEARTHX_CORE_BASE_MAP_H
#define OSGEARTHX_CORE_BASE_MAP_H 1

#include <osgEarthXCoreExport/Export.h>

#include <ThreadPool/ThreadPool.h>
#include <Viewer/MapViewer.h>

#include <OpenThreads/ReadWriteMutex>
#include <osgEarth/MapNode>
#include <osgEarthUtil/Controls>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT BaseMap : 
		virtual public osg::Referenced
	{
	public:
		friend MapViewer;

	protected:
		MapViewer* m_pMapViewer;

		osg::ref_ptr<osg::Group> m_refRoot;
		osg::ref_ptr<osgEarth::MapNode> m_refMapNode;
		osg::ref_ptr<osgEarth::Map> m_refMap;
		OpenThreads::ReadWriteMutex m_MapMutex;
		osg::ref_ptr<osg::Group> m_refModels;
		osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> m_refCanvas;
		osg::ref_ptr< osg::Group > m_refAnalysis;
	private:
		osgXThreads::ThreadPool* m_pThreadPool;///应该使用getOrCreateThreadPool函数访问该指针，不应直接调用

	public:
		BaseMap();
		virtual ~BaseMap();

		osg::Group* getMapRoot();
		osgEarth::Map* getOsgEarthMap();
		osgEarth::MapNode* getMapNode();
		OpenThreads::ReadWriteMutex& getMapMutex();
		const osgEarth::SpatialReference* getSRS();

		///线程相关
		///设置最大线程数
		void setMaxThreadsNum(unsigned int num);
		unsigned int getMaxThreadsNum();
		void setupViewShed(osg::Vec3d pos, osg::Vec3 dir);
		void setMaskLayer(std::vector<double> &cords, double dHeight, bool bOpacity);
		void removeMaskLayer();
	protected:
		virtual void initMapViewer( MapViewer* pMapViewer );

		///获取或者创建线程池
		osgXThreads::ThreadPool* getOrCreateThreadPool();

		void addGroupObject( osg::Group* pGroup, osg::Node* pNode );
		void removeGroupObjet( osg::Group* pGroup, osg::Node* pNode );
		void removeAllGroupObjet(osg::Group * pGroup);
	};

	class OSGEARTHX_CORE_EXPORT AddGroupObjectOperation : public osg::Operation
	{
	public:
		AddGroupObjectOperation( osg::Group* pGroup, osg::Node* pNode );

		virtual void operator()( osg::Object* );

	protected:
		virtual ~AddGroupObjectOperation();

	protected:
		osg::ref_ptr<osg::Group> m_refGroup;
		osg::ref_ptr<osg::Node> m_refNode;

	private:
	};

	class OSGEARTHX_CORE_EXPORT RemoveGroupObjectOperation : public osg::Operation
	{
	public:
		RemoveGroupObjectOperation( osg::Group* pGroup, osg::Node* pNode );
		virtual void operator()( osg::Object* );

	protected:
		virtual ~RemoveGroupObjectOperation();

	protected:
		osg::ref_ptr<osg::Group> m_refGroup;
		osg::ref_ptr<osg::Node> m_refNode;

	private:
	};

	class OSGEARTHX_CORE_EXPORT RemoveAllGroupObjectOperation : public osg::Operation
	{
	public:
		RemoveAllGroupObjectOperation(osg::Group* pGroup);
		virtual void operator()(osg::Object*);

	protected:
		virtual ~RemoveAllGroupObjectOperation();

	protected:
		osg::ref_ptr<osg::Group> m_refGroup;
	private:
	};
}

#endif