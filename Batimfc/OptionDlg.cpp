// OptionDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "Batimfc.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "BatisConstant.h"

// COptionDlg �Ի���
IMPLEMENT_DYNAMIC(COptionDlg, CDialogEx)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionDlg::IDD, pParent)
{
	m_nRadio		= GConf.nLevel;
	m_nHuman		= GConf.nHuman;
	m_nComputer		= GConf.nComputer;
	m_nBoardSize	= GConf.nBoardSize;
}

COptionDlg::~COptionDlg(){}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nBoardSize);
	DDV_MinMaxInt(pDX, m_nBoardSize, BatisConstant::nBoardSizeMin, BatisConstant::nBoardSizeMax);
	DDX_Text(pDX, IDC_EDIT2, m_nHuman);
	DDV_MinMaxInt(pDX, m_nHuman, BatisConstant::nHumanMin, BatisConstant::nHumanMax);
	DDX_Text(pDX, IDC_EDIT3, m_nComputer);
	DDV_MinMaxInt(pDX, m_nComputer, BatisConstant::nComputerMin, BatisConstant::nComputerMax);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDV_MinMaxInt(pDX, m_nRadio, BatisConstant::nLevelMin, BatisConstant::nLevelMax);
	if (pDX->m_bSaveAndValidate){
		if(m_nHuman+m_nComputer>m_nBoardSize){
			CString ct;
			ct.LoadString(IDS_OPT_ERROR1);
			AfxMessageBox(ct);
			pDX->Fail();
		}
		if((m_nHuman+m_nComputer-m_nBoardSize)%2!=0){
			m_nBoardSize++;
		}
	}

	if (pDX->m_bSaveAndValidate){
		GConf.nLevel	= m_nRadio;
		GConf.nHuman	= m_nHuman;
		GConf.nComputer	= m_nComputer;
		GConf.nBoardSize= m_nBoardSize;
		GConf.Save();
	}
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &COptionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

void COptionDlg::OnBnClickedButton1()
{
	m_nHuman	= BatisConstant::nHumanDefault;
	m_nComputer	= BatisConstant::nComputerDefault;
	m_nBoardSize= BatisConstant::nBoardSizeDefault;
	m_nRadio	= BatisConstant::nLevelDefault;
	UpdateData(false);
}

