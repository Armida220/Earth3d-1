#include <Earth/Earth.h>
using namespace osgEarthX;

#include <Earth/EarthRootCallback.h>

#include <cassert>

Earth::Earth()
{
	m_refRoot = new NotifiedGroup();
	m_refRoot->addCallback( new EarthRootCallback( this ) );
}

Earth::~Earth()
{
}

NotifiedGroup* Earth::getEarthRoot()
{
	return m_refRoot;
}

EarthViewer* Earth::getEarthViewer()
{
	if (m_refEarthViewer == NULL)
	{
		m_refEarthViewer = new EarthViewer;
	}

	return m_refEarthViewer;
}

void Earth::setEarthViewer( EarthViewer* pEarthViewer )
{
	assert(pEarthViewer != NULL);
	m_refEarthViewer = pEarthViewer;
}

EarthMap* Earth::getEarthMap()
{
	if (m_refEarthMap == NULL)
	{
		m_refEarthMap = new EarthMap;
	}

	return m_refEarthMap;
}

void Earth::setEarthMap( EarthMap*pEarthMap )
{
	assert(pEarthMap != NULL);
	m_refEarthMap = pEarthMap;
}

void Earth::initialize()
{
	getEarthViewer()->setMap( getEarthMap() );
}

void Earth::run()
{
	initialize();
	getEarthViewer()->run();
}

void Earth::setActiveSky( Sky* pSky )
{
	getEarthMap()->setSkyProvider( pSky );
}

void Earth::removeActiveSky()
{
	getEarthMap()->removeSkyProvider();
}

void Earth::setHomeViewpoint( Viewpoint* pViewpoint )
{
	getEarthViewer()->getOrCreateEarthManipulator()->setHomeViewpoint( *pViewpoint );
}

Viewpoint* Earth::getViewpoint(  )
{
	if(m_refViewpoint == NULL)
	{
		m_refViewpoint = new osgEarthX::Viewpoint;
		m_refViewpoint->bindEarthManipulator( getEarthViewer()->getOrCreateEarthManipulator() );	
	}
	osgEarth::Viewpoint vp = getEarthViewer()->getOrCreateEarthManipulator()->getViewpoint( );
	m_refViewpoint->x() = vp.x();
	m_refViewpoint->y() = vp.y();
	m_refViewpoint->z() = vp.z();
	m_refViewpoint->setHeading(vp.getHeading());
	m_refViewpoint->setPitch(vp.getPitch());
	m_refViewpoint->setRange(vp.getRange());

	return m_refViewpoint;
}

void Earth::setActiveGraticule( Graticule* pGraticule )
{
	getEarthMap()->setGraticuleProvider( pGraticule );
}

void Earth::removeActiveGraticule()
{
	getEarthMap()->removeGraticuleProvider();
}