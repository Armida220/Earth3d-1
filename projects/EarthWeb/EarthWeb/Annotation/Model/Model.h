/**
	@brief Model½Ó¿Ú
	@author Jinping	Li
	@date 2017/04/28
*/

#ifndef OSGEARTHX_WEB_MODEL_H
#define OSGEARTHX_WEB_MODEL_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>
#include <EarthWeb/Basis/Geometry/LineString/GeoLine.h>
//osgEarthX
#include <Annotation/Model.h>

#define ModelDispatchImpl EarthObjectDispatchImpl< osgEarthX::Model, Model, IEarthObject >

FB_FORWARD_PTR(Model)
class Model :
	public ModelDispatchImpl
{
public:
	Model( const std::vector<FB::variant>* pArgs = NULL ) :
		ModelDispatchImpl( ANNOTATION_MODEL )
	{
		registerProperty("name", make_property(this, &Model::getName, &Model::setName));
		registerProperty("play", make_property(this, &Model::getPlay, &Model::setPlay));
		registerProperty("speed", make_property(this, &Model::getSpeed, &Model::setSpeed));
		registerProperty("action", make_property(this, &Model::getAction, &Model::setAction));
		registerProperty("visible", make_property(this, &Model::get_visible, &Model::put_visible));
		registerMethod("setModelPath",make_method(this,&Model::setModelPath));
		registerMethod("setPosition",make_method(this,&Model::setPosition));
		registerMethod("getPosition", make_method(this, &Model::getPosition));
		registerMethod("setScale", make_method(this, &Model::setScale));
		registerMethod("setRotate", make_method(this, &Model::setRotate));
		registerMethod("startEdit", make_method(this, &Model::startEdit));
		registerMethod("stopEdit", make_method(this, &Model::stopEdit));
		registerMethod("grow", make_method(this, &Model::grow));
		registerMethod("setPath", make_method(this, &Model::setPath));
	}

	virtual ~Model()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	//impl EarthObject
	void setModelPath( const std::wstring& strPath );
	void setPosition(GeoPointPtr ptrGeoPoint);
	GeoPointPtr getPosition();
	void setScale(double x, double y, double z);
	void setRotate(double x, double y, double z);

	void setName(const std::wstring& wstrText);
	std::wstring getName();
	void startEdit();
	void stopEdit();
	void grow();
	void setPlay(bool bPlay);
	bool getPlay();
	void setSpeed(double dSpeed);
	double getSpeed();
	void setPath(GeoLinePtr lin);
	void setAction(int iAct);
	int getAction();
protected:
private:
};

#endif