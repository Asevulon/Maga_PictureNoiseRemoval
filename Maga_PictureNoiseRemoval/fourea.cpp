#include "pch.h"
#include "fourea.h"

std::vector<cmplx> foureier::Transform(std::vector<double>& input)
{
	std::vector<cmplx>res;
	res.resize(input.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = cmplx(input[i], 0);
	}
	return res;
}

template<typename T>
std::vector<T> foureier::GetColumn(std::vector<std::vector<T>>& input, int id)
{
	std::vector<T>res;
	res.resize(input.size());

	for (int i = 0; i < res.size(); i++)
	{
		res[i] = input[i][id];
	}
	return res;
}

template<typename T>
void foureier::EmplaceColumn(std::vector<T>& source, std::vector<std::vector<T>>& target, int id)
{
	for (int i = 0; i < target.size(); i++)
	{
		target[i][id] = source[i];
	}
}

std::vector<double> foureier::GetReal(std::vector<cmplx>& source)
{
	std::vector<double>res;
	res.resize(source.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = source[i].re;
	}
	return res;
}

void foureier::DoFourea(std::vector<std::vector<double>>& input)
{
	if (input.empty())return;
	if (input[0].empty())return;

	data.resize(input.size());
	for (int i = 0; i < data.size(); i++)data[i].resize(input[i].size());

	int width = data[0].size();
	int height = data.size();
	for (int i = 0; i < height; i++)
	{
		std::vector<cmplx> row = Transform(input[i]);
		
		fourea(row.size(), row, -1);

		data[i] = row;
	}

	for (int i = 0; i < width; i++)
	{
		std::vector<cmplx> col = GetColumn(data, i);
		
		fourea(col.size(), col, -1);
		
		EmplaceColumn(col, data, i);
	}
}

void foureier::DoInversedFourea(std::vector<std::vector<cmplx>>& input)
{
	if (input.empty())return;
	if (input[0].empty())return;

	data.resize(input.size());
	for (int i = 0; i < data.size(); i++)data[i].resize(input[i].size());

	int width = data[0].size();
	int height = data.size();

	for (int i = 0; i < height; i++)
	{
		std::vector<cmplx>row = input[i];

		fourea(row.size(), row, 1);

		data[i] = row;
	}

	for (int i = 0; i < width; i++)
	{
		std::vector<cmplx> col = GetColumn(data, i);

		fourea(col.size(), col, 1);
	
		EmplaceColumn(col, data, i);
	}
}

std::vector<std::vector<cmplx>> foureier::GetData()
{
	return data;
}

std::vector<std::vector<double>> foureier::GetDataReal()
{
	std::vector<std::vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = GetReal(data[i]);
	}
	return res;
}

std::vector<std::vector<double>> foureier::GetDataAbs()
{
	std::vector<std::vector<double>>res;
	res.resize(data.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(data[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = data[i][j].Abs();
		}
	}
	return res;
}
