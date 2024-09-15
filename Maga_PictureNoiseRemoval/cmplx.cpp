#include"pch.h"
#include "cmplx.h"


void msg(char *err)
{
	FILE* fp;
	fopen_s(&fp, "report.msg", "at");
	fprintf(fp, "%s\n\n", err);
	fclose(fp);
}


//----------------------------------------------------------------
cmplx operator * (cmplx& x, cmplx& y)
{
	cmplx z;
	z.re = x.re * y.re - x.im * y.im;
	z.im = x.re * y.im + y.re * x.im;
	return z;
}

//----------------------------------------------------------------
cmplx operator / (cmplx& x, cmplx& y)
{
	cmplx z;
	double y2 = y.re*y.re + y.im*y.im;
	if( y2 < 10e-40 )  return z;
	z.re = (x.re*y.re + x.im*y.im) / y2;
	z.im = (y.re*x.im - x.re*y.im) / y2;
	return z;
}

cmplx operator / (cmplx& x, double y)
{
	cmplx z;
	if( y < 10e-40 )  return z;
	z.re = x.re / y;
	z.im = x.im / y;
	return z;
}

cmplx operator + (cmplx& x, cmplx& y)
{
	cmplx z;
	z.re = x.re + y.re;
	z.im = x.im + y.im;
	return z;
}

cmplx operator - (cmplx& x, cmplx& y)
{
	cmplx z;
	z.re = x.re - y.re;
	z.im = x.im - y.im;
	return z;
}

cmplx& cmplx::operator = (cmplx &c)
{
	re = c.re;
	im = c.im;
	return *this;
}

double cmplx::Abs()
{
	return sqrt(re * re + im * im);
}

// комплексное сопряжение
cmplx conjg(cmplx c) { return cmplx(c.re, -c.im); }
cmplx conjg(double re, double im) { return cmplx(re, -im); }

// сложение комплексных матриц C = A + B
void ADD( long m, long n, cmplx** A, cmplx** B, cmplx** C)
{
	for( long i=0; i<n; i++)
		for( long j=0; j<m; j++)  
			C[j][i] = A[j][i] + B[j][i];
}

// переносит содержимое матрицы A в B
void ASSIGN(long m, long n, cmplx** A, cmplx** B)
{
	for( long j=0; j<m; j++)  for( long i=0; j<n; i++)  B[j][i] + A[j][i];
}

// возвращает идентифицированную матрицу A[m][m]
int IDENT(long m, long n, cmplx** A)
{
	if(m!=n)  return -1;

	for( long j=0; j<m; j++)
		for( long i=0; i<n; i++)
			if(i==j) A[j][i]=cmplx(1.,0.);  else A[j][i]=cmplx(0.,0.);
	return 0;
}

//-------------------------- Быстрое фурье преобразование   is=  -1 -prjamoe  +1 -obratnoe
void fourea(long v_size, std::vector<cmplx>& F, double is)
{
	cmplx  temp, w, c;
	long i, i1, j, j1, istep;
	long m, mmax;
	long n = v_size;
	double fn, r1, theta;
	fn=(double)n;
	double r = PI * is;
	j = 1;
	for( i=1; i<=n; i++)
	{
		i1 = i - 1;
		if(i<j)
		{
			j1=j-1;
			temp = F[j1];
			F[j1] = F[i1];
			F[i1] = temp;
		}
		m=n/2;
		while( j > m ){	j -= m;	m = (m+1)/2; }
		j+=m;
	}
	mmax=1;
	while(mmax<n)
	{
		istep=2*mmax;
		r1=r/(double)mmax;
		for( m=1; m<=mmax; m++)
		{
			theta = r1*(double)(m-1);
			w = cmplx(cos(theta), sin(theta));			
			for( i=m-1; i<n; i+=istep)
			{
				j=i+mmax;
				c = F[j];
				temp = w * c;
				F[j] = F[i] - temp;
				F[i] = F[i] + temp;
			}
		}
		mmax=istep;
	}
	if(is>0)  for( i=0; i<n; i++){ F[i].re /= fn;  F[i].im /= fn; }
	return;
}

//---------------------------- Двумерное фурье преобразование is= -1 -prjamoe  +1 -obratnoe
//int FFT2(long m, long n, cmplx** F, long is)
//{
//	//if( m!=n )  return -1;// только для квадратных матриц
//	long i,j,k=m;
//	if(n>k)  k=n;
//	cmplx* v = new cmplx[k];
//	for( j=0; j<m; j++)
//	{
//		for( i=0; i<n; i++)  v[i] = F[j][i];
//		fourea( n, v, is);
//		for( i=0; i<n; i++)  F[j][i] = v[i];
//	}
//	for( i=0; i<n; i++)
//	{
//		for( j=0; j<m; j++)  v[j] = F[j][i];
//		fourea( m, v, is);
//		for( j=0; j<m; j++)  F[j][i] = v[j];
//	}
//	delete[] v;
//	return 0;
//}



// Решение эрмитовой системы линейных уравнений
int cholesky(long m, cmplx* A, cmplx* B)
{
	int istat=0;
	long i, j, k, l;
	long kpiv, ind, lend, lpiv;

	double eps = 1e-15;
	double tiny;

	cmplx sum, ctmp, dpiv;


	kpiv=0;
	for( k=0; k<m; k++)	// ->100
	{
		kpiv += k;
		ind = kpiv;
		lend = k-1; //???????????????
		tiny = fabs(eps * A[kpiv].re);

		for( i=k; i<m; i++)
		{
			sum = cmplx(0.,0.);

			if(lend != -1 )
			{
				lpiv = kpiv;
				for( l=0; l<lend; l++)
				{
					lpiv = lpiv + l - k; // - 1;
					ctmp = A[lpiv]*A[ind-l]*conjg(A[kpiv-l]);
					ctmp.im = 0.;					
					sum = sum + ctmp;
				}
			}
			sum = A[ind] - sum;

			if( i!=k )  A[ind] = sum * dpiv;	//	->80
			else
			{
				if(sum.re <= tiny)	// ->90
				{
					if(sum.re <= 0)	 { return -1; }// ->70
					if(istat <= 0 )  { return k;} //istat = k;
				}
				A[kpiv] = cmplx(sum.re,0.);
				dpiv = cmplx(1.,0.) / (sum.re);
			}
			ind+=i;
		}
		ind+=i;
	} // :100

	kpiv = 0;
	for( k=1; k<m; k++)	// ->200
	{
		kpiv += k;
		sum = B[k];
		for( j=0; j<k-1; j++)  sum = sum - B[k-j] * conjg(A[kpiv-j]);
		B[k] = sum;
	}

	kpiv = (m*(m+1))/2;
	B[m-1] = B[m-1] / A[kpiv].re;
	for( k=m-1; k>=1; k--)
	{
		kpiv = kpiv-k;
		ind = kpiv;
		sum = B[k-1] / A[kpiv].re;
		for( j=k; j<m; j++)
		{
			ind = ind + (j-1);//-1; //????????
			sum = sum - B[j] * A[ind];
		}
		B[k-1] = sum;
	}

	return 0;
}



// комплексное SVD разложение матрицы
int CSVD( long size_m, long size_n, double** u, double** v, double* sigma)
{
	return 0;
}




