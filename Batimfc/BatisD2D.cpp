#include "StdAfx.h"
#include "BatisD2D.h"
#include "Batimfc.h"

const BatisSketchPara BatisSketch::DefaultParas[]={
	{0.74f,0.97f},
};

BatisSketch::BatisSketch(const BatisSketchPara* Para):Para(Para){}

void BatisSketch::Update(int Width,int Height)
{
	
	SceneTop	= 0;
	SceneLeft	= 0;
	SceneWidth	= Width;
	SceneHeight	= Height;
	SperatorLine= Width * Para->SperatorLineRatio;
	Update();
	
}

void BatisSketch::Update(){
	int BoardNo	= GConf.nBoardSize;
	BoardSize	= (SceneHeight<SperatorLine?
		SceneHeight:SperatorLine)*Para->BoardSizeRatio;
	Delta		= BoardSize / BoardNo;
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
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray,.5),&pGrayBrush);
	
	int Delta	= 0xFFFFFF / (nColourMax+1);
	for(int i=0;i<nColourMax;i++){
		ID2D1SolidColorBrush*	tmp;
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF((i+1)*Delta),&tmp);
		D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES gp;
		
		ID2D1GradientStopCollection* gc;
		D2D1_GRADIENT_STOP st[]={
			D2D1::GradientStop(3,D2D1::ColorF((i+1)*Delta)),
			D2D1::GradientStop(5,D2D1::ColorF(0))
		};
		pRenderTarget->CreateGradientStopCollection(st,2,&gc);
		ID2D1RadialGradientBrush* tmp2;
		pRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(0,0),
			D2D1::Point2F(3,3),
			5,5),
			D2D1::BrushProperties(1),
			gc,
			&tmp2);
			
		pBrushes[i]=tmp2;
	}

	DWriteCreateFactory(  
            DWRITE_FACTORY_TYPE_SHARED,  
            __uuidof(p_pDWriteFactory),  
            reinterpret_cast<IUnknown **>(&p_pDWriteFactory)  
            );  
	 p_pDWriteFactory->CreateTextFormat(  
            L"微软雅黑",  
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
	if(!pRenderTarget)return;
	pRenderTarget->BeginDraw() ;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Green));
	
	DrawSperatorLine(pWhiteBrush);
	if(bg){
		DrawBoard(pWhiteBrush,bg->GetBoardSize());
		DrawPieces(pBrushes);
		DrawInfo();
		DrawDebug();
	}
	pRenderTarget->EndDraw();
}

void BatisD2D::DrawSperatorLine(ID2D1Brush* Brush)
{
	pRenderTarget->DrawLine(
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop),
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop+Sketch.SceneHeight),
		Brush);
}

static int rx,ry;
void BatisD2D::Resize(int x,int y)
{
	rx=x;ry=y;
	//if(pRenderTarget){
	//	Sketch.Update(x,y,bg?bg->GetBoardSize():8);
	//	//pRenderTarget->Resize(D2D1::SizeU(x,y));
	//}
}

void BatisD2D::ResizeEnd()
{
	if(pRenderTarget){
		Sketch.Update(rx,ry);
		pRenderTarget->Resize(D2D1::SizeU(Sketch.SceneWidth,Sketch.SceneHeight));
	}
}

void BatisD2D::StartGame()
{
	bg = new BatisGame(GConf.nHuman+GConf.nComputer,GConf.nBoardSize,GConf.nHuman,GConf.nLevel);
	bg->Start();
	Sketch.Update();
	Render();
}

