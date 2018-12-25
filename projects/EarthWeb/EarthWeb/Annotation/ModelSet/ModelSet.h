/**
	@brief ModelSet½Ó¿Ú
	@author Jinping	Li
	@date 2017/05/18
*/

#ifndef OSGEARTHX_WEB_MODELSET_H
#define OSGEARTHX_WEB_MODELSET_H 1

#include <EarthWeb/EarthObject/EarthObjectDispatchImpl.h>
#include <EarthWeb/EarthObject/IEarthObject.h>

//osgEarthX
#include <Annotation/Model.h>

#define ModelSetDispatchImpl EarthObjectDispatchImpl< osgEarthX::ModelSet, ModelSet, IEarthObject >

FB_FORWARD_PTR(ModelSet)
class ModelSet :
	public ModelSetDispatchImpl
{
public:
	ModelSet( const std::vector<FB::variant>* pArgs = NULL ) :
		ModelSetDispatchImpl(ANNOTATION_MODELSET)
	{
		registerMethod("setModelPath",make_method(this,&ModelSet::setModelPath));
	}

	virtual ~ModelSet()
	{

	}

	//impl EarthObject
	virtual bool get_visible();
	virtual void put_visible(bool bVisible);
	virtual bool get_enabled();

	//impl EarthObject
	void setModelPath( const std::wstring& strPath );

protected:
private:
};

#endif