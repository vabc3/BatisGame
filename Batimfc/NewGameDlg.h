#pragma once


// CNewGameDlg �Ի���

class CNewGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewGameDlg)

public:
	CNewGameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewGameDlg();

// �Ի�������
	enum { IDD = IDD_NEWGAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
