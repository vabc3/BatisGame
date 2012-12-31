
// Batimfc.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Batimfc.h"
#include "MainFrm.h"
#include "OptionDlg.h"

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

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Batis"));


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






	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
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
	optionDlg.m_nRadio		= theApp.GetProfileIntW(L"Settings",L"Radio",0);
	optionDlg.m_nHuman		= theApp.GetProfileIntW(L"Settings",L"Human",1);
	optionDlg.m_nComputer	= theApp.GetProfileIntW(L"Settings",L"Computer",1);
	optionDlg.m_nBoardSize	= theApp.GetProfileIntW(L"Settings",L"BoardSize",8);
	optionDlg.DoModal();
	theApp.WriteProfileInt(L"Settings",L"Radio",optionDlg.m_nRadio);
	theApp.WriteProfileInt(L"Settings",L"Human",optionDlg.m_nHuman);
	theApp.WriteProfileInt(L"Settings",L"Computer",optionDlg.m_nComputer);
	theApp.WriteProfileInt(L"Settings",L"BoardSize",optionDlg.m_nBoardSize);
}


