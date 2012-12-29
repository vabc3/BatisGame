#pragma once


// CNewGameDlg 对话框

class CNewGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewGameDlg)

public:
	CNewGameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewGameDlg();

// 对话框数据
	enum { IDD = IDD_NEWGAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
