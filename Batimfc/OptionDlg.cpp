// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Batimfc.h"
#include "OptionDlg.h"
#include "afxdialogex.h"


// COptionDlg 对话框

IMPLEMENT_DYNAMIC(COptionDlg, CDialogEx)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionDlg::IDD, pParent)
{

	m_nBoardSize = 0;
	m_nHuman = 0;
	m_nComputer = 0;
	m_nRadio = 0;
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nBoardSize);
	DDV_MinMaxInt(pDX, m_nBoardSize, 4, 28);
	DDX_Text(pDX, IDC_EDIT2, m_nHuman);
	DDV_MinMaxInt(pDX, m_nHuman, 0, 6);
	DDX_Text(pDX, IDC_EDIT3, m_nComputer);
	DDV_MinMaxInt(pDX, m_nComputer, 0, 22);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDV_MinMaxInt(pDX, m_nRadio, 0, 2);
	if (pDX->m_bSaveAndValidate)
		if(m_nHuman+m_nComputer>m_nBoardSize){
			AfxMessageBox(L"Bs too small");
			pDX->Fail();
		}
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &COptionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COptionDlg 消息处理程序


void COptionDlg::OnBnClickedButton1()
{
	m_nHuman	= 1;
	m_nComputer	= 1;
	m_nBoardSize= 8;
	//UpdateData(false);
	UpdateData(true);
	afxDump<<m_nRadio<<"\n";
}

