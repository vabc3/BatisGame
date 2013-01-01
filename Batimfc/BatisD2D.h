#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "BatisGame.h"

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
	void Update(int BoardNo,BatisSketchPara* para=DefaultParas);
};

class BatisD2D
{
public:
	BatisD2D(void);
	~BatisD2D(void);

	void InitDevice(HWND hWnd);
	void Resize(int x,int y);
	void ResizeEnd();
	void Update();
	void Render();
	void DrawSperatorLine(ID2D1Brush* brush);
	void DrawBoard(ID2D1Brush* brush,int Size);
	void DrawDebug();
	void DrawInfo();
	void DrawPieces(ID2D1Brush** brush);
	void HandleClick();
	void HandleMove(int x,int y);

	BatisGame*				bg;
private:	
	static const int nColourMax	= 64;
	void DrawPiece(ID2D1Brush* brush,int X,int Y,int R);
	void DrawHint(ID2D1Brush* Brush,int x,int y,int idx,int mark);
	int						px,py,ActiveX,ActiveY;
	BatisSketch				Sketch;
	ID2D1Factory*			pD2DFactory; // Direct2D factory
	ID2D1HwndRenderTarget*	pRenderTarget;   // Render target	
	ID2D1SolidColorBrush*	pGrayBrush;
	ID2D1SolidColorBrush*	pWhiteBrush;
	ID2D1Brush*	pBrushes[nColourMax];

	IDWriteFactory       * p_pDWriteFactory;
	IDWriteTextFormat    * m_pText; 
	
};
