#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Batimfc.h"
#include "MainFrm.h"
#include "OptionDlg.h"
#include "BatisD2D.h"
#include "BatisConstant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Global variables
CBatimfcApp theApp;
BatisConfigure GConf;
BatisD2D d2d;

BEGIN_MESSAGE_MAP(CBatimfcApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT,&CBatimfcApp::OnAppAbout)
	ON_COMMAND(ID_NEW_GAME, &CBatimfcApp::OnNewGame)
	ON_COMMAND(ID_OPTION,	&CBatimfcApp::OnOption)
END_MESSAGE_MAP()

CBatimfcApp::CBatimfcApp()
{
	SetAppID(_T("Karata.Batis.Batimfc"));
}

BOOL CBatimfcApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("Batis"));
	GConf.Load();

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		NULL,NULL);
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CBatimfcApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD){}
	enum { IDD = IDD_ABOUTBOX };
};

void CBatimfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CBatimfcApp::OnOption()
{
	COptionDlg optionDlg;
	optionDlg.DoModal();
}

void CBatimfcApp::OnNewGame()
{
	d2d.StartGame();
	
}

LPCTSTR BatisConfigure::Section=L"Settings";
LPCTSTR BatisConfigure::Keys[]={L"Radio",L"Human",L"Computer",L"BoardSize"};

void BatisConfigure::Save()
{
	theApp.WriteProfileInt(Section,Keys[0],nLevel);
	theApp.WriteProfileInt(Section,Keys[1],nHuman);
	theApp.WriteProfileInt(Section,Keys[2],nComputer);
	theApp.WriteProfileInt(Section,Keys[3],nBoardSize);
};

void BatisConfigure::Load()
{
	nLevel		= theApp.GetProfileIntW(Section,Keys[0],BatisConstant::nLevelDefault);
	nHuman		= theApp.GetProfileIntW(Section,Keys[1],BatisConstant::nHumanDefault);
	nComputer	= theApp.GetProfileIntW(Section,Keys[2],BatisConstant::nComputerDefault);
	nBoardSize	= theApp.GetProfileIntW(Section,Keys[3],BatisConstant::nBoardSizeDefault);
};