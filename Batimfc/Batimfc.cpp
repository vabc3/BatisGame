// Batimfc.cpp : 定义应用程序的类行为。
//

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


// CBatimfcApp
BEGIN_MESSAGE_MAP(CBatimfcApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBatimfcApp::OnAppAbout)
	ON_COMMAND(ID_NEW_GAME, &CBatimfcApp::OnNewGame)
	ON_COMMAND(ID_OPTION, &CBatimfcApp::OnOption)
END_MESSAGE_MAP()


// CBatimfcApp 构造
CBatimfcApp::CBatimfcApp()
{
	SetAppID(_T("Karata.Batis.Batimfc"));
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CBatimfcApp 对象
CBatimfcApp theApp;
BatisConfigure GConf;
BatisGame* Game;

// CBatimfcApp 初始化
BOOL CBatimfcApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	SetRegistryKey(_T("Batis"));
	GConf.Load();

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//d2d.InitDevice(pFrame->GetSafeHwnd());
	//pFrame->view
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

int CBatimfcApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	return CWinApp::ExitInstance();
}

// CBatimfcApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CBatimfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBatimfcApp 消息处理程序
void CBatimfcApp::OnNewGame()
{
	Game = new BatisGame(GConf.nHuman+GConf.nComputer,GConf.nBoardSize,GConf.nHuman,GConf.nLevel);
	d2d.bg=Game;
	d2d.Update();
	d2d.Render();
	Game->Start();
}

void CBatimfcApp::OnOption()
{
	COptionDlg optionDlg;
	optionDlg.DoModal();
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