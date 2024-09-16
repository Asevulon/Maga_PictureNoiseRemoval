
// MyDlg.h: файл заголовка
//

#pragma once

#include"SourceDialog.h"
#include"MainJob.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGA_PICTURENOISEREMOVAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSource();
	CString targetpath;
	bool IsPicture = false;
	Drawer SourcePicture;
	Drawer FrequencyPicture;
	Drawer NoisedPicture;
	double NoiseLevel;
	double RestoreEnergyLevel;
	Drawer RestoredPicture;
	bool init = false;
	Drawer RestoredFreq;
	Drawer PurePicture;
	CString mistake;
	CString differance;
};
