#ifndef __TEXT__
#define __TEXT__

std::ostringstream	StreamText;

void Text(int x, int y, Pixel pColor=pRED, int tab = 5)
{
	RECT    TextRect = {(x), (y), mmWIDTH, (y+20)};			// 20 пикселей высота - при стандартном размере шрифта!!! 
	std::string text = StreamText.str();  StreamText.str("");
	SetTextColor(buffer, (pColor.b<<8*2 | pColor.g<<8*1 | pColor.r<<8*0));
    DrawText(buffer, text.c_str(), text.length(), &TextRect, DT_EXPANDTABS | DT_LEFT | DT_TABSTOP | tab<<8);
}


void PrintVector(sVector V, int x, int y, Pixel pColor=pRED, int tab = 2)
{
	StreamText<<"{";
	StreamText<<(V.x<0 ? "":" ")<<r(V.x,2); StreamText<<"\t";
	StreamText<<(V.y<0 ? "":" ")<<r(V.y,2); StreamText<<"\t";
	StreamText<<(V.z<0 ? "":" ")<<r(V.z,2); StreamText<<"\t";
	StreamText<<(V.w<0 ? "":" ")<<r(V.w,2);
	StreamText<<"}";
	Text(x, y, pColor, tab);
}
void PrintVector(Vector V, int x, int y, Pixel pColor=pRED, int tab = 2)
{
	StreamText<<"{";
	StreamText<<(V.x<0 ? "":" ")<<r(V.x,2); StreamText<<"\t";
	StreamText<<(V.y<0 ? "":" ")<<r(V.y,2); StreamText<<"\t";
	StreamText<<(V.z<0 ? "":" ")<<r(V.z,2);
	StreamText<<"}";
	Text(x, y, pColor, tab);
}
void PrintMatrix(Matrix M, int x, int y, Pixel pColor=pRED, int tab = 4)
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			StreamText<<(M[i][j]<0 ? "":" ")<<r(M[i][j],1);   if(j<3) StreamText<<"\t";
		}
		Text(x, y+20*i, pColor, tab);
	}
}
#endif
