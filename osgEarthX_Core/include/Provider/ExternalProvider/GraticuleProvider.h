/**
	@brief ��γ���ṩ��
	@author Hao Luo
	@date 2015/03/30
	@note Graticule�������Ҫ�ķ�һ��ʱ�䣬��Ȼ��������Ⱦ�����ǻ����������̣߳�����ʵ��Ӧ����ռ��ʱ����������Կ��Ǽ���Task
*/

#ifndef OSGEARTHX_CORE_GRATICULE_PROVIDER_H
#define OSGEARTHX_CORE_GRATICULE_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>

#include <Provider/AsynProvider.h>
#include <osgEarthUtil/GeodeticGraticule>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT GraticuleProvider : public AsynProvider
	{
	private:
		osg::ref_ptr< osgEarth::Util::GeodeticGraticule > m_refGeodeticGraticule;

	public:
		GraticuleProvider();

		void initGeodeticGraticule( osgEarth::MapNode* pMapNode );

		osgEarth::Util::GeodeticGraticule* getGeodeticGraticule()
		{
			return m_refGeodeticGraticule;
		}

	protected:
		virtual ~GraticuleProvider();

	private:
	};
}

#endif