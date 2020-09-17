#ifndef __PIXEL__
#define __PIXEL__

//==========================================================================================
//										Пиксель
//==========================================================================================
struct Pixel
{
	int r, g, b;
	Pixel() {}
	Pixel(int RED, int GREEN, int BLUE) { r = RED; g = GREEN; b = BLUE; }
	Pixel(const Vector&);
};
Pixel operator + (Pixel pixA, Pixel pixB)	{ return Pixel(pixA.r+pixB.r, pixA.g+pixB.g, pixA.b+pixB.b); }
Pixel operator * (Pixel color, float s)		{ return Pixel(color.r*s,     color.g*s,	 color.b*s); }
DWORD operator ~ (Pixel color)				{ return color.r << 8*2 | color.g << 8*1 | color.b << 8*0; }

Pixel::Pixel(const Vector &A)
{
	r = (int)(A.a + 0.5);
	g = (int)(A.b + 0.5);
	b = (int)(A.c + 0.5);
}

const Pixel pBLACK	= Pixel(0, 0, 0);
const Pixel pGRAY	= Pixel(120, 120, 120);
const Pixel pWHITE	= Pixel(255, 255, 255);
const Pixel pRED	= Pixel(255, 0, 0);
const Pixel pGREEN	= Pixel(0, 255, 0);
const Pixel pBLUE	= Pixel(0, 0, 255);
const Pixel pYELLOW	= Pixel(255, 255, 0);

const Pixel pPIG	= Pixel(255, 203, 219);
//==========================================================================================
//										Текстура
//==========================================================================================
struct Texture
{
	Pixel pixels[2000][2000];
	int width, height;
};
Texture texture[10];			// сюда заносятся все текстуры используемые в движке


#endif