void BatisD2D::DrawBoard(ID2D1Brush* Brush,int Size)
{
	pRenderTarget->DrawRectangle(
		D2D1::RectF(Sketch.BoardLeft,Sketch.BoardTop,Sketch.BoardRight,Sketch.BoardDown),
		Brush);
	FLOAT Delta = Sketch.Delta;
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



void BatisD2D::DrawHint(ID2D1Brush* Brush,int X,int Y,int idx,int mark)
{
	WCHAR dk[255];
	FLOAT x=static_cast<FLOAT>(X);
	FLOAT y=static_cast<FLOAT>(Y);
	wsprintf(dk,L"%d(%d)",idx,mark);
	pRenderTarget->DrawText(  
			dk,  
			wcslen(dk),  
			m_pText, 
			D2D1::RectF(x, y,x+30,y+30),  
			Brush
        );
}

void BatisD2D::DrawDebug()
{
	ID2D1Brush* Brush=pWhiteBrush;
    WCHAR dk[255];
	wsprintf(dk,L"S:(%ld,%ld,%ld,%ld)\nLine:%ld\nMouse:(%ld,%ld)\nAXY:(%ld,%ld)\nap:%d",
		Sketch.SceneLeft,Sketch.SceneTop,
		Sketch.SceneWidth,Sketch.SceneHeight,
		Sketch.SperatorLine,
		px,py,
		ActiveX,ActiveY,
		Boint::ap
		);
	pRenderTarget->DrawText(  
			dk,  
			wcslen(dk),  
			m_pText,  
			D2D1::RectF(Sketch.SperatorLine, 400,Sketch.SperatorLine+300,300),  
			Brush
        );
	
	/*std::vector<Boint> vb=bg->GetBTable();
	int idx=1;
	for(std::vector<Boint>::iterator it=vb.begin();it!=vb.end();it++){
		DrawHint(pRenderTarget,Brush,m_pText,Sketch.BoardLeft+it->x*Sketch.Delta,
			Sketch.BoardTop+it->y*Sketch.Delta,idx++,it->mark);
	}*/

}

const WCHAR WTurnStatusMsg[][32]={L"自动",L"跳过",L"正常"};
const WCHAR WGameStatusMsg[][32]={L"未开始",L"进行中",L"结束"};
void BatisD2D::DrawInfo()
{
	ID2D1Brush* Brush=pWhiteBrush;
	  WCHAR dk[255];

	  wsprintf(dk,L"游戏状态:%s\n轮状态:%s\n轮数:%d\n选手:%d\n",
		  WGameStatusMsg[bg->GetGameStatus()],WTurnStatusMsg[bg->GetTurnStatus()],bg->GetTurn(),bg->GetActivePlayer()
		);
	  wcscat_s(dk,L"局势：");
	  WCHAR NC[10];
	  const ChessBoard* board=bg->GetBoard();
	  int num=bg->GetNumberOfPlayer();
	  for(int i=0;i<=num;i++){
		  wsprintf(NC,L"%d:",board->GetStatus(i));
		  wcscat_s(dk,NC);
	  }
	  
		pRenderTarget->DrawText(  
			dk,  
			wcslen(dk),  
			m_pText,  
			D2D1::RectF(Sketch.SperatorLine, 0,Sketch.SperatorLine+300,300),  
			Brush
        );
}

void BatisD2D::DrawPiece(ID2D1Brush* brush,FLOAT X,FLOAT Y,FLOAT R)
{
	
	pRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(X,Y),R,R),
		brush);
	pRenderTarget->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(X,Y),R,R),
		pWhiteBrush);
}

void BatisD2D::DrawPieces(ID2D1Brush** brush)
{
	const ChessBoard* board=bg->GetBoard();
	int Num=bg->GetBoardSize();
	FLOAT D = Sketch.Delta;
	for(int i=0;i<Num;i++){
		for(int j=0;j<Num;j++){
			int idx=board->GetID(i,j)*(nColourMax-1) /  bg->GetNumberOfPlayer() ;
			if(idx>0)
				DrawPiece(brush[idx],
				Sketch.BoardLeft+D*i+D/2,
				Sketch.BoardTop+D*j+D/2,
				D/2.1F);
		}
	}

	if(ActiveX>=0 && ActiveY>=0){
		pRenderTarget->FillRoundedRectangle(
			D2D1::RoundedRect(
			D2D1::RectF(Sketch.BoardLeft+D*ActiveX,Sketch.BoardTop+D*ActiveY,
				Sketch.BoardLeft+D*ActiveX+D,Sketch.BoardTop+D*ActiveY+D)
				,17,17),pGrayBrush);
	}
} 


void BatisD2D::HandleClick()
{
	if(!bg)return ;
	if(ActiveX>=0 && ActiveX>=0){
		if(bg->IsWaitingForInput())
		bg->Put(ActiveX,ActiveY);
		bg->Next();
	}else{
		bg->Put();
		bg->Next();
	}
	//while(bg->AutoGo())Sleep(500);
}

void BatisD2D::HandleMove(int x,int y)
{
	px=x;py=y;
	if(x>Sketch.BoardLeft && x<Sketch.BoardRight && y> Sketch.BoardTop && y<Sketch.BoardDown){
		ActiveX=static_cast<int>((x-Sketch.BoardLeft)/Sketch.Delta);
		ActiveY=static_cast<int>((y-Sketch.BoardTop)/Sketch.Delta);
	}else
	{
		ActiveX=-1;
		ActiveY=-1;
	}
	Render();
}