#include <EarthWeb/Annotation/Location/Location.h>
#include <atlstr.h>
#include <osgEarthSymbology/Color>

void Location::setName(const std::wstring& wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring Location::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

bool Location::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void Location::put_visible( bool bVisible )
{
	getOrCreateInnerObject()->setVisible( bVisible );
}

bool Location::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void Location::setPosition( GeoPointPtr ptrGeoPoint )
{
	if ( ptrGeoPoint.get() == NULL )
	{
		return;
	}

	getOrCreateInnerObject()->setPosition( * ptrGeoPoint->getOrCreateInnerObject() );
}

GeoPointPtr Location::getPosition()
{
	GeoPointPtr ptrGeoPoint = boost::make_shared<GeoPoint>();
	*( ptrGeoPoint->getOrCreateInnerObject() ) = getOrCreateInnerObject()->getPosition();
	return ptrGeoPoint;
}

void Location::setModelPath( const std::wstring& wstrText )
{
	getOrCreateInnerObject()->setModelPath( CStringA( wstrText.data() ).GetString() );
}

void Location::setModelXML(const std::wstring& wstrText)
{
	getOrCreateInnerObject()->setModelXML(CStringA(wstrText.data()).GetString());
}

void Location::setRotate(double ax, double ay, double az)
{
	getOrCreateInnerObject()->makeRotate(ax,  ay,  az);
}

void Location::showChild(const std::wstring& name, bool vis)
{
	getOrCreateInnerObject()->showNode(CStringA(name.data()).GetString(), vis);
}

void Location::showChilds(std::vector<const std::wstring> name, bool vis)
{
	std::vector<const std::string> nm;
	for (int i = 0; i < name.size(); i++)
	{
		nm.push_back(CStringA(name[i].data()).GetString());
	}
	getOrCreateInnerObject()->showChilds(nm, vis);
}

void Location::setEffect(unsigned int uiEffect)
{
	getOrCreateInnerObject()->setEffect(uiEffect);
}

void Location::setColor(const std::wstring& col)
{
	std::string strCol = CStringA(col.data()).GetString();
	getOrCreateInnerObject()->setColor(osgEarth::Symbology::Color(strCol));
}

void Location::removeColor()
{
	getOrCreateInnerObject()->removeColor();

}

void Location::setColorEx(const std::wstring& name, const std::wstring& col)
{
	std::string strCol = CStringA(col.data()).GetString();
	getOrCreateInnerObject()->setColor(CStringA(name.data()).GetString(),osgEarth::Symbology::Color(strCol));
}

void Location::removeColorEx(const std::wstring& name)
{
	getOrCreateInnerObject()->removeColor(CStringA(name.data()).GetString());
}

void Location::setColors(std::vector<const std::wstring> name, const std::wstring& col)
{
	std::vector<const std::string> nm;
	for (int i = 0; i < name.size(); i++)
	{
		nm.push_back(CStringA(name[i].data()).GetString());
	}
	std::string strCol = CStringA(col.data()).GetString();
	getOrCreateInnerObject()->setColor(nm, osgEarth::Symbology::Color(strCol));
}

void Location::removeColors(std::vector<const std::wstring> name)
{
	std::vector<const std::string> nm;
	for (int i = 0; i < name.size(); i++)
	{
		nm.push_back(CStringA(name[i].data()).GetString());
	}
	getOrCreateInnerObject()->removeColor(nm);
}

std::vector<double> Location::getChildPositon(const std::wstring& name)
{
	double lat, lon, height;
	std::vector<double> vRes;
	getOrCreateInnerObject()->getCenter(CStringA(name.data()).GetString(), lat, lon, height);
	vRes.push_back(lon);
	vRes.push_back(lat);
	vRes.push_back(height);
	return vRes;
}
