#include "StdAfx.h"
#include "BatisD2D.h"


BatisD2D::BatisD2D(void)
{
}


BatisD2D::~BatisD2D(void)
{
}

void BatisD2D::InitDevice(HWND hWnd)
{
	HRESULT hr;
    RECT rc;
    GetClientRect( hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory) ;
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, 
			D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)
		), 
		&pRenderTarget
	) ;
	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&pBlackBrush
	) ;
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red),&pRedBrush);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green),&pGreenBrush);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue),&pBlueBrush);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow),&pYellowBrush);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),D2D1::BrushProperties(0.6),&pGrayBrush);
	 DWriteCreateFactory(  
            DWRITE_FACTORY_TYPE_SHARED,  
            __uuidof(p_pDWriteFactory),  
            reinterpret_cast<IUnknown **>(&p_pDWriteFactory)  
            );  
	 p_pDWriteFactory->CreateTextFormat(  
            L"Î¢ÈíÑÅºÚ",  
            NULL,  
            DWRITE_FONT_WEIGHT_NORMAL,  
            DWRITE_FONT_STYLE_NORMAL,  
            DWRITE_FONT_STRETCH_NORMAL,  
            13,  
            L"", //locale  
            &m_pText  
            ); 
	 m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);  
	 m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);  

}


void BatisD2D::Render()
{
	pRenderTarget->BeginDraw() ;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Yellow));
	DrawSperatorLine(pBlueBrush);
	pRenderTarget->EndDraw();
}

typedef struct{
	LONG SperatorLine;
	LONG SceneTop,SceneLeft,SceneWidth,SceneHeight;
	LONG BoardTop,BoardLeft,BoardSize,BoardDown,BoardRight;
	LONG Delta;
}BatisSketch;

BatisSketch Sketch={300,200,300,200,300,200,300,200,300,200,100};
void BatisD2D::DrawSperatorLine(ID2D1Brush* Brush)
{
	pRenderTarget->DrawLine(D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop),
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop+Sketch.SceneHeight),
		Brush);
}