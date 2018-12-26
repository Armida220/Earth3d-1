/**
	@brief Layer�����������
	@author Hao Luo
	@date 2014/11/05
*/

#ifndef OSGEARTHX_CORE_LAYER_TASK_H
#define OSGEARTHX_CORE_LAYER_TASK_H

#include <osgEarthXCoreExport/Export.h>
#include <ThreadPool/Task.h>

#include <osg/ref_ptr>

namespace osgEarthXCore
{
	class BaseMap;
	class LayerProvider;

	class OSGEARTHX_CORE_EXPORT LayerTask :
		public osgXThreads::Task
	{
	public:
		LayerTask(LayerProvider* pLayerProvider = NULL, BaseMap* pMap = NULL);

		void setBaseMap(BaseMap* pMap);
		void setLayerProvider(LayerProvider* pLayerProvider);
		const LayerProvider* getLayerProvider();

	protected:
		virtual ~LayerTask();

	protected:
		LayerProvider* m_pLayerProvider;//����m_pLayerProvider
		BaseMap* m_pBaseMap;//���ص�m_pBaseMap��
	};
}

#endif