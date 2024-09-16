#pragma once
#include"GaussPreloader.h"
#include"ImageConverter.h"
#include"RangeStretcher.h"
#include"fourea.h"
class MainJob
{
private:
	CString targetpath;
	bool IsPicture = false;
	bool pathinit = false;

	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> Puredata;
	std::vector<std::vector<double>> StretchedData;
	std::vector<std::vector<double>> Freq;
	std::vector<std::vector<double>> NoisedFreq;
	std::vector<std::vector<double>> RestoredData;
	std::vector<std::vector<double>> RestoredFreq;
	std::vector<std::vector<cmplx>> cNoisedFreq;
	std::pair<int, int> OriginalSourceSize;
	double NoiseLevel = 0;
	double RestoreEnegryLevel = 0;
	cmplx zeroFreq;
	double EstimateSourceNoise = 0;
	double EstimateSourcePurified = 0;
protected:
	void LoadPicture();
	void LoadGauss();
	void StretchImage();
	double CalcEnergy();
	std::vector<std::vector<double>>CreateNoiseBase();
	double CalcNoiseEnergy(std::vector<std::vector<double>>& source);
	double CalcEnergy(std::vector<std::vector<double>>& source);
	void NoiseData();
	template<typename T> void SwapQuadrants(std::vector<std::vector<T>>& source);
	void CalcNoisedFreq();
	void RemoveNoise();
	double CalcEnergy(std::vector<std::vector<cmplx>>& source);
	std::vector<std::vector<double>>GetReal(std::vector<std::vector<cmplx>>& source);
	std::vector<std::vector<double>>GetAbs(std::vector<std::vector<cmplx>>& source);
	void RestoreRange();
	void Estimate();
public:
	void SetPath(CString& path, bool ispicture);
	void main();
	std::vector<std::vector<double>> GetData();
	std::vector<std::vector<double>> GetPureData();
	std::vector<std::vector<double>> GetRestoredData();
	std::vector<std::vector<double>> GetFreq(bool zero = true);
	std::vector<std::vector<double>> GetRestoredFreq(bool zero = true);
	std::vector<std::vector<double>> GetNoisedFreq(bool zero = true);
	void SetNoiseLevel(double NL);
	void SetRestoreEnergyLevel(double val);
	double GetMistake();
	double GetDifferance();
};

