/**
	@brief LabelNode Provider
	@author Hao Luo
	@date 2015/10/22
*/

#ifndef OSGEARTHX_CORE_LOCATION_PROVIDER_H
#define OSGEARTHX_CORE_LOCATION_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>
#include <OSGEarth\GeoTransform>
#include <osg/Material>
#include <osgFX/Effect>

namespace osgEarthXCore
{

	class OSGEARTHX_CORE_EXPORT LocationProvider : 
		public osgEarth::GeoTransform
	{
	public:
		LocationProvider();

		void setModelPath( const std::string& str );
		void setModelXML(const std::string& str);
		void makeRotate(double ax,double ay,double az);
		void showNode(const std::string& name, bool visible);
		void showNode(bool visible);
		void setEffect(unsigned int uiEffect);
		void setColor(osg::Vec4f col);
		void removeColor();
		void setColor(const std::string& name, osg::Vec4f col);
		void removeColor(const std::string& name);
		void setColor(std::vector<const std::string> &name, osg::Vec4f col);
		void removeColor(std::vector<const std::string> &name);
		void getCenter(const std::string& name, double& lat, double& lon, double& height);
		void showChilds(std::vector<const std::string> &name, bool vis);
	protected:
		virtual ~LocationProvider();
		void OpenModelFile(const std::string & modelPath);
		osg::Node* findAnnotation(std::string strID);
		void setColor(osg::Node* nd, osg::Vec4f& col);
		void removeColor(osg::Node* nd);
		void OpenModelXML(const std::string & strXml);
	private:
		osg::ref_ptr<osg::MatrixTransform> _mt;
		osg::ref_ptr<osgFX::Effect> _effect;
		std::map<std::string, osg::Vec3d> _modelMap;
		std::map<std::string, osg::Vec3d> _rotMap;
		std::map<std::string, std::string> _nameMap;
		std::map<std::string, double> _rangeMap;
	};
}

#endif