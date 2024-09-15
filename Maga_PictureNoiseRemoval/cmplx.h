#include <math.h>
#include <stdio.h>
#include<vector>

#define PI 3.141592653589793

void msg(char *err);

class cmplx{
public:
  double re;
  double im;

  cmplx() { re=im=0.; }
  cmplx(double x, double y) { re=x; im=y; }
  cmplx& operator = (cmplx&);
  friend cmplx operator * (cmplx& x, cmplx& y);
  friend cmplx operator / (cmplx& x, cmplx& y);
  friend cmplx operator / (cmplx& x, double y);
  friend cmplx operator + (cmplx& x, cmplx& y);
  friend cmplx operator - (cmplx& x, cmplx& y);
  double Abs();
};

// комплексное сопряжение
cmplx conjg(cmplx c);
cmplx conjg(double re, double im);

// сложение комплексных матриц C = A + B
void ADD( long m, long n, cmplx** A, cmplx** B, cmplx** C);
// переносит содержимое матрицы A в B
void ASSIGN(long m, long n, cmplx** A, cmplx** B);

// возвращает идентифицированную матрицу A[m][m]
int IDENT(long m, long n, cmplx** A);

// Быстрое фурье преобразование is= -1 -prjamoe  +1 -obratnoe, спектр записывается в вектор сигнала
void fourea(long v_size, std::vector<cmplx>& F, double is);

// Двумерное фурье преобразование is= -1 -prjamoe  +1 -obratnoe, спектр записывается в вектор сигнала
// возвращает 0 при отсутствии ошибок
int FFT2(long m, long n, cmplx** F, long is);


/* Решение эрмитовой системы линейных уравнений
	M - порядок матрицы A (число линейных уравнений)
	B - ветор правой части уравнения - замещается решением
	возвращаемые значения: 
		0 - нормальный выход; 
		-1 - матрица вырождена; 
		+k - утеряна численная значимость илиобнаружена неположительно определенная матрица на шаге k
*/
int cholesky(long m, cmplx* A, cmplx* B);

// комплексное SVD разложение матрицы
// возвращает 0 при отсутствии ошибок
int CSVD( long size_m, long size_n, double** u, double** v, double* sigma);
