/**
	@brief ŒÔÃÂPlace
	@author Hao Luo
	@date 2015/04/01
*/

#ifndef OSGEARTHX_LOCATION_H
#define OSGEARTHX_LOCATION_H 1

#include <osgEarthXExport/Export.h>
#include <Object/Object.h>

//osgEarthX_Core
#include <Provider/AnnotationProvider/LocationProvider.h>

namespace osgEarthX
{
	class OSGEARTHX_EXPORT Location : 
		public Object,
		public osgEarthXCore::LocationProvider
	{
	public:
		Location();

		virtual bool isEnabled();
		virtual bool isVisible();
		virtual void setVisible( bool bVisible );
	protected:
		virtual ~Location();

		bool  m_bVisible;
	private:
	};
}

#endif