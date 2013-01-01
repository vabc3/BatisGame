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

void BatisSketch::Update(int BoardNo,BatisSketchPara* Para)
{
	Update(SceneWidth,SceneHeight,BoardNo,Para);
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
	
	int Delta	= 0xFFFFFF / (nColourMax+1);
	for(int i=0;i<nColourMax;i++){
		ID2D1SolidColorBrush*	tmp;
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF((i+1)*Delta),&tmp);
		pBrushes[i]=tmp;
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
	pRenderTarget->BeginDraw() ;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Green));
	
	DrawSperatorLine(pBlueBrush);
	if(bg){
	DrawBoard(pWhiteBrush,bg->GetBoardSize());
	ID2D1Brush* bst[]={
		pBlueBrush,pGreenBrush,pRedBrush,pRedBrush,pBlueBrush,pGreenBrush,pRedBrush,
	};
	
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

void BatisD2D::Resize(int x,int y)
{
	if(pRenderTarget){
		Sketch.Update(x,y,bg?bg->GetBoardSize():8);
		//pRenderTarget->Resize(D2D1::SizeU(x,y));
	}
}

void BatisD2D::ResizeEnd()
{
	if(pRenderTarget){
		pRenderTarget->Resize(D2D1::SizeU(Sketch.SceneWidth,Sketch.SceneHeight));
	}
}


void BatisD2D::Update()
{
	Sketch.Update(bg?bg->GetBoardSize():8);
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



void BatisD2D::DrawHint(ID2D1Brush* Brush,int x,int y,int idx,int mark)
{
	WCHAR dk[255];
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
	  wcscat(dk,L"局势：");
	  WCHAR NC[10];
	  const ChessBoard* board=bg->GetBoard();
	  int num=bg->GetNumberOfPlayer();
	  for(int i=0;i<=num;i++){
		  wsprintf(NC,L"%d:",board->GetStatus(i));
		  wcscat(dk,NC);
	  }
	  
		pRenderTarget->DrawText(  
			dk,  
			wcslen(dk),  
			m_pText,  
			D2D1::RectF(Sketch.SperatorLine, 0,Sketch.SperatorLine+300,300),  
			Brush
        );
}


void BatisD2D::DrawPiece(ID2D1Brush* brush,int X,int Y,int R)
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
	LONG D = Sketch.BoardSize / (Num);
	for(int i=0;i<Num;i++){
		for(int j=0;j<Num;j++){
			int idx=board->GetID(i,j)*(nColourMax-1) /  bg->GetNumberOfPlayer() ;
			//afxDump<<idx<<"|";
			if(idx>0)
				DrawPiece(brush[idx],Sketch.BoardLeft+D*i+D/2,Sketch.BoardTop+D*j+D/2,D/2.1);
		}
		//afxDump<<"\n";
	}

	if(ActiveX>=0 && ActiveY>=0){
		pRenderTarget->FillRoundedRectangle(
			D2D1::RoundedRect(
			D2D1::RectF(Sketch.BoardLeft+D*ActiveX,Sketch.BoardTop+D*ActiveY,
				Sketch.BoardLeft+D*ActiveX+D,Sketch.BoardTop+D*ActiveY+D)
			,17,17),brush[0]);
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
		ActiveX=(x-Sketch.BoardLeft)/Sketch.Delta;
		ActiveY=(y-Sketch.BoardTop)/Sketch.Delta;
	}else
	{
		ActiveX=-1;
		ActiveY=-1;
	}
	Render();
}