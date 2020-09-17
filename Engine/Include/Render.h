#ifndef __RENDER__
#define __RENDER__

//==========================================================================================
//									Рисование точки
//==========================================================================================
void DrawPoint(const iVector &p, Pixel pColor = pRED, int s = 0)
{	
	DWORD color = ~pColor;
	if(s==0) ScreenBuffer[p.x][p.y] = color;
	else
	for(int X = max(0, (p.x-s)); X<min((p.x+s), mmWIDTH);  X++)
	for(int Y = max(0, (p.y-s)); Y<min((p.y+s), mmHEIGHT); Y++)
			 ScreenBuffer[X][Y] = color, DepthBuffer[X][Y] = -1;
}
//==========================================================================================
//									Рисование линии
//==========================================================================================
void DrawLine(iVector a, iVector b, Pixel pColor = pRED)
{
	DWORD color = ~pColor;
	bool transpose = false;
	int width  = mmWIDTH;
	int height = mmHEIGHT;

	if(abs(a.y-b.y) > abs(a.x-b.x)) swap(a.x,a.y), swap(b.x,b.y), transpose = true, swap(width,height);
	if(b.x < a.x) swap(a,b);

	int dx = b.x - a.x;
	int dy = b.y - a.y;

	int k = abs(dy)*2;
	int error = 0;
	int dp = (b.y>a.y ? 1 : -1);	//delta pixel +1/-1
	int cp = a.y;					//calculate pixel

	for(int p = a.x; p<=b.x; p++)
	{
		if(p>=0 && p<width && cp>=0 && cp<height)
			if(!transpose)	ScreenBuffer[p][cp] = color, DepthBuffer[p][cp] = -1;
			else			ScreenBuffer[cp][p] = color, DepthBuffer[cp][p] = -1;

		if((error+=k) > dx) cp+=dp, error-=2*dx;		//k < 0.5  =>  dy/dx < 0.5 => 2*dy/dx < 1 => 2*dy < dx
	}
}
//==========================================================================================
//									Рисование треугольника
//==========================================================================================
Vector Barycentric(float a, float b, float d, float e, iVector PA, float ea_bd)
{
	float c = PA.x;				//u*(AB).x + v*(AC).x + (PA).x = 0   ==>   u*a + v*b + c = 0
	float f = PA.y;				//u*(AB).y + v*(AC).y + (PA).y = 0   ==>   u*d + v*e + f = 0
	float u = (b*f-c*e)/ea_bd;
	float v = (c*d-f*a)/ea_bd;
	return Vector(1-u-v, u, v);
}
void DrawTriangle(Triangle tri, Pixel pColor = pRED, bool color = false)
{
	iVector A = tri.a;		float Aww = tri.a.ww;
	iVector B = tri.b;		float Bww = tri.b.ww;
	iVector C = tri.c;		float Cww = tri.c.ww;
	iVector MIN, MAX;		triAABB(MIN, MAX, A, B, C);	
	if(MIN.x < 0)			MIN.x = 0;					if(MIN.x > mmWIDTH-1)	return;
	if(MAX.x > mmWIDTH-1)	MAX.x = mmWIDTH-1;			if(MAX.x < 0)			return;
	if(MIN.y < 0)			MIN.y = 0;					if(MIN.y > mmHEIGHT-1)	return;
	if(MAX.y > mmHEIGHT-1)	MAX.y = mmHEIGHT-1;			if(MAX.y < 0)			return;

	float a = (B-A).x,  b = (C-A).x;
	float d = (B-A).y,  e = (C-A).y;
	float ea_bd = e*a-b*d;								if(ea_bd == 0)			return;

	int numTEX = tri.NumberTexture;
	Vector txA = tri.txA[0];
	Vector txB = tri.txB[0];
	Vector txC = tri.txC[0];

	Vector crA = tri.txA[1];
	Vector crB = tri.txB[1];
	Vector crC = tri.txC[1];

	iVector P;
	for(P.x = MIN.x; P.x<=MAX.x; P.x++)
	for(P.y = MIN.y; P.y<=MAX.y; P.y++)
	{
		Vector bc = Barycentric(a,b,d,e,(A-P),ea_bd);	if(bc.x<0.0001 || bc.y<0 || bc.z<0) continue;	//0.001 чтобы хвостики не торчали в вершинах треугольника

		float depth = 1/(bc.x*Aww + bc.y*Bww + bc.z*Cww);	if(DepthBuffer[P.x][P.y]<depth) continue;
		float ta = bc.x*Aww * depth;
		float tb = bc.y*Bww * depth;
		float tc = bc.z*Cww * depth;
		DepthBuffer[P.x][P.y] = depth;

		Pixel Pix;
		
		Vector  Tex = txA*ta + txB*tb + txC*tc;
		int x = Tex.x*texture[numTEX].width;
		int y = Tex.y*texture[numTEX].height;
		Pix = texture[numTEX].pixels[x][y];

		//Pix = crA*ta + crB*tb + crC*tc;

		if(color) Pix = pColor;
		ScreenBuffer[P.x][P.y] = ~Pix;
	}
}
//==========================================================================================
//									Рисование объекта
//==========================================================================================
void DrawObject(Object *mesh, Pixel pColorTri = pGRAY, bool color = false)
{	
	for(int i = 0; i<mesh->cTriangles; i++)
	{
		Triangle tri = mesh->triangles[i];

		bool backA = 0, backB = 0, backC = 0;  int backs = 0;

		if(tri.a.z/tri.a.ww < -1)	backA = 1, backs++;
		if(tri.b.z/tri.b.ww < -1)	backB = 1, backs++;
		if(tri.c.z/tri.c.ww < -1)	backC = 1, backs++;
		
		if(backs==3) continue;
		
		if(backs==0)
		{
			DrawTriangle(tri, pRED, color);
			continue;
		}

		Vector Normal={ 0,0,1 }, Point={ 0,0,1 }; // Задаем плоскость отсечения (нормаль + точка)

		tri.a = tri.a * invmProjectionViewPos;
		tri.b = tri.b * invmProjectionViewPos;
		tri.c = tri.c * invmProjectionViewPos;

		Vector F[2],E[2], txF[2][5],txE[2][5]; int f=0, e=0;

		if(backA)	{ for(int t = 0; t<5; t++) txE[e][t] = tri.txA[t];  E[e++] = tri.a; } else
					{ for(int t = 0; t<5; t++) txF[f][t] = tri.txA[t];	F[f++] = tri.a; }

		if(backB)	{ for(int t = 0; t<5; t++) txE[e][t] = tri.txB[t];  E[e++] = tri.b; } else
					{ for(int t = 0; t<5; t++) txF[f][t] = tri.txB[t];	F[f++] = tri.b; }

		if(backC)	{ for(int t = 0; t<5; t++) txE[e][t] = tri.txC[t];  E[e++] = tri.c; } else
					{ for(int t = 0; t<5; t++) txF[f][t] = tri.txC[t];	F[f++] = tri.c; }


		if(backs==2)
		{
			Vector A=F[0], B=E[0], C=E[1];	// своего рода переназначаем вершины так, чтобы А была спереди а B и C сзади				
			float t1  = ((Point-A)*Normal) / ((B-A)*Normal);	Vector P1 = (1-t1)*A + (t1)*B;
			float t2  = ((Point-A)*Normal) / ((C-A)*Normal);	Vector P2 = (1-t2)*A + (t2)*C;
			
			A  = A  * matProjectionViewPos;
			P1 = P1 * matProjectionViewPos;
			P2 = P2 * matProjectionViewPos;

			Triangle triONE;	triONE = tri;	// мб нормаль на этапе отрисовки уже и не нужна?

			// интерполируем атрибуты и записуем их в переназначенные вершины так сказать
			triONE.a = A;	for(int n = 0; n<5; n++) triONE.txA[n] = txF[0][n];
			triONE.b = P1; 	for(int n = 0; n<5; n++) triONE.txB[n] = (1-t1)*txF[0][n] + (t1)*txE[0][n];
			triONE.c = P2;	for(int n = 0; n<5; n++) triONE.txC[n] = (1-t2)*txF[0][n] + (t2)*txE[1][n];
			
			DrawTriangle(triONE, pRED, color);
		}
		if(backs==1)
		{
			Vector A=F[0], B=F[1], C=E[0];	 // своего рода переназначаем вершины так, чтобы А и B были спереди а C сзади
			float t1  = ((Point-A)*Normal) / ((C-A)*Normal);	Vector P1 = (1-t1)*A + (t1)*C;
			float t2  = ((Point-B)*Normal) / ((C-B)*Normal);	Vector P2 = (1-t2)*B + (t2)*C;

			A  = A  * matProjectionViewPos;
			B  = B  * matProjectionViewPos;
			P1 = P1 * matProjectionViewPos;
			P2 = P2 * matProjectionViewPos;

			Triangle triONE, triTWO;	triONE = triTWO = tri;	// мб нормаль на этапе отрисовки уже и не нужна?

			// интерполируем атрибуты и записуем их в переназначенные вершины так сказать
			triONE.a = A;	for(int n = 0; n<5; n++) triONE.txA[n] = txF[0][n];
			triONE.b = P1; 	for(int n = 0; n<5; n++) triONE.txB[n] = (1-t1)*txF[0][n] + (t1)*txE[0][n];
			triONE.c = P2;	for(int n = 0; n<5; n++) triONE.txC[n] = (1-t2)*txF[1][n] + (t2)*txE[0][n];

			triTWO.a = A;	for(int n = 0; n<5; n++) triTWO.txA[n] = txF[0][n];
			triTWO.b = B; 	for(int n = 0; n<5; n++) triTWO.txB[n] = txF[1][n];
			triTWO.c = P2;	for(int n = 0; n<5; n++) triTWO.txC[n] = (1-t2)*txF[1][n] + (t2)*txE[0][n];

			DrawTriangle(triONE, pRED, color);
			DrawTriangle(triTWO, pRED, color);
		}			
	}
}


#endif