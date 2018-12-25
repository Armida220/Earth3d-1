#include <windows.h>
#include <cmath>
#include <string.h>
#include <png.h>
#include <iostream>
using namespace std;

#include "schemes.h"

#define pixelLonLat 0.003
typedef struct point{
	double x;
	double y;
}point;

double maxLon=-180.0, minLon=180.0, maxLat=-90.0, minLat=90.0;
point leftDown={0}, rightUp={0};
int WIDTH=0, HEIGHT=0, DOTSIZE=0, OPACITY=0;


//////////////////////////���㾭γ�ȷ�Χ////////////////////////////////////
void computeMinMax(vector<double> LonLat)
{
	for(int i=0; i<LonLat.size(); i+=2){
		double lon = LonLat[i];
		double lat = LonLat[i+1];

		if(maxLon < lon)
			maxLon = lon;
		if(minLon > lon)
			minLon = lon;
		if(maxLat < lat)
			maxLat = lat;
		if(minLat > lat)
			minLat = lat;
	}
}

//////////////////////////����png��С////////////////////////////////////////
bool computePngSize(vector<double> LonLat)
{
	bool judges = true;
	 
	if(LonLat.size() == 0)
	{
		judges = false;
		return judges;
	}
	else
	{
		///////////////////���㾭γ�ȷ�Χ//////////////////////////
		computeMinMax(LonLat);
		double lonlatX = maxLon - minLon;
		double lonlatY = maxLat - minLat;

		WIDTH = lonlatX / pixelLonLat;
		HEIGHT = lonlatY / pixelLonLat;

		DOTSIZE = 20;
	}
	WIDTH = WIDTH + DOTSIZE;     //ͼƬ�Ŀ��
	HEIGHT = HEIGHT + DOTSIZE;   //ͼƬ�ĸ߶�

	return judges;
}
////////////////////////////����γ��ת��Ϊ��ά������/////////////////////////////////////
void exchangePoints(vector<int>* points, vector<double> LonLat)
{
	double lonWidth = maxLon - minLon;
	double latHeight = maxLat - minLat;
	
	if(lonWidth != 0 && latHeight != 0)
	{
		leftDown.x = minLon - pixelLonLat*DOTSIZE/2;
		leftDown.y = minLat - pixelLonLat*DOTSIZE/2;

		rightUp.x = maxLon + pixelLonLat*DOTSIZE/2;
		rightUp.y = maxLat + pixelLonLat*DOTSIZE/2;

		for(int i=0; i<LonLat.size(); i+=2)
		{
			int x=0, y=0;
			x = (int)((LonLat[i] - leftDown.x)/pixelLonLat);
			y = (int)((LonLat[i+1] - leftDown.y)/pixelLonLat);
			points->push_back(x);
			points->push_back(y);
		}
	}
	else if(lonWidth == 0 && latHeight ==0)
	{
		leftDown.x = minLon - pixelLonLat*WIDTH/2;
		leftDown.y = minLat - pixelLonLat*HEIGHT/2;

		rightUp.x = maxLon + pixelLonLat*WIDTH/2;
		rightUp.y = maxLat + pixelLonLat*HEIGHT/2;

		int x=0, y=0;
		x = (int)((LonLat[0] - leftDown.x)/pixelLonLat);
		y = (int)((LonLat[1] - leftDown.y)/pixelLonLat);
		points->push_back(x);
		points->push_back(y);
	}

	else if(lonWidth == 0 && latHeight !=0)
	{	
		leftDown.x = minLon - pixelLonLat*WIDTH/2;
		leftDown.y = minLat - pixelLonLat*DOTSIZE/2;

		rightUp.x = maxLon + pixelLonLat*WIDTH/2;
		rightUp.y = maxLat + pixelLonLat*DOTSIZE/2;

		for(int i=0; i<LonLat.size(); i+=2)
		{
			int x=0, y=0;
			x = (int)((LonLat[i] - leftDown.x)/pixelLonLat);
			y = (int)((LonLat[i+1] - leftDown.y)/pixelLonLat);
			points->push_back(x);
			points->push_back(y);
		}
	}
	else if(lonWidth != 0 && latHeight == 0)
	{
		leftDown.x = minLon - pixelLonLat*DOTSIZE/2;
		leftDown.y = minLat - pixelLonLat*HEIGHT/2;

		rightUp.x = maxLon + pixelLonLat*DOTSIZE/2;
		rightUp.y = maxLat + pixelLonLat*HEIGHT/2;

		for(int i=0; i<LonLat.size(); i+=2)
		{
			int x=0, y=0;
			x = (int)((LonLat[i] - leftDown.x)/pixelLonLat);
			y = (int)((LonLat[i+1] - leftDown.y)/pixelLonLat);
			points->push_back(x);
			points->push_back(y);
		}
	}
}

