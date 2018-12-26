/**
	@brief ͼ��ص������ݸı����ʹ����¼�
	@author Hao Luo
	@date 2014/11/08
*/

#ifndef OSGEARTHX_LAYER_EVENT_CALLBACK_H
#define OSGEARTHX_LAYER_EVENT_CALLBACK_H 1

//OSGEARTHX
#include <Layer/LayerEventFirer.h>
#include <Object/ObjectEventCallback.h>

namespace osgEarthX
{
	class LayerEventCallback:
		public ObjectEventCallback
	{
	public:
		LayerEventCallback( LayerEventFirer* pLayerEventFirer );
		virtual void doCallback( osgEarthXCore::Change* pChange );

	protected:
		virtual ~LayerEventCallback();

	protected:
		LayerEventFirer* m_pLayerEventFirer;

	private:
	};
}

#endif