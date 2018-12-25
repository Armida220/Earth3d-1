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
