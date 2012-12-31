#pragma once
#include <dwrite.h>
#include <d2d1.h>

typedef struct{
	FLOAT SperatorLineRatio,BoardSizeRatio;
}BatisSketchPara;

struct BatisSketch{
	int SperatorLine;
	LONG SceneTop,SceneLeft,SceneWidth,SceneHeight;
	LONG BoardTop,BoardLeft,BoardSize,BoardDown,BoardRight;
	LONG Delta;
	static BatisSketchPara DefaultParas[1];
	void Update(int x,int y,int BoardNo,BatisSketchPara* para=DefaultParas);
};

class BatisD2D
{
public:
	BatisD2D(void);
	~BatisD2D(void);

	void InitDevice(HWND hWnd);
	void Resize(int x,int y);
	void Render();
	void DrawSperatorLine(ID2D1Brush* brush);
	void DrawBoard(ID2D1Brush* brush,int Size);
	//void DrawDebug(ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg);
	//void DrawInfo(ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg);
	//void DrawPieces(ID2D1Brush** brush,int Num,const ChessBoard* board);
	//void HandleClick(BatisGame* bg);
	void HandleMove(int x,int y);

private:
	BatisSketch				Sketch;
	ID2D1Factory*			pD2DFactory; // Direct2D factory
	ID2D1HwndRenderTarget*	pRenderTarget;   // Render target	
	ID2D1SolidColorBrush*	pBlackBrush; // A black brush, reflect the line color
	ID2D1SolidColorBrush*	pRedBrush; // A black brush, reflect the line color
	ID2D1SolidColorBrush*	pGreenBrush ; // A black brush, reflect the line color
	ID2D1SolidColorBrush*	pBlueBrush; // A black brush, reflect the line color
	ID2D1SolidColorBrush*	pYellowBrush;
	ID2D1SolidColorBrush*	pGrayBrush;
	ID2D1SolidColorBrush*  pWhiteBrush;
	IDWriteFactory       * p_pDWriteFactory;
	IDWriteTextFormat    * m_pText;  
};
