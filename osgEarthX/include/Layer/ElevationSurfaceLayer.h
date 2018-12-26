/**
	@brief �������߳�ͼ��
	@author Hao Luo
	@date 2015/01/16
*/

#ifndef OSGEARTHX_ELEVATION_SURFACE_LAYER_H
#define OSGEARTHX_ELEVATION_SURFACE_LAYER_H 1

#include <osgEarthXExport/Export.h>
#include <Object/Object.h>

//osgEarthX_Core
#include <Provider/LayerProvider/ElevationLayerProvider.h>
#include <Event/EventHost.h>
#include <Layer/LayerEventFirer.h>

namespace osgEarthX
{
	class OSGEARTHX_EXPORT ElevationSurfaceLayer :
		public Object,
		public osgEarthXCore::ElevationLayerProvider,
		public EventHost<LayerEventFirer>
	{
	public:
		ElevationSurfaceLayer();

		//ʵ��Object���麯��
		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible(bool bVisible);

		//����EventHost<LayerEventFirer>
		virtual void setFirer(LayerEventFirer* pFirer);


	protected:
		virtual ~ElevationSurfaceLayer();

	private:
	};
}

#endif