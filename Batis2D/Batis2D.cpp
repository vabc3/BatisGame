#include "Batis2D.h"
#include "BatisGame.h"
#include <vector>
BatisSketch				Sketch;
BatisSketchPara DefaultParas[]={
	{0.74f,0.9f},
};

int ActiveX,ActiveY;

void UpdateSketch(const RECT* rect,const BatisSketchPara* Para,BatisSketch* Sketch)
{
	LONG RHeight		= rect->bottom - rect->top;
	LONG RWidth			= rect->right - rect->left;
	Sketch->SceneTop	= rect->top;
	Sketch->SceneLeft	= rect->left;
	Sketch->SceneWidth	= RWidth;
	Sketch->SceneHeight	= RHeight;
	Sketch->SperatorLine= static_cast<LONG>(rect->left + RWidth * Para->SperatorLineRatio);
	Sketch->BoardSize	= static_cast<LONG>((Sketch->SceneHeight<Sketch->SperatorLine?
		Sketch->SceneHeight:Sketch->SperatorLine)*Para->BoardSizeRatio);
	Sketch->BoardSize	= Sketch->BoardSize / BoardNo * BoardNo; //Make a multiple of BoardNo
	Sketch->BoardTop	= Sketch->SceneTop + (Sketch->SceneHeight - Sketch->BoardSize) / 2;
	Sketch->BoardLeft	= Sketch->SceneLeft + (Sketch->SperatorLine - Sketch->BoardSize) / 2;
	Sketch->BoardDown	= Sketch->BoardTop + Sketch->BoardSize;
	Sketch->BoardRight	= Sketch->BoardLeft + Sketch->BoardSize;
	Sketch->Delta		= Sketch->BoardSize / BoardNo;
}

void DrawSperatorLine(ID2D1RenderTarget* Target,ID2D1Brush* Brush)
{
	Target->DrawLine(D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop),
		D2D1::Point2F(Sketch.SperatorLine,Sketch.SceneTop+Sketch.SceneHeight),
		Brush);
}
extern int px,py;

static void DrawHint(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* Brush,IDWriteTextFormat* m_pText,int x,int y,int idx,int mark)
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

void DrawDebug(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg)
{
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
			D2D1::RectF(0, 0,300,300),  
			Brush
        );
	
	//std::vector<Boint> vb=bg->GetBTable();
	//int idx=1;
	//for(std::vector<Boint>::iterator it=vb.begin();it!=vb.end();it++){
	//	DrawHint(pRenderTarget,Brush,m_pText,Sketch.BoardLeft+it->x*Sketch.Delta,
	//		Sketch.BoardTop+it->y*Sketch.Delta,idx++,it->mark);
	//}

}

const WCHAR WTurnStatusMsg[][32]={L"自动",L"跳过",L"正常"};
const WCHAR WGameStatusMsg[][32]={L"未开始",L"进行中",L"结束"};
void DrawInfo(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg)
{
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



void DrawBoard(ID2D1RenderTarget* Target,ID2D1Brush* Brush,int size)
{
	Target->DrawRectangle(
		D2D1::RectF(Sketch.BoardLeft,Sketch.BoardTop,Sketch.BoardRight,Sketch.BoardDown),
		Brush);
	LONG Delta = Sketch.BoardSize / (size);
	for(int i=1;i<size;i++){
		Target->DrawLine(
			D2D1::Point2F(Sketch.BoardLeft+Delta*i,Sketch.BoardTop),
			D2D1::Point2F(Sketch.BoardLeft+Delta*i,Sketch.BoardDown),
			Brush);
		Target->DrawLine(
			D2D1::Point2F(Sketch.BoardLeft,Sketch.BoardTop+Delta*i),
			D2D1::Point2F(Sketch.BoardRight,Sketch.BoardTop+Delta*i),
			Brush);
	}
	
}

static void DrawPiece(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* brush,int X,int Y,int R)
{
	pRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(X,Y),R,R),
		brush);
}

void DrawPieces(ID2D1RenderTarget* pRenderTarget,ID2D1Brush** brush,int Num,const ChessBoard* board)
{
	LONG D = Sketch.BoardSize / (Num);
	for(int i=0;i<Num;i++)
		for(int j=0;j<Num;j++){
			int idx=board->GetID(i,j);
			if(idx>0)
				DrawPiece(pRenderTarget,brush[idx],Sketch.BoardLeft+D*i+D/2,Sketch.BoardTop+D*j+D/2,D/2.1);
		}

	if(ActiveX>=0 && ActiveY>=0){
		pRenderTarget->FillRoundedRectangle(
			D2D1::RoundedRect(
			D2D1::RectF(Sketch.BoardLeft+D*ActiveX,Sketch.BoardTop+D*ActiveY,
				Sketch.BoardLeft+D*ActiveX+D,Sketch.BoardTop+D*ActiveY+D)
			,17,17),brush[0]);
	}
} 


void HandleClick(BatisGame* bg)
{
	
	if(ActiveX>=0 && ActiveX>=0){
		if(bg->IsWaitingForInput())
		bg->Put(ActiveX,ActiveY);
		bg->Next();
	}else{
		bg->Put();
		bg->Next();
	}
	/*while(bg->AutoGo())
		Sleep(500);*/
}

void HandleMove(int x,int y)
{
	if(x>Sketch.BoardLeft && x<Sketch.BoardRight && y> Sketch.BoardTop && y<Sketch.BoardDown){
		ActiveX=(x-Sketch.BoardLeft)/Sketch.Delta;
		ActiveY=(y-Sketch.BoardTop)/Sketch.Delta;
	}else
	{
		ActiveX=-1;
		ActiveY=-1;
	}
}


//pRenderTarget->DrawText(  
//			sc_helloWorld,  
//			ARRAYSIZE(sc_helloWorld) - 1,  
//			m_pText,  
//			D2D1::RectF(0, 0,300,300),  
//			Brush
//        );  
//HRESULT hr;
//	pRenderTarget->BeginDraw() ;
//
//// Clear background color white
//	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::HotPink));
//
//// Draw Rectangle
//	pRenderTarget->DrawRectangle(
//		D2D1::RectF(100.f, 100.f, 500.f, 500.f),
//		pBlackBrush
//	);
//
//
//	D2D1_GRADIENT_STOP gradientStops[2] ;
//gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow) ;
//gradientStops[0].position = 0.f ;
//gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::GreenYellow) ;
//gradientStops[1].position = 1.f ;
//ID2D1GradientStopCollection* pGradientStops = NULL ;
//hr = pRenderTarget->CreateGradientStopCollection(
//    gradientStops,
//    2, 
//    D2D1_GAMMA_2_2,
//    D2D1_EXTEND_MODE_CLAMP,
//    &pGradientStops
//    ) ;
//	D2D1_ELLIPSE g_Ellipse = D2D1::Ellipse(D2D1::Point2F(300, 300), 200, 150);
//	ID2D1RadialGradientBrush* pRadialGradientBrush;
//	hr = pRenderTarget->CreateRadialGradientBrush(
//    D2D1::RadialGradientBrushProperties(
//    g_Ellipse.point,
//    D2D1::Point2F(0, 0),
//    g_Ellipse.radiusX,
//    g_Ellipse.radiusY),
//    pGradientStops,
//    &pRadialGradientBrush
//    ) ;
//

	//pRenderTarget-> FillEllipse(
	//	D2D1::Ellipse(D2D1::Point2F(400,400),200,150),
	//	pRadialGradientBrush
	//);
//
//	hr = pRenderTarget->EndDraw() ;

