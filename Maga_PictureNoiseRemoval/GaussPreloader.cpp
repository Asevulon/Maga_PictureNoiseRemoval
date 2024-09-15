#include "pch.h"
#include "GaussPreloader.h"

#define pow2(x) (x) * (x)


void GaussPreloader::LoadFromFile()
{
	std::ifstream file(SourcePath);
	file >> width >> height;
	file >> num;

	for (int i = 0; i < num; i++)
	{
		double tA = 0;
		double tx0 = 0;
		double tSx = 0;
		double ty0 = 0;
		double tSy = 0;

		file >> tA >> tx0 >> tSx >> ty0 >> tSy;
		
		A.push_back(tA);
		x0.push_back(tx0);
		Sx.push_back(tSx);
		y0.push_back(ty0);
		Sy.push_back(tSy);
	}
}

void GaussPreloader::CreateData()
{
	data.resize(height);
	for (int i = 0; i < height; i++)data[i].resize(width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			data[i][j] = 0;
			for (int k = 0; k < num; k++)
			{
				data[i][j] += A[k] * exp(-pow2(j - x0[k]) / Sx[k] - pow2(i - y0[k]) / Sy[k]);
			}
		}
	}
}

void GaussPreloader::SetPath(CString path)
{
	SourcePath = path;
}

void GaussPreloader::Load()
{
	LoadFromFile();
	CreateData();
	Loaded = true;
}

std::vector<std::vector<double>> GaussPreloader::GetData()
{
	if (!Loaded)return std::vector<std::vector<double>>();
	return data;
}
