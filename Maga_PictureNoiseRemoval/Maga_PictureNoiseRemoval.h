
// Maga_PictureNoiseRemoval.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMagaPictureNoiseRemovalApp:
// Сведения о реализации этого класса: Maga_PictureNoiseRemoval.cpp
//

class CMagaPictureNoiseRemovalApp : public CWinApp
{
public:
	CMagaPictureNoiseRemovalApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMagaPictureNoiseRemovalApp theApp;
