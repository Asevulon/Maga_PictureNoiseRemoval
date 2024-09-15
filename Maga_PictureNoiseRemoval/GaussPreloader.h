#pragma once
#include<fstream>
#include<vector>
class GaussPreloader
{
private:
	CString SourcePath;
	std::vector<std::vector<double>> data;

	int width = 0;
	int height = 0;

	int num = 0;

	std::vector<double> A;

	std::vector<double> x0;
	std::vector<double> Sx;

	std::vector<double> y0;
	std::vector<double> Sy;

	bool Loaded = false;
protected:
	void LoadFromFile();
	void CreateData();
public:
	void SetPath(CString path);
	void Load();
	std::vector<std::vector<double>> GetData();

};

