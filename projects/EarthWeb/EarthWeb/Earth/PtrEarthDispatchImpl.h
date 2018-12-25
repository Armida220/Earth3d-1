/**
	@brief �����ڲ�����ָ��Ľӿ�ʵ��
	@author Hao Luo
	@date 2015/04/12
*/

#ifndef OSGEARTHX_WEB_PTR_EARTH_DISPATCH_IMPL_H
#define OSGEARTHX_WEB_PTR_EARTH_DISPATCH_IMPL_H 1

#include <EarthWeb/Earth/IEarthDispatch.h>

template< class T_ClassType, class T_InnerClass >
class PtrEarthDispatch
{
public:
	PtrEarthDispatch( T_ClassType classType ) : m_ClassType( classType )
	{
		m_pInnerObject = NULL;
	}

	virtual ~PtrEarthDispatch()
	{
		delete m_pInnerObject;
	}

	T_InnerClass* getInnerObject()
	{
		return m_pInnerObject;
	}

	virtual void setInnerObject(T_InnerClass* pObject)
	{
		m_pInnerObject = pObject;
	}

	T_InnerClass* getOrCreateInnerObject()
	{
		if ( m_pInnerObject == NULL )
		{
			m_pInnerObject = new T_InnerClass();
		}
		return m_pInnerObject;
	}

protected:
	const T_ClassType m_ClassType;
	T_InnerClass* m_pInnerObject;

private:
};

template
	<
		class T_ClassType,
		class T_InnerClass, //�ڲ�������
		class T_ImplClass, //ʵ��T_IDispatch����
		class T_IDispatch // IEarthDispatch����
	>
class PtrEarthDispatchImpl : 
	public PtrEarthDispatch<T_ClassType, T_InnerClass >,
	public T_IDispatch
{
public:
	PtrEarthDispatchImpl( T_ClassType classType ) : PtrEarthDispatch<T_ClassType, T_InnerClass >( classType ){}
	virtual ~PtrEarthDispatchImpl(){}

	//T_IDispatch
	virtual bool equal( IEarthDispatchPtr ptr )
	{
		boost::shared_ptr< T_ImplClass > pImplClass = boost::dynamic_pointer_cast< T_ImplClass >( ptr );
		if ( pImplClass.get() == NULL )
		{
			return false;
		}

		return pImplClass->getInnerObject() == this->getInnerObject();
	}

protected:
private:
};


#endif