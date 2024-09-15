#pragma once
#include<vector>
class RangeStretcher
{
private:
	std::vector<std::vector<double>> data;

	std::pair<int, int>OriginalSize;
	std::pair<int, int>StretchedSize;
	bool Stretched;
	bool StretchedSizeInit = false;
protected:
public:
	void ConvertRange(std::vector<std::vector<double>>& input, bool AutoResize = false);
	void SetTargetSize(std::pair<int,int>&size);
	std::pair<int, int> GetOriginalSize();
	std::pair<int, int> GetStretchedSize();
	std::vector<std::vector<double>> GetStretched();

};

