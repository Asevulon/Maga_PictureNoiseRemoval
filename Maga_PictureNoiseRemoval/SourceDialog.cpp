// SourceDialog.cpp: файл реализации
//

#include "pch.h"
#include "Maga_PictureNoiseRemoval.h"
#include "afxdialogex.h"
#include "SourceDialog.h"


// Диалоговое окно SourceDialog

IMPLEMENT_DYNAMIC(SourceDialog, CDialogEx)

SourceDialog::SourceDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOURCE_DIALOG, pParent)
{

}

SourceDialog::~SourceDialog()
{
}

void SourceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, TextsList);
	DDX_Control(pDX, IDC_LIST2, PicsList);
	DDX_Control(pDX, IDC_PICTURE, pic);
}

void SourceDialog::GetPresetsPath()
{
	wchar_t path[100];
	GetModuleFileNameW(NULL, path, 100);
	PresetsPath = path;
	int i = PresetsPath.GetLength() - 1;
	int j = 0;
	while ((j < 3) && (i > 0))
	{
		if (PresetsPath[i] == L'\\')j++;
		i--;
	}
	PresetsPath = PresetsPath.Left(i + 2);
	PresetsPath += L"Maga_PictureNoiseRemoval\\";
}

void SourceDialog::GetPicturesPresets()
{
	CFileFind finder;
	auto temp = PresetsPath + _T("pic\\*.jpg");
	BOOL bWorking = finder.FindFile(temp);
	if (bWorking)PicsList.ResetContent();
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		PicsList.AddString(finder.GetFileName());
	}

	CFileFind finder1;
	auto temp1 = PresetsPath + _T("pic\\*.png");
	bWorking = finder1.FindFile(temp1);
	while (bWorking)
	{
		bWorking = finder1.FindNextFileW();
		PicsList.AddString(finder1.GetFileName());
	}
}

void SourceDialog::GetTextPresets()
{
	CFileFind finder;
	auto temp = PresetsPath + _T("text\\*.txt");
	BOOL bWorking = finder.FindFile(temp);
	if (bWorking)TextsList.ResetContent();
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		TextsList.AddString(finder.GetFileName());
	}
}

void SourceDialog::UpdatePresetsLists()
{
	GetTextPresets();
	GetPicturesPresets();
}


BEGIN_MESSAGE_MAP(SourceDialog, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST2, &SourceDialog::OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST1, &SourceDialog::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &SourceDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &SourceDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SourceDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// Обработчики сообщений SourceDialog


BOOL SourceDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	GetPresetsPath();
	UpdatePresetsLists();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void SourceDialog::OnLbnSelchangeList2()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = PicsList.GetCurSel();
	if (id < 0)return;

	TextsList.SetCurSel(-1);

	CString filename;
	PicsList.GetText(id, filename);

	/*ImageConverter conv;
	conv.SetImagePath(PresetsPath + L"pic\\" + filename);
	conv.Convert();
	pic.SetData(conv.GetData());*/
	targetpath = PresetsPath + L"pic\\" + filename;
	IsPicture = true;
	pic.SetPicturePath(PresetsPath + L"pic\\" + filename);
	pic.ShowPicture(true);

	pic.Invalidate();
}


void SourceDialog::OnLbnSelchangeList1()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = TextsList.GetCurSel();
	if (id < 0)return;

	PicsList.SetCurSel(-1);

	CString filename;
	TextsList.GetText(id, filename);
	GaussPreloader gauss;
	targetpath = PresetsPath + L"text\\" + filename;
	IsPicture = false;
	gauss.SetPath(PresetsPath + L"text\\" + filename);
	gauss.Load();
	pic.SetData(gauss.GetData());
	pic.ShowPicture(false);
	pic.SetTitle(filename);
	pic.Invalidate();
}


void SourceDialog::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	int idt = TextsList.GetCurSel();
	int idp = PicsList.GetCurSel();
	
	UpdatePresetsLists();

	if (idt >= 0)
	{
		TextsList.SetCurSel(idt);
		CString filename;
		TextsList.GetText(idt, filename);
		GaussPreloader gauss;
		targetpath = PresetsPath + L"text\\" + filename;
		IsPicture = false;
		gauss.SetPath(PresetsPath + L"text\\" + filename);
		gauss.Load();
		pic.SetData(gauss.GetData());
		pic.ShowPicture(false);
		pic.SetTitle(filename);
		pic.Invalidate();
	}
	if (idp >= 0)
	{
		PicsList.SetCurSel(idp);
		CString filename;
		PicsList.GetText(idp, filename);
		targetpath = PresetsPath + L"pic\\" + filename;
		IsPicture = true;
		pic.SetPicturePath(PresetsPath + L"pic\\" + filename);
		pic.ShowPicture(true);
		pic.Invalidate();
	}
}


void SourceDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if ((TextsList.GetCurSel() < 0) && (PicsList.GetCurSel() < 0))
	{
		MessageBoxW(L"Ничего не выбрано!", L"Ошибка", MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}


void SourceDialog::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}
