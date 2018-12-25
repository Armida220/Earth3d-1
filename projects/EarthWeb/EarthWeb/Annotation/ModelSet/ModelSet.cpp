#include <EarthWeb/Annotation/ModelSet/ModelSet.h>
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

bool ModelSet::get_visible()
{
	return getOrCreateInnerObject()->isVisible();
}

void ModelSet::put_visible(bool bVisible)
{
	getOrCreateInnerObject()->setVisible(bVisible);
}

bool ModelSet::get_enabled()
{
	return getOrCreateInnerObject()->isEnabled();
}

void ModelSet::setModelPath( const std::wstring& strPath )
{
	//std::string str;
	//WStringToString(strPath,str);
	//getOrCreateInnerObject()->setModelPath(str);
	getOrCreateInnerObject()->setModelPath(CStringA(strPath.data()).GetString());
}

