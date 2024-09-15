#include "pch.h"
#include "ImageConverter.h"
using namespace Gdiplus;

void ImageConverter::LoadImageFromFile()
{
	Bitmap bmp(ImagePath);

	int xSize = bmp.GetWidth();
	int ySize = bmp.GetHeight();

	data.clear();

	data.resize(ySize);
	for (int i = 0; i < ySize; i++)data[i].resize(xSize);

	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			Color c;
			bmp.GetPixel(j, ySize - i - 1, &c);
			data[i][j] = Convert(c);
		}
	}
}
double ImageConverter::Convert(Gdiplus::Color& color)
{
	return 0.299 * color.GetRed() + 0.587 * color.GetGreen() + 0.114 * color.GetBlue();
}
ImageConverter::ImageConverter()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
}

ImageConverter::~ImageConverter()
{
	GdiplusShutdown(token);
}

void ImageConverter::SetImagePath(CString path)
{
	ImagePath = path;
}

void ImageConverter::Convert()
{
	LoadImageFromFile();
	Converted = true;
}

std::vector<std::vector<double>> ImageConverter::GetData()
{
	if (!Converted)	return std::vector<std::vector<double>>();
	return data;
}
