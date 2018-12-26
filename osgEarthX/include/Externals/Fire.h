/**
	@brief Íâ²¿Ìì¿Õ
	@author Hao Luo
	@date 2015/01/13
*/

#ifndef OSGEARTHX_FIRE_H
#define OSGEARTHX_FIRE_H 1

#include <osgEarthXExport/Export.h>
#include <Object/Object.h>

//osgEarthX_Core
#include <Provider/ExternalProvider/FireProvider.h>
namespace osgEarthX
{
	class OSGEARTHX_EXPORT Fire :
		public Object,
		public osgEarthXCore::FireProvider
	{
	public:
		Fire();

		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible(bool bVisible);

	protected:
		virtual ~Fire();
	private:
	};
}

#endif