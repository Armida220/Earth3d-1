#include <Provider/AnnotationProvider/ImageProvider.h>
using namespace osgEarthXCore;

#include <osgDB/ReadFile>

typedef	std::map<std::string, osg::ref_ptr<osg::Image> > MapImg;
static MapImg g_imgPool;

osg::Image* createRandomImage(int width, int height)
{
	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);

	int xc = width / 2;
	int yc = height / 2;
	int len = xc*xc;// +yc*yc;
	unsigned char* data = image->data();
	for (int y = 0; y<height; ++y)
	{
		for (int x = 0; x<width; ++x)
		{
			double d = (x - xc)*(x - xc) + (y - yc)*(y - yc);
			if (d > xc*xc) {
				*(data++) = 0;
				*(data++) = 0;
				*(data++) = 0;
				*(data++) = 0;
			}
			else {
				*(data++) = 0xFF;
				*(data++) = 0;
				*(data++) = 0;
				*(data++) = 0xFF * (1.0 - d / len);
			}
		}
	}
	return image.release();
}

void ImageProvider::setImageSrc( std::string strSrc )
{
	auto result = g_imgPool.find(strSrc);
	if (result == g_imgPool.end()) {
		if (strSrc == "heatmap") {
			g_imgPool[strSrc] = createRandomImage(128,128);
		}
		else {
			g_imgPool[strSrc] = osgDB::readImageFile(strSrc);
		}
	}
	_setImage(g_imgPool[strSrc].get());
	//_setImage( osgDB::readImageFile( strSrc ) );
}

void ImageProvider::setImageSrc2(std::string strSrc)
{
	auto result = g_imgPool.find(strSrc);
	if (result == g_imgPool.end()) {
		g_imgPool[strSrc] = osgDB::readImageFile(strSrc);
	}
	_setImage2(g_imgPool[strSrc].get());
	//_setImage( osgDB::readImageFile( strSrc ) );
}

void ImageProvider::setImageSrcNoCache(std::string strSrc)
{
	m_strSrc = strSrc;
	_setImage( osgDB::readImageFile( strSrc ) );
}

std::string  ImageProvider::getImageSrcNoCache()
{
	return m_strSrc;
}