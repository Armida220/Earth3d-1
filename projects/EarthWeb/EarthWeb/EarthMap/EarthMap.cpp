#include <EarthWeb/EarthMap/EarthMap.h>
#include <EarthWeb/Utils/StaticUtils/EarthObjectStaticUtils.h>

unsigned int EarthMap::get_maxThreadsNum()
{
	return getOrCreateInnerObject()->getMaxThreadsNum();
}

void EarthMap::put_maxThreadsNum(unsigned int num)
{
	getOrCreateInnerObject()->setMaxThreadsNum(num);
}

FB::JSAPIPtr EarthMap::get_SRS()
{
	osgEarth::SpatialReference * pObject = const_cast<osgEarth::SpatialReference *>(getOrCreateInnerObject()->getSRS());
	return EarthObjectStaticUtils::createInstance(SPATIAL_REFERENCE, pObject);
}

void EarthMap::put_SRS(IEarthObjectPtr ptrObject)
{

}