/**
	@brief Feature Provider
	@author Hao Luo
	@date 2015/10/31
*/

#ifndef OSGEARTHX_CORE_FEATURE_H
#define OSGEARTHX_CORE_FEATURE_H 1

#include <osgEarthXCoreExport/Export.h>
#include <osgEarthFeatures/Feature>

namespace osgEarthXCore
{
	class OSGEARTHX_CORE_EXPORT Feature : public osgEarth::Features::Feature
	{
	public:
		static void getDefaultPolygonStyle( osgEarth::Symbology::Style& outStyle );
		static void getDefaultLineStringStyle( osgEarth::Symbology::Style& outStyle );
		
	public:
		Feature();

		void init( const osgEarth::SpatialReference* pSRS );
		void setColor(unsigned int col);
		void setFillcolor(unsigned int col);
		void setWidth(float w);
		void setClamp(bool bClamp);
		void setPattern(unsigned short pattern);
	protected:
		virtual ~Feature();
		static unsigned int _col;
		static unsigned int _fillcol;
		static float _width;
		static bool _bClamp;
		static unsigned short _pattern;
	private:
	};
}

#endif