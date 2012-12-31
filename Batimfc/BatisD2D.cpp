#include "StdAfx.h"
#include "BatisD2D.h"

BatisSketchPara BatisSketch::DefaultParas[]={
	{0.74f,0.99f},
};

void BatisSketch::Update(int Width,int Height,int BoardNo,BatisSketchPara* Para)
{
	SceneTop	= 0;
	SceneLeft	= 0;
	SceneWidth	= Width;
	SceneHeight	= Height;
	SperatorLine= static_cast<LONG>(Width * Para->SperatorLineRatio);
	BoardSize	= static_cast<LONG>((SceneHeight<SperatorLine?
					SceneHeight:SperatorLine)*Para->BoardSizeRatio);
	Delta		= BoardSize / BoardNo;
	BoardSize	= Delta * BoardNo; //Make a multiple of BoardNo
	BoardTop	= SceneTop + (SceneHeight - BoardSize) / 2;
	BoardLeft	= SceneLeft + (SperatorLine - BoardSize) / 2;
	BoardDown	= BoardTop + BoardSize;
	BoardRight	= BoardLeft + BoardSize;
}


BatisD2D::BatisD2D(void)
{
}

BatisD2D::~BatisD2D(void)
{
	//TODO Release
}

void BatisD2D::InitDevice(HWND hWnd)
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory) ;
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd), 
		&pRenderTarget
	) ;
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),&pWhiteBrush);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),&pBlackBrush);
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
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Green));
	DrawSperatorLine(pBlueBrush);
	DrawBoard(pWhiteBrush,8);
	pRenderTarget->EndDraw();
}

void BatisD2D::DrawSperatorLine(ID2D1Brush* Brush)
{
	pRenderTarget->DrawLine(
		D2D1::Point2F(200,1),
		D2D1::Point2F(1,300),
		Brush);
	pRenderTarget->DrawLine(
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop),
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop+Sketch.SceneHeight),
		Brush);
}

void BatisD2D::Resize(int x,int y)
{
	if(pRenderTarget){
		Sketch.Update(x,y,8);
		pRenderTarget->Resize(D2D1::SizeU(x,y));
	}
}

void BatisD2D::DrawBoard(ID2D1Brush* Brush,int Size)
{
	pRenderTarget->DrawRectangle(
		D2D1::RectF(Sketch.BoardLeft,Sketch.BoardTop,Sketch.BoardRight,Sketch.BoardDown),
		Brush);
	LONG Delta = Sketch.BoardSize / (Size);
	for(int i=1;i<Size;i++){
		pRenderTarget->DrawLine(
			D2D1::Point2F(Sketch.BoardLeft+Delta*i,Sketch.BoardTop),
			D2D1::Point2F(Sketch.BoardLeft+Delta*i,Sketch.BoardDown),
			Brush);
		pRenderTarget->DrawLine(
			D2D1::Point2F(Sketch.BoardLeft,Sketch.BoardTop+Delta*i),
			D2D1::Point2F(Sketch.BoardRight,Sketch.BoardTop+Delta*i),
			Brush);
	}
}