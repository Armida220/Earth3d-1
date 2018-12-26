#include <ThreadPool/LockMutex.h>

#include <Map/ImageLayerMap.h>

#include <Provider/LayerProvider/ImageLayerTask.h>
#include <Provider/ProviderChange.h>
using namespace osgEarthXCore;

#include <cassert>

ImageLayerTask::ImageLayerTask( ImageLayerProvider* pImageLayerProvider /* = NULL */, ImageLayerMap* pMap /* = NULL */ ):
	LayerTask( pImageLayerProvider, pMap )
{

}

ImageLayerTask::~ImageLayerTask()
{

}

void ImageLayerTask::doTask()
{
	ImageLayerProvider* pImageLayerProvider = dynamic_cast< ImageLayerProvider* >( m_pLayerProvider );

	if ( pImageLayerProvider == NULL || pImageLayerProvider->isInitialized() )
	{
		return;
	}

	pImageLayerProvider->initialized();

	ImageLayerMap* pImageLayerMap = dynamic_cast< ImageLayerMap* >( m_pBaseMap );
	if ( pImageLayerMap != NULL )
	{
		SCOPEDLOCK_WRITE_MUTEX( pImageLayerProvider->m_ProviderInTaskMutex );//����
		pImageLayerMap->_addAndFixOrderOfImgLyerProv( pImageLayerProvider );//�ڲ�����pImageLayerProvider��״̬
	}

	ProviderChangeType initialzeCompleted = INITIALIZE_COMPLETED;
	ProviderChange providerChange( initialzeCompleted );
	pImageLayerProvider->callback( & providerChange );
}