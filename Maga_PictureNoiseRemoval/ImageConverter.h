#pragma once

#include<gdiplus.h>
#include<vector>
class ImageConverter
{
private:
	ULONG_PTR token;

	CString ImagePath;

	std::vector<std::vector<double>> data;

	bool Converted = false;
protected:
	void LoadImageFromFile();
	double Convert(Gdiplus::Color& color);
public:
	ImageConverter();
	~ImageConverter();

	void SetImagePath(CString path);
	void Convert();
	std::vector<std::vector<double>> GetData();

};

