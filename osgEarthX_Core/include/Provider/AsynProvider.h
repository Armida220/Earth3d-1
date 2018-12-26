/**
	@brief Map�е�ͼ�㡢�ⲿ����ȵ��첽�ṩ�߻���
	@author Hao Luo
	@date 2015/01/14
*/

#ifndef OSGEARTHX_CORE_ASYN_PROVIDER_H
#define OSGEARTHX_CORE_ASYN_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>
#include <osg/Referenced>

namespace osgEarthXCore
{
	//������ʱ������������Ķ���
	class OSGEARTHX_CORE_EXPORT AsynProvider : 
		virtual public osg::Referenced
	{
	public:
		enum ProviderState
		{
			UNINITIALIZED,//δ��ʼ��
			INITIALIZED//�ѳ�ʼ��
		};

	public:
		ProviderState m_ProviderState;

	public:
		AsynProvider();

	protected:
		virtual ~AsynProvider();

		virtual void initialized();
		virtual bool isInitialized();

	private:
	};
}

#endif