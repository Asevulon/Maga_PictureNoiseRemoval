#pragma once


// Drawer
#include<vector>
#include<algorithm>
using namespace std;
using namespace Gdiplus;


struct  m_Color
{
	double r = 0; double g = 0; double b = 0;
	m_Color() {}
	m_Color(double r, double g, double b) :r(r), g(g), b(b) {}
	Gdiplus::Color AsColor() { return Gdiplus::Color(255, r, g, b); }
	m_Color& operator+ (const m_Color& right) { r += right.r; g += right.g; b += right.b; r /= 2; g /= 2; b /= 2; return *this; }
	void ToOpenGlRange() { r /= 255; g /= 255; b /= 255; }
	inline m_Color Avg(m_Color& c1, m_Color& c2, m_Color& c3) { r += c1.r + c2.r + c3.r; r /= 4; g += c1.g + c2.g + c3.g; g /= 4; b += c1.b + c2.b + c3.b; b /= 4; return *this; }
	m_Color ToBlack() { double c = 0.299 * r + 0.587 * g + 0.114 * b; return m_Color(c, c, c); }
};


class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
private:
	vector<vector<double>>data;
	double lPadding = 12;
	double rPadding = 5;
	double tPadding = 5;
	double bPadding = 15;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;
	double zmax;
	double zmin;
	double outzmax;
	double outzmin;
	ULONG_PTR token;
	
	CString title;

	
	std::vector<m_Color>Colors;
	vector<vector<m_Color>>Colordata;

	bool m_ShowPicture = false;
	bool ToBlack = true;
	CString m_PicturePath;
	bool logarithmic = false;
	double logconstant = 0;
public:
	Drawer();
	virtual ~Drawer();

protected:
	
	double CalcStringLen(HDC hDC, CString string);
	void Drawer::Normalize();
	inline m_Color Drawer::GetColor(double val);
	inline Color Drawer::GetQuadreColor(int i, int j);
	inline void MakeColorData();

	void OnPicture(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnGraph(LPDRAWITEMSTRUCT lpDrawItemStruct);
	inline double logTransform(double val);
	void DataToLog();
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void Drawer::SetData(std::vector<std::vector<double>>& in);

	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);
	void SetPicturePath(CString path);
	void ShowPicture(bool flag);
	void SetLogarithmic(bool flag);
};


template <typename T> inline int sgn(T val);