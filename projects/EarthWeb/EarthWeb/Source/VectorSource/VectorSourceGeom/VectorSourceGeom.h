#ifndef OSGEARTHX_WEB_VECTOR_SOURCE_GEOM_H
#define OSGEARTHX_WEB_VECTOR_SOURCE_GEOM_H 1

#include <EarthWeb/Source/SourceDispatchImpl.h>
#include <EarthWeb/Source/VectorSource/IVectorSourceDispatch.h>
#include <EarthWeb/Source/FeatureSource/IFeatureSourceDispatch.h>

//osgEarth
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#define VectorSourceGeomDispatchImpl SourceDispatchImpl< osgEarth::Drivers::FeatureGeomModelOptions, VectorSourceGeom, IVectorSourceDispatch >

FB_FORWARD_PTR(VectorSourceGeom)
class VectorSourceGeom : 
	public VectorSourceGeomDispatchImpl
{
public:
	VectorSourceGeom( const std::vector<FB::variant>* pArgs = NULL );

	virtual ~VectorSourceGeom(){}
private:
	void _register()
	{
		registerProperty( "featureSource", make_property( this, &VectorSourceGeom::get_featureSource, &VectorSourceGeom::put_featureSource ) );
		registerProperty("stock", make_property(this, &VectorSourceGeom::get_stock, &VectorSourceGeom::put_stock));
		registerProperty("fill", make_property(this, &VectorSourceGeom::get_fill, &VectorSourceGeom::put_fill));
		registerProperty("extrude", make_property(this, &VectorSourceGeom::get_extrude, &VectorSourceGeom::put_extrude));
		registerMethod("addextrude", make_method(this, &VectorSourceGeom::addextrude));
		registerMethod("addfill", make_method(this, &VectorSourceGeom::addfill));
		registerMethod("setStockEx", make_method(this, &VectorSourceGeom::setStockEx));
	}

public:
	IFeatureSourceDispatchPtr get_featureSource();
	void put_featureSource( IFeatureSourceDispatchPtr ptrIFeatureSourceDispatch );

	std::wstring get_stock();
	void put_stock(std::wstring col);
	void setStockEx(std::wstring col,double width,unsigned short pattern);

	std::wstring get_fill();
	void put_fill(std::wstring col);

	std::wstring get_extrude();
	void put_extrude(std::wstring col);

	void addextrude(std::wstring name, std::wstring h, std::wstring col, std::wstring con);
	void addfill(std::wstring name, std::wstring col, std::wstring con);

protected:
private:
	std::wstring _stockColor;
	std::wstring _fillColor;
	std::wstring _extrude;
};

#endif