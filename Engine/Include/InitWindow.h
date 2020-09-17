#ifndef __INIT_WINDOW__
#define __INIT_WINDOW__


const std::string APPTITLE = "Engine";

HWND				window;
HDC					device, buffer, buffer2;
BITMAPINFOHEADER    bih;			// ���������� � ��������
HBITMAP             hBmp;			
DWORD              *pBmp;			// ����� � ���������� ���������
HGDIOBJ				hOld;

HFONT				hFont;



DWORD ScreenBuffer[2000][2000];		// �������������� ��������  ������ � �������
float  DepthBuffer[2000][2000];		// �������������� ��������� ������ � �������
//==========================================================================================
//									���������� ����������
//==========================================================================================
bool  mmENDPROG = false;
bool  mmFULL_SCREEN = false;
bool  mmVsync = true;
int   mmWIDTH;
int   mmHEIGHT;
short mmWHEEL;
int   mmFLAG[10];
float mmVALUE[10];
//==========================================================================================
//							�������, ���������� � ����� ��������
//==========================================================================================
void ClearPixels()
{
	for(int j = 0; j<mmHEIGHT; j++)	for(int i = 0; i<mmWIDTH; i++) ScreenBuffer[i][j] = 0, DepthBuffer[i][j] = 999999;
}
void UpdatePixels()
{
	for(int j = 0; j<mmHEIGHT; j++)	for(int i = 0; i<mmWIDTH; i++) pBmp[j*mmWIDTH + i] = ScreenBuffer[i][j];
}
void RenderScene()
{
	BitBlt(device, 0, 0, mmWIDTH, mmHEIGHT, buffer, 0, 0, SRCCOPY);		// ������ �������
	BitBlt(device, 0, 0, mmWIDTH, mmHEIGHT, buffer, 0, 0, SRCCOPY);		// ������� ����� ������� ����� ��������							
}
//==========================================================================================
//									������������� ����
//==========================================================================================
void SetResolution(int WIDTH, int HEIGHT)
{
	mmWIDTH  = WIDTH;
	mmHEIGHT = HEIGHT;

	memset(&bih, 0, sizeof bih);			// ������� ��������� �����������
    bih.biSize          = sizeof bih;		// ��������� ��� ������ �����������
    bih.biPlanes        = 1;
    bih.biBitCount      = 32;				// 32 ���� �� ������� (R=8 G=8 B=8) + 8 ��� �� ����� ����� (32-������ ����� � �����������) 
    bih.biCompression   = BI_RGB;			// ������� ������
    bih.biWidth         = mmWIDTH;
    bih.biHeight        = mmHEIGHT;

	hBmp   = CreateDIBSection(NULL, (PBITMAPINFO)&bih, DIB_RGB_COLORS, (PVOID *)&pBmp, NULL, NULL);	// ������ �����������
	SelectObject(buffer, hBmp);

	RECT WindowRect;		   
	RECT ClientRect;
	GetClientRect(window, &ClientRect);
	GetWindowRect(window, &WindowRect);
	int  sx = GetSystemMetrics(SM_CXSCREEN);// ������ ������� � �������� (� ������� �����������)
	int  sy = GetSystemMetrics(SM_CYSCREEN);// ������ ������� � �������� (� ������� �����������)
	int  width  = mmWIDTH  + ((WindowRect.right  - WindowRect.left) - ClientRect.right);
	int  height = mmHEIGHT + ((WindowRect.bottom - WindowRect.top)  - ClientRect.bottom);
	SetWindowPos(window, 0, sx/2-width/2, sy/2-height/2, width, height, 0);  
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message) 
	{
    case WM_DESTROY:
        mmENDPROG = true;
        PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL:
		mmWHEEL = ((wParam >> 16) & 0xffff);
		break;
    }
	return DefWindowProc(hWnd, message, wParam, lParam);
}
BOOL InitInstance(HINSTANCE hInstance) 
{
	WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
    wc.hIconSm = NULL;
	RegisterClassEx(&wc);
	
	DWORD Style = mmFULL_SCREEN ? (WS_POPUP) : (WS_OVERLAPPEDWINDOW);

	window = CreateWindow(APPTITLE.c_str(), APPTITLE.c_str(), Style, 111,111, 333,333, NULL, NULL, hInstance, NULL);
	if(window == 0) return 0;

	ShowWindow(window, mmFULL_SCREEN ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT);
    UpdateWindow(window);

    device = GetDC(window);
	buffer = CreateCompatibleDC(device);	// ������ ��������

	
	SetBkMode(buffer, TRANSPARENT);
	hFont  = CreateFont(20, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, "Consolas");
	hOld   = SelectObject(buffer, hFont);	// ��� ���������� ��������� ���������� ������
	
	SetResolution(400, 400);



	BITMAP		bm;
	HBITMAP		bit;
	BYTE	   *buff;

	bit = (HBITMAP)LoadImage(hInstance, "texture.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	ZeroMemory(&bm, sizeof(BITMAP));

	if(GetObject(bit, sizeof(BITMAP), (LPVOID)&bm) == NULL)	MessageBox(NULL, "Error, GetObject(bit)", "ERROR", MB_OK);
	
	texture[0].width  = bm.bmWidth;
	texture[0].height = bm.bmHeight;
	
	buff = (BYTE*)bm.bmBits;

	for(int y = 0; y < bm.bmHeight; y++)
	for(int x = 0; x < bm.bmWidth;  x++)
	{
		BYTE B = buff[y*bm.bmWidthBytes + 3*x + 0];		// ������� � bmWidthBytes ����� ������, ������� ������� 4 (rgba)
		BYTE G = buff[y*bm.bmWidthBytes + 3*x + 1];
		BYTE R = buff[y*bm.bmWidthBytes + 3*x + 2];
		texture[0].pixels[x][y] = Pixel(R, G, B);
	}
		
    return 1;
}
void ReleaseInstance()
{
	SelectObject(buffer, hOld);				// hOld �������� ������ �� ������� ������ SelectObject � ����� ���������
	DeleteDC(buffer);
    DeleteObject(hBmp);
	DeleteObject(hFont);
    ReleaseDC(window, device);
}
//==========================================================================================
//									��������������� �������
//==========================================================================================
float r(float a)		{ return (int)(a*1000)/float(1000); }
float r(float a, int p) { float t = 1; while(p--) t *= 10; return (int)(a*t)/t; }


#endif
