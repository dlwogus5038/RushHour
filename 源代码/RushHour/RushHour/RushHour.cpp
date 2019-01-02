#include "RushHour.h"

static int i = 20;
static int down = 0;
static bool Jump = false;// 방향키와 점프의 상태 

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //零눗왯湳駕，꼇옵맣긴댕鬼，꼇옵離댕뺏
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//놓迦뺏
		Init(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		//삥齡
		Render(hWnd);
		break;
	case WM_KEYDOWN:
		//숩턍객苟慤숭
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		//숩턍漑역慤숭
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//璘柑깃慤숭
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		//땍珂포慤숭
		TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//속潼교쒼貫暠
	m_hBackgroundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BACKGROUND));
	//속潼Building貫暠
	m_hBuildingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BUILDING));
	//속潼Building貫暠
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO));
	//속潼踏狗榴檄貫暠
	m_hGameStatusBmp =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GAME_STATUS));
	m_hStart = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_START));
	m_hEnd = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_END));
	//속潼Block貫暠
	int k;
	for (k = 0; k < BLOCK_COLOR_NUM; ++k)
	{
		m_hBlockBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_blockBmpNames[k]));
	}
	//속潼肱땅貫暠
	for (k = 0; k < ROOF_COLOR_NUM; ++k)
	{
		m_hRoofkBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_roofBmpNames[k]));
	}

	//눼쉔亶衿、쉔蓼
	m_hero = CreateHero(54, 180, 30, 50, m_hHeroBmp, 0, 3);
	m_building = CreateBuilding(0, 70, 500, 250, m_hBuildingBmp);
	//눼쉔뒈近

	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (k % 4 == 0)
		{
			
			m_terrian[k] = CreateTerrian(k * 64, 400, 64, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
			continue;
		}
		m_terrian[k] = CreateTerrian(k * 64, 180 + 50 * (k % 2), 64, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
	}

	//눼쉔踏狗榴檄
	m_gameStatus = CreateGameStatus(-50, 0, 40, 30, m_hGameStatusBmp);
	m_start = CreateStart(0,0,500,360,m_hStart);
	//폘땡셕珂포
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//역迦삥齡
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, m_hBackgroundBmp);

	//삥齡교쒼돕뻠닸
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	//삥齡쉔蓼돕뻠닸
	SelectObject(hdcBmp, m_building.hBmp);
	TransparentBlt(
		hdcBuffer, m_building.pos.x, m_building.pos.y,
		m_building.size.cx, m_building.size.cy,
		hdcBmp, 0, 0, m_building.size.cx, m_building.size.cy,
		RGB(255, 255, 255)
		);

	//삥齡Hero돕뻠닸
	SelectObject(hdcBmp, m_hero.hBmp);
	TransparentBlt(
		hdcBuffer, m_hero.pos.x, m_hero.pos.y,
		m_hero.size.cx, m_hero.size.cy,
		hdcBmp, 0, m_hero.size.cy * m_hero.curFrameIndex, m_hero.size.cx, m_hero.size.cy,
		RGB(255, 255, 255)
		);

	//삥齡뒈近
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		Terrian terrian = m_terrian[k];
		SelectObject(hdcBmp, terrian.hRoofBmp);
		TransparentBlt(
			hdcBuffer, terrian.pos.x, terrian.pos.y,
			terrian.roofWidth, terrian.roofHeight,
			hdcBmp, 0, 0, terrian.roofWidth, terrian.roofHeight,
			RGB(255, 255, 255)
			);
		SelectObject(hdcBmp, terrian.hBlockBmp);
		int t;
		for (t = 0; t < terrian.blockNum; ++t)
		{
			TransparentBlt(
				hdcBuffer, terrian.pos.x, terrian.pos.y + terrian.roofHeight + terrian.blockHeight * t,
				terrian.blockWidth, terrian.blockHeight,
				hdcBmp, 0, 0, terrian.blockWidth, terrian.blockHeight,
				RGB(255, 255, 255)
				);
		}
	}

	//삥齡踏狗榴檄
	//董界샀셨崎貫暠
	SelectObject(hdcBmp, m_gameStatus.hBmp);
	TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));


	//삥齡롸鑒



	SelectObject(hdcBmp, m_start.hBmp);
	TransparentBlt(hdcBuffer, m_start.pos.x, m_start.pos.y, m_start.size.cx, m_start.size.cy,
		hdcBmp, 0, 0,
		m_start.size.cx, m_start.size.cy, RGB(0, 255, 0));

	TCHAR	szDist[13];
	if(m_start.hBmp == m_hEnd)
	{
		SetTextColor(hdcBuffer, RGB(255,255, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, posX, 15, szDist, wsprintf(szDist, _T("Score:%d"), m_gameStatus.totalDist));
	}
	else
	{
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, posX, 15, szDist, wsprintf(szDist, _T("Score:%d"), m_gameStatus.totalDist));
	}

	//離빈쉥杰唐돨斤口삥齡돕팁캥
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//쀼澗栗都杰欖돨코닸
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//써監삥齡
	EndPaint(hWnd, &ps);
}

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}

GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)ceil((sizeY - roofHeight) * 1.0 / blockHeight); 
	return terrian;
}

Start CreateStart(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Start start;
	start.hBmp = hBmp;
	start.pos.x = posX;
	start.pos.y = posY;
	start.size.cx = sizeX;
	start.size.cy = sizeY;
	return start;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HeroUpdate();
	TerrianUpdate();
	GameStatusUpdate();
	cndehf();
	if(m_hero.pos.x + m_hero.size.cx <= 0 || m_hero.pos.y > 340)
	{
		KillTimer(hWnd, TIMER);
		m_start.pos.x = 0;
		m_start.hBmp = m_hEnd;
		m_gameStatus.pos.x = - 50;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}


VOID HeroUpdate()
{
	//TODO

	if(Jump ==1){
		m_hero.pos.y-=i;
		i--;
		i--;
	}


	//更新?作
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= m_hero.maxFrameSize ? 0 : m_hero.curFrameIndex;
}


VOID TerrianUpdate()
{
	//TODO
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 4;
		if (m_terrian[k].pos.x + m_terrian[k].size.cx <= 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}

VOID GameStatusUpdate()
{
	//TODO
	++m_gameStatus.totalDist;
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		if(jump_time < 2)
		{
			jump_time++;
			Jump = true;
			i = 20;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		m_hero.pos.y = 180;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}


VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	if(m_start.hBmp == m_hStart)
	{
		SetTimer(hWnd, TIMER, 30, NULL);
		m_gameStatus.pos.x = 0;
		m_start.pos.x = 500;
		posX = 400;
	}
	//흔벎듐샌죄셨崎샀董界暠깃
	if (Paused(ptMouse))
	{
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER, 30, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

VOID cndehf()
{
	int k;
	duddnd.left = m_hero.pos.x;
	duddnd.top = m_hero.pos.y;
	duddnd.right = m_hero.pos.x + m_hero.size.cx;
	duddnd.bottom = m_hero.pos.y + m_hero.size.cy;

	for(k = 0; k < MAX_TERRIAN_NUM; k++)
	{
		if(m_terrian[k].pos.x < m_hero.pos.x + m_hero.size.cx && m_terrian[k].pos.x + m_terrian[k].size.cx > m_hero.pos.x + m_hero.size.cx)
		{
			cur = k;
		}
	}
	if(cur == MAX_TERRIAN_NUM - 1)
	{
		next = 0;
	}
	else
	{
		next = cur + 1;
	}
	if(cur == 0)
	{
		pre = MAX_TERRIAN_NUM - 1;
	}
	else
	{
		pre = cur - 1;
	}
	rjsanf[0].left = m_terrian[cur].pos.x;
	rjsanf[0].top = m_terrian[cur].pos.y;
	rjsanf[0].right = m_terrian[cur].pos.x + m_terrian[cur].size.cx;
	rjsanf[0].bottom = m_terrian[cur].pos.y + m_terrian[cur].size.cy;
	rjsanf[1].left = m_terrian[next].pos.x;
	rjsanf[1].top = m_terrian[next].pos.y;
	rjsanf[1].right = m_terrian[next].pos.x + m_terrian[next].size.cx;
	rjsanf[1].bottom = m_terrian[next].pos.y + m_terrian[next].size.cy;
	rjsanf[2].left = m_terrian[pre].pos.x;
	rjsanf[2].top = m_terrian[pre].pos.y;
	rjsanf[2].right = m_terrian[pre].pos.x + m_terrian[pre].size.cx;
	rjsanf[2].bottom = m_terrian[pre].pos.y + m_terrian[pre].size.cy;
	if(duddnd.bottom >= rjsanf[1].top + 10 && duddnd.top <= rjsanf[1].bottom && duddnd.left <= rjsanf[1].right && duddnd.right >= rjsanf[1].left)
	{
		m_hero.pos.x -= 4;
	}
	if(duddnd.bottom >= rjsanf[0].top && duddnd.top <= rjsanf[0].bottom && duddnd.left <= rjsanf[0].right && duddnd.right >= rjsanf[0].left)
	{

		m_hero.pos.y = rjsanf[0].top - m_hero.size.cy;
		if(i < 0)
		{
			Jump = false;
			jump_time = 0;
		}
		if(rjsanf[0].top < 180)
		{
			i = -17;
		}
		else if(rjsanf[0].top == 180)
		{
			i = -22;
		}
	}
	if( duddnd.bottom < rjsanf[0].top && !Jump)
	{
		if(!(duddnd.bottom >= rjsanf[2].top && duddnd.top <= rjsanf[2].bottom && duddnd.left <= rjsanf[2].right && duddnd.right >= rjsanf[2].left))
		{
			Jump = true;
			i = 0;
		}
	}
}