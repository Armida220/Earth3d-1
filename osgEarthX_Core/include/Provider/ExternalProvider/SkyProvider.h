/**
	@brief ��ս���ṩ��
	@author Hao Luo
	@date 2015/01/14
*/

#ifndef OSGEARTHX_CORE_SKY_PROVIDER_H
#define OSGEARTHX_CORE_SKY_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>

#include <Provider/AsynProvider.h>
#include <OSGEarth/Externals/ISkyWrapper.h>
#include <OSGEarth/Externals/TSkyAdapter.h>

#include <osgEarthUtil/Sky>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT SkyProvider : 
		public AsynProvider,
		public TSkyAdapter<ISkyWrapper>
	{
	protected:
		osg::ref_ptr<osgEarth::Util::SkyNode> m_refSkyNode;
		osgEarth::Util::SkyOptions* m_pSkyOptions;

	public:
		SkyProvider();

		void initSky( osgEarth::MapNode* pMapNode );//����MapNode��ʼ��SkyNode������SkyNode���MapNodeΪChild
		osgEarth::Util::SkyNode* getSkyNode();

	protected:
		virtual ~SkyProvider();
		virtual void initialized();

	private:
	};
}

#endif