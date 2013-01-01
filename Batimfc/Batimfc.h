#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "BatisGame.h"
#include "BatisD2D.h"

class CBatimfcApp : public CWinApp
{
public:
	CBatimfcApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnNewGame();
	afx_msg void OnOption();
};

class BatisConfigure
{
private:
	static LPCTSTR Section;
	static LPCTSTR Keys[4];
public:
	void Save();
	void Load();
	int nBoardSize;
	int nHuman;
	int nComputer;
	int nLevel;
};

extern CBatimfcApp theApp;
extern BatisConfigure GConf;
extern BatisD2D d2d;
