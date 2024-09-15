#include "pch.h"
#include "MainJob.h"
using namespace std;

#define pow2(x) (x) * (x)

void MainJob::LoadPicture()
{
	ImageConverter conv;
	conv.SetImagePath(targetpath);
	conv.Convert();
	data = conv.GetData();
}

void MainJob::LoadGauss()
{
	GaussPreloader gauss;
	gauss.SetPath(targetpath);
	gauss.Load();
	data = gauss.GetData();
}

void MainJob::StretchImage()
{
	RangeStretcher rstr;
	rstr.ConvertRange(data);
	data = rstr.GetStretched();
	OriginalSourceSize = rstr.GetOriginalSize();
}

double MainJob::CalcEnergy()
{
	double res = 0;
	foureier four;
	four.DoFourea(data);
	vector<vector<cmplx>>cbuf = four.GetData();
	cbuf[0][0] = cmplx(0, 0);
	Freq = four.GetDataAbs();
	Freq[0][0] = 0;
	for (auto& row : cbuf)
	{
		for (auto& f : row)
		{
			res += f.re * f.re + f.im * f.im;
		}
	}
	return res;
}

std::vector<std::vector<double>> MainJob::CreateNoiseBase()
{
	srand(time(NULL));

	vector<vector<double>> res;
	res.resize(data.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = 0;
			for (int k = 0; k < 12; k++)
			{
				res[i][j] = -1 + 2. * (double)rand() / double(RAND_MAX);
			}
		}
	}
	return res;
}

double MainJob::CalcNoiseEnergy(std::vector<std::vector<double>>& source)
{
	double res = 0;
	for (auto& row : source)
	{
		for (auto& item : row)
		{
			res += item * item;
		}
	}
	return res;
}

void MainJob::NoiseData()
{
	auto NoiseBase = CreateNoiseBase();
	double Es = CalcEnergy();
	double En = CalcNoiseEnergy(NoiseBase);
	double betta = sqrt(NoiseLevel * Es / En);

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			data[i][j] += NoiseBase[i][j] * betta / 12.;
		}
	}
}

void MainJob::CalcNoisedFreq()
{
	foureier f;
	f.DoFourea(data);
	NoisedFreq = f.GetDataAbs();
	cNoisedFreq = f.GetData();
	NoisedFreq[0][0] = 0;
	zeroFreq = cNoisedFreq[0][0];
	cNoisedFreq[0][0] = cmplx(0, 0);
	SwapQuadrants(NoisedFreq);
	SwapQuadrants(cNoisedFreq);
}

void MainJob::RemoveNoise()
{
	double E = CalcEnergy(cNoisedFreq) * RestoreEnegryLevel;

	double r = 1;
	int x0 = cNoisedFreq[0].size() / 2;
	int y0 = cNoisedFreq.size() / 2;
	double actE = 0;
	int mrange = max(x0, y0);
	while (r < mrange)
	{
		actE = 0;
		for (int i = 0; i < cNoisedFreq.size(); i++)
		{
			for (int j = 0; j < cNoisedFreq[i].size(); j++)
			{
				if (pow2(i - y0) + pow2(j - x0) < pow2(r))
				{
					actE += pow2(cNoisedFreq[i][j].re) + pow2(cNoisedFreq[i][j].im);
				}
			}
		}
		if (actE > E)break;
		r++;
	}

	for (int i = 0; i < cNoisedFreq.size(); i++)
	{
		for (int j = 0; j < cNoisedFreq[i].size(); j++)
		{
			if (pow2(i - y0) + pow2(j - x0) > pow2(r))
			{
				cNoisedFreq[i][j] = cmplx(0, 0);
			}
		}
	}
	RestoredFreq = GetAbs(cNoisedFreq);

	SwapQuadrants(cNoisedFreq);
	cNoisedFreq[0][0] = zeroFreq;
	foureier f;
	f.DoInversedFourea(cNoisedFreq);
	RestoredData = f.GetDataReal();
}

double MainJob::CalcEnergy(std::vector<std::vector<cmplx>>& source)
{
	double res = 0;
	for (int i = 0; i < source.size(); i++)
	{
		for (int j = 0; j < source[i].size(); j++)
		{
			res += source[i][j].im * source[i][j].im + source[i][j].re * source[i][j].re;
		}
	}
	return res;
}

std::vector<std::vector<double>> MainJob::GetReal(std::vector<std::vector<cmplx>>& source)
{
	std::vector<std::vector<double>> res;
	res.resize(source.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(source[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = source[i][j].re;
		}
	}
	return res;
}

std::vector<std::vector<double>> MainJob::GetAbs(std::vector<std::vector<cmplx>>& source)
{
	std::vector<std::vector<double>> res;
	res.resize(source.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(source[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = sqrt(pow2(source[i][j].re) + pow2(source[i][j].im));
		}
	}
	return res;
}

void MainJob::RestoreRange()
{
	RangeStretcher rstr;
	rstr.SetTargetSize(OriginalSourceSize);
	rstr.ConvertRange(RestoredData);
	RestoredData = rstr.GetStretched();
}

template<typename T>
inline void MainJob::SwapQuadrants(std::vector<std::vector<T>>& source)
{
	if (source.empty())return;

	int xHalf = source[0].size() / 2;
	int yHalf = source.size() / 2;
	T buff;
	for (int i = 0; i < yHalf; i++)
	{
		for (int j = 0; j < xHalf; j++)
		{
			buff = source[i][j];
			source[i][j] = source[i + yHalf][j + xHalf];
			source[i + yHalf][j + xHalf] = buff;

			buff = source[i + yHalf][j];
			source[i + yHalf][j] = source[i][j + xHalf];
			source[i][j + xHalf] = buff;
		}
	}
}

void MainJob::SetPath(CString& path, bool ispicture)
{
	if (path.IsEmpty())return;
	targetpath = path;
	IsPicture = ispicture;
	pathinit = false;
}

void MainJob::main()
{
	if (IsPicture)
	{
		LoadPicture();
	}
	else
	{
		LoadGauss();
	}
	Puredata = data;
	StretchImage();
	NoiseData();
	CalcNoisedFreq();
	RemoveNoise();
	RestoreRange();
}

std::vector<std::vector<double>> MainJob::GetData()
{
	return data;
}

std::vector<std::vector<double>> MainJob::GetPureData()
{
	return Puredata;
}

std::vector<std::vector<double>> MainJob::GetRestoredData()
{
	return RestoredData;
}

std::vector<std::vector<double>> MainJob::GetFreq(bool zero)
{
	if (Freq.empty())return Freq;
	auto temp = Freq;
	if (zero)temp[0][0] = 0;
	return temp;
}

std::vector<std::vector<double>> MainJob::GetRestoredFreq(bool zero)
{
	if (RestoredFreq.empty())return RestoredFreq;
	auto temp = RestoredFreq;
	if (zero)temp[0][0] = 0;
	return temp;
}

std::vector<std::vector<double>> MainJob::GetNoisedFreq(bool zero)
{
	if (NoisedFreq.empty())return NoisedFreq;
	auto temp = NoisedFreq;
	if (zero)temp[0][0] = 0;
	return temp;
}

void MainJob::SetNoiseLevel(double NL)
{
	NoiseLevel = NL;
}

void MainJob::SetRestoreEnergyLevel(double val)
{
	RestoreEnegryLevel = val;
}
