#pragma once
#include <windows.h>
#include <D2D1.h>
#include "ChessBoard.h"
#include "BatisGame.h"
typedef struct{
	FLOAT SperatorLineRatio,BoardSizeRatio;
}BatisSketchPara;

typedef struct{
	LONG SperatorLine;
	LONG SceneTop,SceneLeft,SceneWidth,SceneHeight;
	LONG BoardTop,BoardLeft,BoardSize,BoardDown,BoardRight;
	LONG Delta;
}BatisSketch;

extern int BoardNo;
extern BatisSketchPara DefaultParas[];
extern BatisSketch	Sketch;
extern void UpdateSketch(const RECT* rect,const BatisSketchPara* Para,BatisSketch* Sketch);
extern void DrawDebug(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg);
extern void DrawInfo(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* Brush,IDWriteTextFormat* m_pText,BatisGame* bg);
extern void DrawSperatorLine(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* brush);
extern void DrawBoard(ID2D1RenderTarget* pRenderTarget,ID2D1Brush* brush,int Size);
extern void DrawPieces(ID2D1RenderTarget* pRenderTarget,ID2D1Brush** brush,int Num,const ChessBoard* board);
extern void HandleClick(BatisGame* bg);
extern void HandleMove(int x,int y);