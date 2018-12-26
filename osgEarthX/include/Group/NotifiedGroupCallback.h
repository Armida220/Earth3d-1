/**
	@brief ֪ͨ����״̬�����ı�Ļص�
	@author Hao Luo
	@date 2014/11/07
	@description ��ǰ״̬�����仯��֪ͨ���ڵ㡣ʹ��ʱ�����ڵ㴴���ûص��������ӽڵ㣬�����ӽڵ㣬
						���ӽڵ�״̬�����仯ʱ��Ӧ��֪ͨ���ڵ㡣
*/

#ifndef OSGEARTHX_NOTIFIED_GROUP_CALLBACK_H
#define OSGEARTHX_NOTIFIED_GROUP_CALLBACK_H 1

//OSGEARTHX_CORE
#include <Callback/Callback.h>

namespace osgEarthX
{
	class Group;

	class NotifiedGroupCallback:
		public osgEarthXCore::Callback
	{
	public:
		NotifiedGroupCallback( Group*pGroup );//֪ͨ��pGroup

		virtual void doCallback( osgEarthXCore::Change* pChange );

	protected:
		virtual ~NotifiedGroupCallback();

	protected:
		Group* m_pParentGroup;
	};
}

#endif