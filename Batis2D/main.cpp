#include <windows.h>
#include <windowsx.h>
#include <D2D1.h>
#include <dwrite.h>
#include "resource.h"
#include "Batis2D.h"
#include "BatisGame.h"
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
ID2D1Factory*			pD2DFactory = NULL ; // Direct2D factory
ID2D1HwndRenderTarget*	pRenderTarget = NULL;   // Render target
ID2D1SolidColorBrush*	pBlackBrush = NULL ; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pRedBrush = NULL ; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pGreenBrush = NULL ; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pBlueBrush = NULL ; // A black brush, reflect the line color
ID2D1SolidColorBrush*	pYellowBrush=NULL;
ID2D1SolidColorBrush*	pGrayBrush=NULL;
IDWriteFactory       * p_pDWriteFactory = NULL;  
IDWriteTextFormat    * m_pText = NULL;  
BatisGame*				BGame;
int px,py;
int BoardNo;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();


void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
	if(BGame){
		BGame->AutoGo();
	}
}


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	BGame	= new BatisGame(4,8);
	BGame->Start();
	BoardNo=BGame->GetBoardSize();
	
    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	SetTimer(g_hWnd,1,50,TimerProc);
	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_ICON2 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"BatisWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_ICON2 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 800, 600 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"BatisWindowClass", L"Batis",
					WS_OVERLAPPEDWINDOW,//WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory) ;
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			g_hWnd, 
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


    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}
void CleanupDevice()
{
	SAFE_RELEASE(pRenderTarget) ;
	SAFE_RELEASE(pBlackBrush) ;
	SAFE_RELEASE(pD2DFactory) ;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
	/*static int m_lastWidth;
	static float m_prop=6.0/8;*/
    switch( message )
    {
		case WM_LBUTTONDOWN:
			HandleClick(BGame);
			break;

		case WM_MOUSEMOVE:
			px = GET_X_LPARAM(lParam); 
			py = GET_Y_LPARAM(lParam); 
			HandleMove(px,py);
			break;

        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
			Render();
            EndPaint( hWnd, &ps );
            break;
		case WM_SIZE:
			RECT rect;
			GetClientRect(hWnd,&rect);
			UpdateSketch(&rect,&DefaultParas[0],&Sketch);
			pRenderTarget->Resize(D2D1::SizeU(Sketch.SceneWidth,Sketch.SceneHeight));
			//if((rect.right-rect.left)!=m_lastWidth)         //如果调整宽度发生变化，则将高度调整
			//	rect.bottom=rect.top+(rect.right-rect.left)*m_prop;
			//else                            //如果调整的是高度，则将宽度调整
			//	rect.right=rect.left+(rect.bottom-rect.top)/m_prop;
			//MoveWindow(hWnd,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,false);
			//m_lastWidth=rect.right-rect.left;
			break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}



//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	pRenderTarget->BeginDraw() ;
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	ID2D1Brush* bst[]={
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
		pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,pGrayBrush,pBlueBrush,pGreenBrush,pRedBrush,pYellowBrush,
	};
	DrawSperatorLine(pRenderTarget,pGreenBrush);
	DrawBoard(pRenderTarget,pBlueBrush,BGame->GetBoardSize());
	DrawPieces(pRenderTarget,bst,BGame->GetBoardSize(),BGame->GetBoard());
	DrawInfo(pRenderTarget,pBlackBrush,m_pText,BGame);
	DrawDebug(pRenderTarget,pBlackBrush,m_pText,BGame);
	pRenderTarget->EndDraw();
}


