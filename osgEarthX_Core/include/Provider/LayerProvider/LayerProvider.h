/**
	@brief Layer�ṩ�����
	@author Hao Luo
	@date 2014/11/05
*/


#ifndef OSGEARTHX_CORE_LAYER_PROVIDER_H
#define OSGEARTHX_CORE_LAYER_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>
#include <Callback/CallbackHost.h>
#include <ThreadPool/OrderHoldersContainer/OrderHolder.h>
#include <Provider/LayerProvider/LayerTask.h>
#include <Provider/ProviderInTask.h>

#include <string>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT LayerProvider:
		public ProviderInTask,
		public CallbackHost,
		public osgXThreads::OrderHolder
	{
	protected:
		osg::ref_ptr<LayerTask> m_refLayerTask;

	public:
		LayerProvider();
		
		virtual LayerTask* getOrCreateLayerTask() = 0;//�����������������ڶ����߳��ڣ������������߳�

	protected:
		virtual ~LayerProvider();

	};
}

#endif