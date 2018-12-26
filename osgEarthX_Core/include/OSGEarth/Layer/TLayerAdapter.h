/**
	@brief Layer�ṩ��ģ����࣬���ڲ�����ӿڽ���ת����ʹ����ͳһ
	@author Hao Luo
	@date 2015/01/24
*/

#ifndef OSGEARTHX_CORE_T_LAYER_ADAPTER_H
#define OSGEARTHX_CORE_T_LAYER_ADAPTER_H 1

#include <OSGEarth/Layer/ILayerWrapper.h>

namespace osgEarthXCore
{
	template< class T = ILayerWrapper>
	class TLayerAdapter :
		virtual public ILayerWrapper
	{
	protected:
		T* m_pIWrapper;

	public:
		TLayerAdapter()
		{
			m_pIWrapper = 0;
		}

		virtual ~TLayerAdapter()
		{

		}

		virtual bool getEnabled()
		{
			return m_pIWrapper->getEnabled();
		}

		virtual bool getVisible() const
		{
			return m_pIWrapper->getVisible();
		}

		virtual void setVisible( bool value )
		{
			m_pIWrapper->setVisible( value );
		}

	public:

	private:
	};
}

#endif