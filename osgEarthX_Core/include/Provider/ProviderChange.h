/**
	@brief �ṩ��״̬�仯��
	@author Hao Luo
	@date 2014/11/07
*/

#ifndef OSGEARTHX_CORE_PROVIDER_CHANGE_H
#define OSGEARTHX_CORE_PROVIDER_CHANGE_H 1

#include <osgEarthXCoreExport/Export.h>
#include <Callback/Change.h>

namespace osgEarthXCore
{
	//�����ı�����ö��
	enum OSGEARTHX_CORE_EXPORT ProviderChangeType
	{
		INITIALIZE_COMPLETED
	};

	typedef ChangeTemplate<ProviderChangeType> ProviderChange;
}

#endif