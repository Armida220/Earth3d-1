/**
	@brief ���ݽӿ�ʵ��ģ��
	@author Hao Luo
	@date 2015/03/27
*/

#ifndef OSGEARTHX_WEB_SOURCE_DISPATCH_IMPL_H
#define OSGEARTHX_WEB_SOURCE_DISPATCH_IMPL_H 1

#include <EarthWeb/Earth/PtrEarthDispatchImpl.h>
#include <EarthWeb/Source/SourceClassType.h>

template
	<
		class T_InnerClass, //�ڲ�������
		class T_ImplClass, //ʵ��T_IDispatch����
		class T_IDispatch // ISourceDispatch����
	>
class SourceDispatchImpl : 
	public PtrEarthDispatchImpl< SourceClassType, T_InnerClass, T_ImplClass, T_IDispatch >
{
public:
	SourceDispatchImpl( SourceClassType classType ) : PtrEarthDispatchImpl< SourceClassType, T_InnerClass, T_ImplClass, T_IDispatch >( classType ){}
	virtual ~SourceDispatchImpl(){}

	virtual SourceClassType get_classType()
	{
		return m_ClassType;
	}

protected:
private:
};

#endif