
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "Batimfc.h"
#include "ChildView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()

// CChildView 消息处理程序
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	// TODO: 在此处添加消息处理程序代码
	// 不要为绘制消息而调用 CWnd::OnPaint()
	d2d.ResizeEnd();
	d2d.Render();
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
	d2d.HandleMove(point.x,point.y);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	d2d.HandleClick();
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	afxDump<<"OS\n";
	CWnd::OnSize(nType, cx, cy);
	d2d.Resize(cx,cy);
	//d2d.ResizeEnd();
	d2d.Render();
}


void CChildView::OnExitSizeMove()
{
	afxDump<<"OSE\n";
	CWnd::OnExitSizeMove();
	d2d.ResizeEnd();
}
