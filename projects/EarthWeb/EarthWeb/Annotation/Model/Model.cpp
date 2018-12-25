#include <EarthWeb/Annotation/Model/Model.h>
#include <EarthWeb/Basis/GeoData/GeoPoint/GeoPoint.h>
#include <atlstr.h>

// BOOL WStringToString(const std::wstring &wstr,std::string &str)
//{    
//    int nLen = (int)wstr.length();    
//    str.resize(nLen,' ');
//
//    int nResult = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wstr.c_str(),nLen,(LPSTR)str.c_str(),nLen,NULL,NULL);
//
//    if (nResult == 0)
//    {
//        return FALSE;
//    }
//
//    return TRUE;
//}

bool Model::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void Model::put_visible(bool bVisible)
{
	getOrCreateInnerObject()->setVisible(bVisible);
}

bool Model::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void Model::setModelPath( const std::wstring& strPath )
{
	//std::string str;
	//WStringToString(strPath,str);
	//getOrCreateInnerObject()->setModelPath(str);
	getOrCreateInnerObject()->setModelPath(CStringA(strPath.data()).GetString());
}

void Model::setPosition(GeoPointPtr ptrGeoPoint)
{
	if (ptrGeoPoint.get() == NULL)
	{
		return;
	}

	getOrCreateInnerObject()->setPosition(*ptrGeoPoint->getOrCreateInnerObject());
}

GeoPointPtr Model::getPosition()
{
	GeoPointPtr ptrGeoPoint = boost::make_shared<GeoPoint>();
	*(ptrGeoPoint->getOrCreateInnerObject()) = getOrCreateInnerObject()->getPosition();
	return ptrGeoPoint;
}

void Model::setScale(double x, double y, double z)
{
	osg::Vec3f scale(x, y, z);
	getOrCreateInnerObject()->setScale(scale);
}

void Model::setRotate(double x, double y, double z)
{
	osg::Quat modelRot(osg::DegreesToRadians(x), osg::Vec3d(1, 0, 0), osg::DegreesToRadians(y), osg::Vec3d(0, 1, 0), osg::DegreesToRadians(z), osg::Vec3d(0, 0, 1));
	getOrCreateInnerObject()->setLocalRotation(modelRot);
}

void Model::setName(const std::wstring & wstrText)
{
	getOrCreateInnerObject()->setName(CStringA(wstrText.data()).GetString());
}

std::wstring Model::getName()
{
	return (std::wstring) CString(getOrCreateInnerObject()->getName().data());
}

void Model::startEdit()
{
	getOrCreateInnerObject()->startEdit();
}

void Model::stopEdit()
{
	getOrCreateInnerObject()->stopEdit();
}

void Model::grow()
{
	getOrCreateInnerObject()->splitShow();
}

void Model::setPlay(bool bPlay)
{
	getOrCreateInnerObject()->setPlay(bPlay);
}

bool Model::getPlay()
{
	return getOrCreateInnerObject()->getPlay();
}

void Model::setSpeed(double dSpeed)
{
	getOrCreateInnerObject()->setSpeed(dSpeed);
}

double Model::getSpeed()
{
	return getOrCreateInnerObject()->getSpeed();
}

void Model::setPath(GeoLinePtr lin)
{
	getOrCreateInnerObject()->setPath(lin->getInnerObject());
}

void Model::setAction(int iAct)
{
	getOrCreateInnerObject()->setAction(iAct);
}

int Model::getAction()
{
	return getOrCreateInnerObject()->getAction();
}