#pragma once

// COptionDlg �Ի���

class COptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionDlg();

// �Ի�������
	enum { IDD = IDD_OPTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nBoardSize;
	int m_nHuman;
	int m_nComputer;
	int m_nRadio;
	afx_msg void OnBnClickedButton1();
};
