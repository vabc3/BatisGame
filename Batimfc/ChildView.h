// ChildView.h : CChildView ��Ľӿ�
#pragma once
#include "BatisD2D.h"

// CChildView ����
class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExitSizeMove();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// ���ɵ���Ϣӳ�亯��
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

extern BatisD2D d2d;