////////////////////////////����ÿ�����ص���ܶ�/////////////////////////////////////
void calcDensity(vector<int>* points, vector<int>* pixels)
{
	float midpt = DOTSIZE / 2.f;
	double radius = sqrt(midpt*midpt + midpt*midpt) / 2.f;
    double dist = 0.0;
    int pixVal = 0;
    int j = 0;
    int k = 0;
    int i = 0;
    int ndx = 0;
    point pt = {0}; 

	// initialize image data to white
    for(i = 0; i < WIDTH*HEIGHT; i++) 
    {
        *(pixels->begin()+i) = 255;
    }

	for(i = 0; i < points->size(); i=i+2)
    {
        pt.x = *(points->begin()+i);
        pt.y = *(points->begin()+i+1);

        for (j = (int)pt.x - midpt; j < (int)pt.x + midpt; j++)
        {   
            for (k = (int)(pt.y - midpt); k < (int)(pt.y + midpt); k++)
            {
                if (j < 0 || k < 0 || j >= WIDTH || k >= HEIGHT) continue; 

                dist = sqrt( (j-pt.x)*(j-pt.x) + (k-pt.y)*(k-pt.y) );

                pixVal = (int)(200*(dist/radius) + 55);
				if (pixVal > 255) 	continue;

                ndx = k*WIDTH + j;
                if(ndx >= (int)WIDTH*HEIGHT) continue;   // ndx can be greater than array bounds

				if(*(pixels->begin()+ndx) > pixVal)
				{
					double ratio = 0.2  - *(pixels->begin()+ndx)/(double)1275;
					*(pixels->begin()+ndx) = (int)(pixVal - pixVal * ratio);
				}
				else
				{
					double ratio = 0.2 - (double)(pixVal)/(double)1275;
					*(pixels->begin()+ndx) = (int)(*(pixels->begin()+ndx) - *(pixels->begin()+ndx) * ratio);
				}
            } // for k
        } //for j
    } // for i

}

////////////////////////////����ÿ�����ص�RGBA/////////////////////////////////////
void colorize(vector<int> pixels_bw, vector<unsigned char>* pixels_color)
{
	int i = 0;
    int pix = 0;
    int alpha = 0;

    for(i = 0; i < (int)WIDTH*HEIGHT; i++)
    {
        pix = pixels_bw[i];

        if (pix < 255)
			alpha = OPACITY;
        else
			continue;

		if(alpha < 0)
			alpha = 0;

		*(pixels_color->begin()+i*4) = schemes[pix][0];
		*(pixels_color->begin()+i*4+1) = schemes[pix][1];
		*(pixels_color->begin()+i*4+2) = schemes[pix][2];
		*(pixels_color->begin()+i*4+3) = alpha;
    } 
}


///////////////////////////����pngͼƬ/////////////////////////////////////
int savePng(const char* pstrFileName, vector<unsigned char> pix_color)
{
	
   FILE *fp = NULL;  
   png_structp png_ptr;  
   png_infop info_ptr;  
   png_colorp palette;  
  
   /* ����Ҫд����ļ� */  
   fp = fopen(pstrFileName, "wb");  
   if (fp == NULL)  
      return (ERROR);  
  
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
  
   if (png_ptr == NULL)  
   {  
      fclose(fp);  
      return (ERROR);  
   }  
  
   /* �����ڴ沢��ʼ��ͼ����Ϣ���ݡ�����Ҫ��*/  
   info_ptr = png_create_info_struct(png_ptr);  
   if (info_ptr == NULL)  
   {  
      fclose(fp);  
      png_destroy_write_struct(&png_ptr,  NULL);  
      return (ERROR);  
   }  
  
   /* ���ô�����������ڵ��� png_create_write_struct() ʱû 
    * �����ô�����������ô��δ����Ǳ���д�ġ�*/  
   if (setjmp(png_jmpbuf(png_ptr)))  
   {  
      /* ��������ܵ������ˣ���ôд���ļ�ʱ���������� */  
      fclose(fp);  
      png_destroy_write_struct(&png_ptr, &info_ptr);  
      return (ERROR);  
   }  
  
   /* ����������ƣ������ʹ�õ��� C �ı�׼ I/O �� */  
   png_init_io(png_ptr, fp);  
   
   int width = WIDTH, height = HEIGHT;
   int bit_depth = 8;
   png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,  
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);  
  
   /* д���ļ�ͷ����Ϣ�����裩 */  
   png_write_info(png_ptr, info_ptr);  

   vector<png_bytep> row_pointers;
   row_pointers.resize(height);
  
   if (height > PNG_UINT_32_MAX/(sizeof (png_bytep)))  
     png_error (png_ptr, "Image is too tall to process in memory");  
  
   for (int k=0; k <height; k++)  
     row_pointers[height-k-1] = &pix_color[0] + k*width*4;  
 
   png_write_image(png_ptr, &row_pointers[0]);  
   png_write_end(png_ptr, info_ptr);  
   png_destroy_write_struct(&png_ptr, &info_ptr);  

   row_pointers.clear();
   /* �ر��ļ� */  
   fclose(fp);  
  
   return 1;  

}


///////////////////////////���ݲ�����������/////////////////////////////////////
bool heatMapImp(const char* pstrFileName, int opacity, vector<double> LonLat)
{
	OPACITY = opacity;     //��������png��͸����

	////////////////////////����pngSize////////////////////////
	bool judge = computePngSize(LonLat);
	if(!judge)
		return false;


	/////����γ��ת��Ϊ��λ������///////////
	vector<int> points;
	exchangePoints(&points, LonLat);
	

	/////////////�����ά��������ܶȷֲ�/////////////////
	vector< int> pixels_bw;
	pixels_bw.resize(WIDTH*HEIGHT);
	calcDensity(&points, &pixels_bw);

	/////////////���������������ɫֵ�Լ�alphaֵ//////////////////
	vector<unsigned char> pix_color;
	pix_color.resize(WIDTH*HEIGHT*4);
	colorize(pixels_bw, &pix_color);

	////////////////����pngͼƬ/////////////////////////////////
	int judges = savePng(pstrFileName, pix_color);
	if(judges == 0)
		judges = false;
	else
		judges = true;


	pix_color.clear();
	pixels_bw.clear();
	points.clear();

	return judges;
}