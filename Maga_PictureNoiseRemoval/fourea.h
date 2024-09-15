#pragma once

#include"cmplx.h"

class foureier
{
private:
	std::vector<std::vector<cmplx>> data;
protected:
	std::vector<cmplx> Transform(std::vector<double>& input);
	template<typename T>
	std::vector<T> GetColumn(std::vector<std::vector<T>>& input, int id);
	template<typename T>
	void EmplaceColumn(std::vector<T>& source, std::vector<std::vector<T>>& target, int id);
	std::vector<double>GetReal(std::vector<cmplx>& source);
public:
	void DoFourea(std::vector<std::vector<double>>& input);
	void DoInversedFourea(std::vector<std::vector<cmplx>>& input);
	std::vector<std::vector<cmplx>>GetData();
	std::vector<std::vector<double>>GetDataReal();
	std::vector<std::vector<double>>GetDataAbs();
};

