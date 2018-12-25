#include <fstream>
#include <EarthWeb/Source/VectorSource/VectorSourceSimple/VectorSourceSimple.h>
#include <atlstr.h>
#include <osgDB/ReadFile>

void readfilestr(const std::string& filename, std::vector<std::string>& osgb)
{
	std::ifstream file(filename, std::ifstream::in);
	if (!file)
	{
		std::string error_message = "No valid input file was given.";
	}
	std::string path;
	while (getline(file, path))
	{
		if (!path.empty())
		{
			osgb.push_back(path);
		}
	}
}

void VectorSourceSimple::setLocation(double x, double y, double z)
{
	osg::Vec3d v(x, y, z);
	getOrCreateInnerObject()->location() = v;
}

void VectorSourceSimple::put_modelSource(std::wstring str)
{
	std::vector<std::string> modelVector;
	readfilestr(CStringA(str.data()).GetString(), modelVector);
	osg::ref_ptr<osg::Node> modelNode = osgDB::readNodeFiles(modelVector);
	getOrCreateInnerObject()->node() = modelNode;
}

std::wstring VectorSourceSimple::get_modelSource()
{
	return std::wstring();
}
