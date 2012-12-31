#pragma once
#include <dwrite.h>
#include <d2d1.h>

class BatisD2D
{
public:
	BatisD2D(void);
	~BatisD2D(void);

	void InitDevice(HWND hWnd);
	ID2D1HwndRenderTarget*	pRenderTarget;   // Render target	
	void Render();
	void DrawSperatorLine(ID2D1Brush* brush);

private:
	ID2D1Factory*			pD2DFactory; // Direct2D factory

ID2D1SolidColorBrush*	pBlackBrush; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pRedBrush; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pGreenBrush ; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pBlueBrush; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pYellowBrush;
ID2D1SolidColorBrush*	pGrayBrush;
IDWriteFactory       * p_pDWriteFactory;
IDWriteTextFormat    * m_pText;  
};

