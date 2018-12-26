#include <Provider/LayerProvider/ImageLayerProvider.h>
#include <Provider/LayerProvider/ImageLayerTask.h>

#include <OSGEarth/Layer/ImageLayerWrapper.h>
#include <OSGEarth/Layer/ImageLayerOptionsWrapper.h>
using namespace osgEarthXCore;

ImageLayerProvider::ImageLayerProvider()
{
	m_refImageLayer = NULL;

	m_pImageLayerOptions = new osgEarth::ImageLayerOptions();
	m_pIWrapper = new ImageLayerOptionsWrapper ( m_pImageLayerOptions );
}

ImageLayerProvider::~ImageLayerProvider()
{
	delete m_pIWrapper;
	delete m_pImageLayerOptions;
}


LayerTask* ImageLayerProvider::getOrCreateLayerTask()
{
	if ( m_refLayerTask.get() == NULL )
	{
		m_refLayerTask = new ImageLayerTask( this );
	}
	return m_refLayerTask;
}

osgEarth::ImageLayer* ImageLayerProvider::getImageLayer()
{
	return m_refImageLayer;
}

void ImageLayerProvider::initialized()
{
	if ( m_refImageLayer.valid() )
	{
		return;
	}
	m_refImageLayer = new ImageLayer( *m_pImageLayerOptions );

	//
	delete m_pIWrapper;//ɾ��ԭ�еĽӿڵ��ð�װ
	delete m_pImageLayerOptions;
	m_pImageLayerOptions = NULL;

	m_pIWrapper = new ImageLayerWrapper( m_refImageLayer );

	LayerProvider::initialized();
}

void ImageLayerProvider::setTileSource( osgEarth::TileSourceOptions& tileSourceOptions )
{
	if ( m_pImageLayerOptions )
	{
		m_pImageLayerOptions->driver() = tileSourceOptions;
	}
}

float ImageLayerProvider::getOpacity()
{
	if (m_refImageLayer)
		return m_refImageLayer->getOpacity();
	return 1.0;
}

void ImageLayerProvider::setOpacity(float opa)
{
	if (m_refImageLayer)
		m_refImageLayer->setOpacity(opa);
}