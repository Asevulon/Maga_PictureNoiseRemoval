#pragma once
#include "afxdialogex.h"
#include "Drawer.h"
#include "GaussPreloader.h"
#include "ImageConverter.h"
#include "RangeStretcher.h"
// Диалоговое окно SourceDialog

class SourceDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SourceDialog)

public:
	SourceDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~SourceDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOURCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	void GetPresetsPath();
	CString PresetsPath;

	void GetPicturesPresets();
	void GetTextPresets();
	void UpdatePresetsLists();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox TextsList;
	CListBox PicsList;
	afx_msg void OnLbnSelchangeList2();
	Drawer pic;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	CString targetpath;
	bool IsPicture = false;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
