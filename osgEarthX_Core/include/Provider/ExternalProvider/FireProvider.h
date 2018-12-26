/**
	@brief 天空结点提供者
	@author Hao Luo
	@date 2015/01/14
*/

#ifndef OSGEARTHX_CORE_FIRE_PROVIDER_H
#define OSGEARTHX_CORE_FIRE_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>
#include <osg/MatrixTransform>
#include <OSGEarth/MapNode>
#include <osgEarth/GeoData>
namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT FireProvider : 
		public osg::MatrixTransform
	{
	protected:

	public:
		FireProvider();

		void initMapNode( osgEarth::MapNode* pMapNode );
		void createFireBall(osg::MatrixTransform* fireNode);
		//根据MapNode初始化SkyNode，并且SkyNode添加MapNode为Child
		void setPosition(const osgEarth::GeoPoint& pos);
		osgEarth::GeoPoint getPosition();
		osg::Group* getFireNode();
	protected:
		virtual ~FireProvider();
		virtual void initialized();
		void creatFire();
		void creatFireInside(osg::MatrixTransform* Node);

		osg::ref_ptr<osgEarth::MapNode>   _mapNode;
		osg::ref_ptr<osg::Group> _fireNode;
		osgEarth::GeoPoint _mapPoint;
	private:
	};
}

#endif