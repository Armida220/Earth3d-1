/*
	@brief : EarthGroup JS API
	@author : Hao Luo
	@date : 2014/11/22
*/

#ifndef OSGEARTHX_MAP_EARTH_GROUP_H
#define OSGEARTHX_MAP_EARTH_GROUP_H 1

#include <EarthWeb/EarthObject/IEarthObject.h>
#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>

//osgEarthX
#include <Group/Group.h>

#define EarthGroupDispatchImpl EarthObjectDispatchImpl< osgEarthX::Group, EarthGroup, IEarthObject >

FB_FORWARD_PTR(EarthGroup)
class EarthGroup : 
	public EarthGroupDispatchImpl
{
public:
	EarthGroup(const std::vector<FB::variant>* pArgs = NULL) : 
		EarthGroupDispatchImpl( EARTH_GROUP )
	{
		registerProperty("name", make_property(this, &EarthGroup::getName, &EarthGroup::setName));
		registerMethod("addChild",make_method(this,&EarthGroup::addChild));
		registerMethod("removeChild",make_method(this,&EarthGroup::removeChild));
		registerMethod("removeChildByPos", make_method(this, &EarthGroup::removeChildByPos));
		registerMethod("getChild", make_method(this, &EarthGroup::getChild));
		registerMethod("getPosOfChild", make_method(this, &EarthGroup::getPosOfChild));
		registerMethod("moveChild", make_method(this, &EarthGroup::moveChild));
		registerMethod("moveChildFromCurPos", make_method(this, &EarthGroup::moveChildFromCurPos));
		registerMethod("getChildrenCount", make_method(this, &EarthGroup::getChildrenCount));
		registerMethod("removeChildren", make_method(this, &EarthGroup::removeChildren));

		registerMethod("insertChild", make_method( this, &EarthGroup::insertChild ));
		registerMethod("findChild", make_method(this, &EarthGroup::findChild));
		registerMethod("hasChild", make_method(this, &EarthGroup::hasChild));
		registerMethod("removeChildByID", make_method(this, &EarthGroup::removeChildByID));
		registerMethod("removeAllAnnotations", make_method(this, &EarthGroup::removeAllAnnotations));
		registerMethod("getAllChilds", make_method(this, &EarthGroup::getAllChilds));
		registerMethod("resetScene", make_method(this, &EarthGroup::resetScene));
	}

	virtual ~EarthGroup(){}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible( bool bVisible );
	virtual bool get_enabled();

	//EarthGroup
	void setName(const std::wstring& wstrText);
	std::wstring getName();

	void addChild( IEarthObjectPtr ptrEarthObject );
	void removeChild( IEarthObjectPtr ptrEarthObject );
	void removeChildByPos( unsigned int pos );
	FB::JSAPIPtr getChild( unsigned int pos );
	int getPosOfChild( IEarthObjectPtr ptrObject );
	void moveChild( IEarthObjectPtr ptrObject, unsigned int newPos );
	void moveChildFromCurPos( unsigned int curPos, unsigned int newPos );
	unsigned int getChildrenCount();

	void removeChildren();
	void insertChild( IEarthObjectPtr ptrEarthObject, unsigned int uiInsertPos );
	FB::JSAPIPtr findChild(const std::wstring& wstrName,unsigned int tp);
	bool hasChild(const std::wstring & wstrName, unsigned int tp);
	void removeAllAnnotations();
	void resetScene();
	void removeChildByID(const std::wstring& wstrName, unsigned int tp);
	std::vector<std::string> getAllChilds();

protected:

private:
};

#endif