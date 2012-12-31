// Batimfc.cpp : ����Ӧ�ó��������Ϊ��
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


// CBatimfcApp ����
CBatimfcApp::CBatimfcApp()
{
	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Karata.Batis.Batimfc"));
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CBatimfcApp ����
CBatimfcApp theApp;
BatisConfigure GConf;

// CBatimfcApp ��ʼ��
BOOL CBatimfcApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	SetRegistryKey(_T("Batis"));
	GConf.Load();

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//d2d.InitDevice(pFrame->GetSafeHwnd());
	//pFrame->view
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

int CBatimfcApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	return CWinApp::ExitInstance();
}

// CBatimfcApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CBatimfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBatimfcApp ��Ϣ�������
void CBatimfcApp::OnNewGame()
{
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
	theApp.WriteProfileInt(Section,Keys[0],nRadio);
	theApp.WriteProfileInt(Section,Keys[1],nHuman);
	theApp.WriteProfileInt(Section,Keys[2],nComputer);
	theApp.WriteProfileInt(Section,Keys[3],nBoardSize);
};

void BatisConfigure::Load()
{
	nRadio		= theApp.GetProfileIntW(Section,Keys[0],BatisConstant::nLevelDefault);
	nHuman		= theApp.GetProfileIntW(Section,Keys[1],BatisConstant::nHumanDefault);
	nComputer	= theApp.GetProfileIntW(Section,Keys[2],BatisConstant::nComputerDefault);
	nBoardSize	= theApp.GetProfileIntW(Section,Keys[3],BatisConstant::nBoardSizeDefault);
};