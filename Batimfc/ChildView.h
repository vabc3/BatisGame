// ChildView.h : CChildView 类的接口
#pragma once

// CChildView 窗口
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
	// 生成的消息映射函数
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

