/**
	@brief �ص������������̳�CallbackHost��ζ�Ÿ�������ӵ��ִ�лص�������
	@author Hao Luo
	@date 2014/11/08
*/

#ifndef OSGEARTHX_CORE_CALLBACK_HOST_H
#define OSGEARTHX_CORE_CALLBACK_HOST_H 1

#include <osgEarthXCoreExport/Export.h>
#include <Callback/Callback.h>
#include <Utils/DynamicList.h>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT CallbackHost
	{
	public:
		typedef DynamicList< Callback > CallbacksDynamicList;
		typedef CallbacksDynamicList::List CallbacksList;
		typedef CallbacksList::iterator CallbackIterator;

	public:
		virtual ~CallbackHost();

		///��ӻص�����
		void addCallback( Callback* pCallback );

		///�Ƴ��ص�����
		void removeCallback( Callback* pCallback );

	protected:
		///�����ص�����
		///�ڲ��������ͷ�change
		void callback( Change* pChange );

	protected:
		//�ص������б�
		CallbacksDynamicList m_CallbacksDynamicList;

	private:
	};
}

#endif