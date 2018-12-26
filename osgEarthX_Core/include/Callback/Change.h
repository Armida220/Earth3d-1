/**
	@brief ״̬�仯���༰ģ��
	@author Hao Luo
	@date 2014/11/08
*/

#ifndef OSGEARTHX_CORE_CHANGE_H
#define OSGEARTHX_CORE_CHANGE_H 1

#include <osgEarthXCoreExport/Export.h>

namespace osgEarthXCore
{
	//�ı����
	class OSGEARTHX_CORE_EXPORT Change
	{
	public:
		virtual ~Change()
		{

		}
	};

	//�ı�ģ��
	template <class ChangeType>
	class ChangeTemplate:
		virtual public Change
	{
	public:
		ChangeTemplate()
		{
		}

		///�����ڲ��仯����
		///�ڲ��������ͷ�changeType
		ChangeTemplate(ChangeType& changeType)
		{
			setChangeType(changeType);
		}

		///�����ڲ��仯����
		///�ڲ��������ͷ�changeType
		void setChangeType(ChangeType& changeType)
		{
			m_Type = changeType;
		}

		virtual ~ChangeTemplate()
		{

		}


		//��Ϊ���ܻ���ڼ̳ж��ָı����ͣ����Բ������麯��
		//����ͨ��ת���ɻ����ָ��Ϳ��Ի�ȡ����ĸı�����
		//not virtual !
		ChangeType& getChangeType()
		{
			return m_Type;
		}

	protected:
		ChangeType m_Type;
	private:
	};

}

#endif