
// Batimfc.h : Batimfc Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CBatimfcApp:
// �йش����ʵ�֣������ Batimfc.cpp
//

class CBatimfcApp : public CWinApp
{
public:
	CBatimfcApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
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
	int nRadio;
};

extern CBatimfcApp theApp;
extern BatisConfigure GConf;
