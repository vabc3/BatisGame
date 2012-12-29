// NewGameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Batimfc.h"
#include "NewGameDlg.h"
#include "afxdialogex.h"


// CNewGameDlg 对话框

IMPLEMENT_DYNAMIC(CNewGameDlg, CDialogEx)

CNewGameDlg::CNewGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewGameDlg::IDD, pParent)
{

}

CNewGameDlg::~CNewGameDlg()
{
}

void CNewGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewGameDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewGameDlg 消息处理程序
