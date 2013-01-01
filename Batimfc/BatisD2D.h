#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "BatisGame.h"

typedef struct{
	FLOAT SperatorLineRatio,BoardSizeRatio;
}BatisSketchPara;

struct BatisSketch{
	BatisSketch(const BatisSketchPara* Para=DefaultParas);
	FLOAT SperatorLine;
	FLOAT SceneTop,SceneLeft;
	UINT32 SceneWidth,SceneHeight;
	FLOAT BoardTop,BoardLeft,BoardSize,BoardDown,BoardRight;
	FLOAT Delta;
	const BatisSketchPara* Para;
	const static BatisSketchPara DefaultParas[1];
	void Update(int x,int y);
};

class BatisD2D
{
public:
	BatisD2D(void);
	~BatisD2D(void);

	void InitDevice(HWND hWnd);
	void Resize(int x,int y);
	void ResizeEnd();
	void StartGame();
	void Render();
	void HandleClick();
	void HandleMove(int x,int y);

	BatisGame*				bg;
private:	
	static const int nColourMax	= 64;
	void DrawSperatorLine(ID2D1Brush* brush);
	void DrawBoard(ID2D1Brush* brush,int Size);
	void DrawPieces(ID2D1Brush** brush);
	void DrawDebug();
	void DrawInfo();
	void DrawPiece(ID2D1Brush* brush,FLOAT X,FLOAT Y,FLOAT R);
	void DrawHint(ID2D1Brush* Brush,int x,int y,int idx,int mark);
	int						px,py,ActiveX,ActiveY;
	BatisSketch				Sketch;
	ID2D1Factory*			pD2DFactory;	// Direct2D factory
	ID2D1HwndRenderTarget*	pRenderTarget;   // Render target	
	ID2D1SolidColorBrush*	pGrayBrush;
	ID2D1SolidColorBrush*	pWhiteBrush;
	ID2D1Brush*				pBrushes[nColourMax];
	IDWriteFactory       * p_pDWriteFactory;
	IDWriteTextFormat    * m_pText; 
};
