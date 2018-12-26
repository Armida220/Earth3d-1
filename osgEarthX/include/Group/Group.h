/**
	@brief ������
	@author Hao Luo
	@date 2014/11/06
*/

#ifndef OSGEARTHX_GROUP_H
#define OSGEARTHX_GROUP_H 1

#include <osgEarthXExport/Export.h>

#include <Object/Object.h>

//osgEarthX_Core
#include <Utils/DynamicVector.h>
#include <Callback/CallbackHost.h>

namespace osgEarthX
{
	class NotifiedGroupCallback;

	typedef DynamicVector<Object> ObjectsDynamicVector;

	class OSGEARTHX_EXPORT Group:
		public Object,
		public osgEarthXCore::CallbackHost
	{
	public:
		friend NotifiedGroupCallback;

	public:
		Group();
		void setName(const std::string& strText);
		std::string getName();

		void addChild( Object*pObject );///�������
		void insertChild( Object* pObject, unsigned int pos );//��������
		void removeChild( Object* pObject );///ɾ������
		void removeChildByPos( unsigned int pos );///ɾ���ض�λ���ϵ����壬��removeChild(Object* pObject)��
		void removeAllAnnotations();
		void resetScene();

		Object* getChild( unsigned int pos );///����λ�û�ȡ����
		int getPosOfChild( Object*pObject );///��ȡ����λ��

		void moveChild( Object* pObject, unsigned int newPos );//�������ƶ���newPosλ�á�
		void moveChildFromCurPos( unsigned int curPos, unsigned int newPos );///��curPosλ���ϵ������ƶ���newPosλ�á���moveChild(Object* pObject, unsigned int newPos)��

		unsigned int getChildrenCount();
		void removeChildren();
		Object* findChild(const std::string strName, unsigned int tp);
		void getAnnotations(std::vector<std::string> &ids);
		//ʵ��Object���麯��
		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible( bool bVisible = true );

	public:
		ObjectsDynamicVector& getObjects();

		//�ڴ��鼰�����ڣ������������ĳ���ڵ��ͬ���ͽڵ������ֱ���ҵ��ýڵ��ֹͣͳ��
		//������ͳ��ImageSurfaceLayer��VectorSurfaceLayer����ȷ����ʾ����
		//����ҵ��򷵻�true
		bool countObjectsOfSameKindUntil( Object* pObject, unsigned int&count );
        
	protected:
		virtual ~Group();

		void _removeObjectCallback( Object* pObject );///�Ƴ�pObject,�����ص�
		void _addObjectCallback( Object* pObject );///���pObject, �����ص�
		void _insertObjectCallback( Object* pObject );
		void _moveObjectCallback( Object*pObject );

	private:
		///���嶯̬�б�
		ObjectsDynamicVector m_ObjectsVector;
		std::string m_name;
		///�Ƿ�ɼ�
		bool m_bVisible;
	};
}

#endif