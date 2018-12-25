#include <EarthWeb/EarthGroup/EarthGroup.h>

#include <EarthWeb/Utils/StaticUtils/EarthObjectStaticUtils.h>
#include <atlstr.h>

bool EarthGroup::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void EarthGroup::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool EarthGroup::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void EarthGroup::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring EarthGroup::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

void EarthGroup::addChild( IEarthObjectPtr ptrEarthObject )
{
	if ( ! ptrEarthObject )
	{
		return;
	}
	getOrCreateInnerObject()->addChild( EarthObjectStaticUtils::getInnerObject( ptrEarthObject.get(), true ) );
}

void EarthGroup::removeChild( IEarthObjectPtr ptrEarthObject )
{
	if (!ptrEarthObject.get())
	{
		return;
	}
	getOrCreateInnerObject()->removeChild( EarthObjectStaticUtils::getInnerObject( ptrEarthObject.get() ) );
}

void EarthGroup::removeChildByPos(unsigned int pos)
{
	getOrCreateInnerObject()->removeChildByPos( pos );
}

FB::JSAPIPtr EarthGroup::getChild(unsigned int pos)
{
	osgEarthX::Object* pObject = getOrCreateInnerObject()->getChild( pos );
	return EarthObjectStaticUtils::createInstance( ( EarthObjectClassType ) pObject->getObjectType(), pObject );
}

int EarthGroup::getPosOfChild(IEarthObjectPtr ptrObject)
{
	if ( !ptrObject.get() )
	{
		return -1;
	}

	return getOrCreateInnerObject()->getPosOfChild( EarthObjectStaticUtils::getInnerObject( ptrObject.get() ) );
}

void EarthGroup::moveChild(IEarthObjectPtr ptrObject, unsigned int newPos)
{
	if (!ptrObject.get())
	{
		return;
	}

	getOrCreateInnerObject()->moveChild( EarthObjectStaticUtils::getInnerObject( ptrObject.get() ) , newPos );
}

void EarthGroup::moveChildFromCurPos(unsigned int curPos, unsigned int newPos)
{
	getOrCreateInnerObject()->moveChildFromCurPos(curPos, newPos);
}

unsigned int EarthGroup::getChildrenCount()
{
	return getOrCreateInnerObject()->getChildrenCount();
}

void EarthGroup::removeChildren()
{
	getOrCreateInnerObject()->removeChildren();
}

void EarthGroup::insertChild(IEarthObjectPtr ptrEarthObject, unsigned int uiInsertPos)
{
	if (!ptrEarthObject)
	{
		return;
	}

	getOrCreateInnerObject()->insertChild( EarthObjectStaticUtils::getInnerObject( ptrEarthObject.get(), true ) , uiInsertPos);
}

FB::JSAPIPtr EarthGroup::findChild(const std::wstring& wstrName, unsigned int tp)
{
	osgEarthX::Object* pObject = getOrCreateInnerObject()->findChild(CStringA(wstrName.data()).GetString(),tp );
	if(pObject)
	    return EarthObjectStaticUtils::createInstance((EarthObjectClassType)pObject->getObjectType(), pObject);
	return EarthObjectStaticUtils::createInstance(ANNOTATION_PLACE);
}

bool EarthGroup::hasChild(const std::wstring& wstrName, unsigned int tp)
{
	osgEarthX::Object* pObject = getOrCreateInnerObject()->findChild(CStringA(wstrName.data()).GetString(), tp);
	if (pObject)
		return true;
	return false;
}

std::vector<std::string> EarthGroup::getAllChilds()
{
	std::vector<std::string> strIDs;
	getOrCreateInnerObject()->getAnnotations(strIDs);
	return strIDs;
}

void EarthGroup::removeAllAnnotations()
{
	getOrCreateInnerObject()->removeAllAnnotations();
}

void EarthGroup::resetScene()
{
	getOrCreateInnerObject()->resetScene();
}

void EarthGroup::removeChildByID(const std::wstring & wstrName, unsigned int tp)
{
	osgEarthX::Object* pObject = getOrCreateInnerObject()->findChild(CStringA(wstrName.data()).GetString(), tp);
	if (pObject)
		getOrCreateInnerObject()->removeChild(pObject);
}
