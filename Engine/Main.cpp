#include"Include\Engine.h"
#include<ddraw.h>
#pragma comment(lib, "ddraw.lib")
IDirectDraw* dd;

double startTimeALL = 0, dtTimeALL = 0;
float  Fi = 0;
bool   HELP = false;
int	   CONTROL = 2;

Object oTri, oPlane, oBox;

Vector angleFIX = Vector(pi/4,pi/4,0), angleFLOAT, maskROT = { 1,0,0 }, vAng;
Vector posFIX	= Vector(0,0,2.75),		posFLOAT = {0,0,10000};


void INIT() 
{	
	oTri	= CreateTriangle(2, 2);
	oPlane	= CreatePlane(2, 2);
	oBox	= CreateBox(2, 2, 2);

	float FovY = 90;
	float aspect  = float(mmHEIGHT)/mmWIDTH;
	matProjection = matProj(aspect,1,1000,FovY*toRAD);
	matViewPos	  = matView(mmWIDTH,mmHEIGHT);

	 matProjectionViewPos =   matProjection*matViewPos;
	invmProjectionViewPos = -(matProjection*matViewPos);

	if(DirectDrawCreate(0,&dd,0) != DD_OK) mmENDPROG = true;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	if(!InitInstance(hInstance)) return GetLastError();
	
	SetResolution(1000, 500);
	
	INIT();
    while(!mmENDPROG) 
	{
	startTimeALL = clock();

		// Вообще обработку сообщений нужно вынести в отдельный поток ...
		MSG msg;
		mmWHEEL = 0;
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		KeysState();
		MouseState();
		if(hitESC)	mmENDPROG = true;
		if(hitTAB)	CONTROL++, CONTROL %= 3;
		if(hitH)	HELP = !HELP;
		if(hitV)	mmVsync = !mmVsync;
		
		float fi = pi/18;	
		if(msFORW)	angleFIX -= Vector(0, fi, 0); //&maskROT;
		if(msBACK)	angleFIX += Vector(0, fi, 0); //&maskROT;
		angleFLOAT += (angleFIX-angleFLOAT)*0.03;
	
		float step = 0.5;
		if(ktSPACE)	posFIX += { 0, +step, 0 };
		if(ktCTRL)	posFIX += { 0, -step, 0 };
		if(ktA)		posFIX += { -step, 0, 0 };
		if(ktD)		posFIX += { +step, 0, 0 };
		if(ktW)		posFIX += { 0, 0, +step/2};
		if(ktS)		posFIX += { 0, 0, -step/2};
		posFLOAT += (posFIX-posFLOAT)*0.2;

		Fi += pi/400;	if(Fi >= pi*2) Fi = 0;	 
		angleFLOAT.x = cos(Fi);
		angleFLOAT.z = sin(Fi);

		if(CONTROL==0)
		{
			Rotation(&oTri, angleFLOAT);
			Position(&oTri, posFLOAT);
		}
		if(CONTROL==1)
		{
			Rotation(&oPlane, angleFLOAT);
			Position(&oPlane, posFLOAT);
		}
		if(CONTROL==2)
		{
			Rotation(&oBox, angleFLOAT);
			Position(&oBox, posFLOAT);
		}
		
		ClearPixels();

			if(CONTROL==0)
			{
				ModifyObject(&oTri);
				DrawObject(&oTri);
			}
			if(CONTROL==1)
			{
				ModifyObject(&oPlane);
				DrawObject(&oPlane);
			}
			if(CONTROL==2)
			{
				ModifyObject(&oBox);
				DrawObject(&oBox);
			}
			// ждем вертикального синхроимпульса:
			if(mmVsync) dd->WaitForVerticalBlank(DDWAITVB_BLOCKEND, 0);

		UpdatePixels();

		StreamText << "Press 'H' to show help";											Text(10, 10);
		if(HELP)
		{
		StreamText << " Vsync  = "<<(mmVsync?"on":"off");								Text(10, 20*3);
		StreamText << " Model  = "<<(CONTROL==0?"triangle":CONTROL==1?"plane":"box");	Text(10, 20*4);
		StreamText << "'Mouse wheel' - rotate model";									Text(10, 20*6);
		StreamText << "'W'     - move forward";											Text(10, 20*7);
		StreamText << "'A'     - move left";											Text(10, 20*8);
		StreamText << "'S'     - move backward";										Text(10, 20*9);
		StreamText << "'D'     - move right";											Text(10, 20*10);
		StreamText << "'Space' - move up";												Text(10, 20*11);
		StreamText << "'Ctrl'  - move down";											Text(10, 20*12);
		StreamText << "'Tab'   - change model";											Text(10, 20*13);
		StreamText << "'V'     - Vsync on/off";											Text(10, 20*14);
		}
		StreamText << "FPS = "<<int(1000/dtTimeALL);									Text(10, 470);

		ClearKeys();
		RenderScene();

	dtTimeALL = clock() - startTimeALL;	
    }
	dd->Release();
    ReleaseInstance();
	return 0;
}