/**
	@brief ʵ��IImageLayerWrapper��ImageLayerOptions��װ��
	@author Hao Luo
	@date 2015/01/25
*/

#ifndef OSGEARTHX_CORE_IMAGE_LAYER_OPTIONS_WRAPPER_H
#define OSGEARTHX_CORE_IMAGE_LAYER_OPTIONS_WRAPPER_H 1

#include <osgEarthXCoreExport/Export.h>

#include <OSGEarth/Layer/IImageLayerWrapper.h>
#include <OSGEarth/Layer/TerrainLayerOptionsWrapper.h>
#include <osgEarth/ImageLayer>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT ImageLayerOptionsWrapper : 
		public IImageLayerWrapper,
		public TerrainLayerOptionsWrapper
	{
	public:
		ImageLayerOptionsWrapper( osgEarth::ImageLayerOptions* pOptions );
		virtual ~ImageLayerOptionsWrapper();

	protected:
		osgEarth::ImageLayerOptions* m_pImageLayerOptions;

	private:
	};

}

#endif