#pragma once

#include <osg/ValueObject>
#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <iostream>
#include <sstream>

class PhotoCallback : public osg::Camera::DrawCallback
{
public:
	PhotoCallback(osg::Image* img)
		: _image(img), _filename("1"){}

	virtual void operator()(osg::RenderInfo& renderInfo) const
	{
		bool capturing = false;
		if (_image.valid() && _image->getUserValue("Capture", capturing))
		{
			osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
			if (capturing && gc->getTraits())
			{
				int width = gc->getTraits()->width;
				int height = gc->getTraits()->height;
				GLenum pixelFormat = (gc->getTraits()->alpha ? GL_RGBA : GL_RGB);
				_image->readPixels(0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE);

				osgDB::writeImageFile(*_image, _filename);
			}
			_image->setUserValue("Capture", false);
		}
	}

	void setFileName(std::string file) {
		_filename = file;
	}
protected:
	osg::ref_ptr<osg::Image> _image;
	std::string _filename;
};
