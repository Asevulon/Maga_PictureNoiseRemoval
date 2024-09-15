#include "pch.h"
#include "RangeStretcher.h"


void RangeStretcher::ConvertRange(std::vector<std::vector<double>>& input, bool AutoResize)
{
	if (input.empty())return;
	if (input[0].empty())return;

	int width = input[0].size();
	int height = input.size();

	OriginalSize = std::pair<int, int>(width, height);

	int nwidth = 0;
	int nheight = 0;

	if (AutoResize || (!StretchedSizeInit))
	{
		double lwidth = log2(width);
		double lheight = log2(height);

		nwidth = ((lwidth - (int)lwidth) == 0) ? lwidth : lwidth + 1;
		nheight = ((lheight - (int)lheight) == 0) ? lheight : lheight + 1;

		nwidth = pow(2, nwidth);
		nheight = pow(2, nheight);

		StretchedSize = std::pair<int, int>(nwidth, nheight);

		StretchedSizeInit = true;
	}
	else
	{
		nwidth = StretchedSize.first;
		nheight = StretchedSize.second;
	}

	data.resize(nheight);
	for (int i = 0; i < nheight; i++)data[i].resize(nwidth);

	double dx = double(width) / nwidth;
	double dy = double(height) / nheight;
	double x = 0;
	double y = 0;
	int idx = 0;
	int idy = 0;

	double f1 = 0;
	double f2 = 0;
	double f3 = 0;
	double f4 = 0;
	for (int i = 0; i < nheight - 1; i++)
	{
		y = i * dy;
		idy = y;
		for (int j = 0; j < nwidth - 1; j++)
		{
			x = j * dx;
			idx = x;

			f1 = input[idy][idx] * (idx + 1 - x) * (idy + 1 - y);
			f2 = input[idy][idx + 1] * (x - idx) * (idy + 1 - y);
			f3 = input[idy + 1][idx] * (idx + 1 - x) * (y - idy);
			f4 = input[idy + 1][idx + 1] * (x - idx) * (y - idy);
			
			data[i][j] = f1 + f2 + f3 + f4;
		}
	}

	idy = height - 1;
	for (int j = 0; j < nwidth - 1; j++)
	{
		x = j * dx;
		idx = x;

		f1 = input[idy][idx] * (idx + 1 - x) * (idy + 1 - y);
		f2 = input[idy][idx + 1] * (x - idx) * (idy + 1 - y);
		f3 = input[idy][idx] * (idx + 1 - x) * (y - idy);
		f4 = input[idy][idx + 1] * (x - idx) * (y - idy);

		data[nheight - 1][j] = f1 + f2 + f3 + f4;
	}

	idx = width - 1;
	for (int i = 0; i < nheight - 1; i++)
	{
		y = i * dy;
		idy = y;

		f1 = input[idy][idx] * (idx + 1 - x) * (idy + 1 - y);
		f2 = input[idy][idx] * (x - idx) * (idy + 1 - y);
		f3 = input[idy + 1][idx] * (idx + 1 - x) * (y - idy);
		f4 = input[idy + 1][idx] * (x - idx) * (y - idy);

		data[i][nwidth - 1] = f1 + f2 + f3 + f4;
	}

	data[nheight - 1][nwidth - 1] = input[height - 1][width - 1];


	Stretched = true;
}

void RangeStretcher::SetTargetSize(std::pair<int, int>& size)
{
	StretchedSize = size;
	StretchedSizeInit = true;
}

std::pair<int, int> RangeStretcher::GetOriginalSize()
{
	return OriginalSize;
}

std::pair<int, int> RangeStretcher::GetStretchedSize()
{
	return StretchedSize;
}

std::vector<std::vector<double>> RangeStretcher::GetStretched()
{
	if (!Stretched)	return std::vector<std::vector<double>>();
	return data;
}
