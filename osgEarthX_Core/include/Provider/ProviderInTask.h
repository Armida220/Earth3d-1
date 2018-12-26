/**
	@brief ��Ҫ���뵽�����е�Provider
	@author Hao Luo
	@date 2015/03/30
*/

#ifndef OSGEARTHX_CORE_PROVIDER_IN_TASK_H
#define OSGEARTHX_CORE_PROVIDER_IN_TASK_H 1

#include <Provider/AsynProvider.h>
#include <OpenThreads/ReadWriteMutex>

namespace osgEarthXCore
{
	class ProviderInTask : public AsynProvider
	{
	public:
		//��Map�İ󶨵�״̬
		enum MapBindingState
		{
			NOT_ADDED,//��û׼������Map
			IN_TASK,//�ڼ��ض�����
			ADDED,//�Ѿ�����Map��
			REMOVED//�Ѿ���Map���Ƴ�
		};

	public:
		MapBindingState m_BindingState;
		OpenThreads::ReadWriteMutex m_ProviderInTaskMutex;

	public:
		ProviderInTask();

	protected:
		virtual ~ProviderInTask();
		virtual void initialized();

	private:
	};
}

#endif