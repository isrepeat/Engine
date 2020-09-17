#ifndef __VECTOR__
#define __VECTOR__

#define pi    3.141592
#define toRAD (pi/180)

template <class T>
void swap(T &a, T &b) { T aa = a;  a = b; b = aa; }



//==========================================================================================
//										Вектор
//==========================================================================================
struct Vector;
struct iVector;
struct sVector;

struct iVector
{
	int x, y;
	iVector()				{ x = 0; y = 0; }
	iVector(int X, int Y)	{ x = X; y = Y; }
	iVector(const Vector&);
};
struct Vector
{
	union  { float x, a; };
	union  { float y, b; };
	union  { float z, c; };
	union  { float ww;	 };			// ww теряется при выполнении операторов!!!

	Vector()									{ x = 0; y = 0; z = 0; ww = 0; }
	Vector(float X, float Y)					{ x = X; y = Y; z = 0; ww = 0; }
	Vector(float X, float Y, float Z)			{ x = X; y = Y; z = Z; ww = 0; }
	Vector(float X, float Y, float Z, float WW) { x = X; y = Y; z = Z; ww = WW; }
	Vector(const iVector&);
	Vector(const sVector&);
};	
struct sVector
{
	float x, y, z, w;
	sVector()										{ x = 0; y = 0; z = 0; w = 0; }
	sVector(float X, float Y, float Z, float W = 1) { x = X; y = Y; z = Z; w = W; }
	sVector(const Vector&);
};

//Операторы векторов:
//------------------------------------------------------------------------------------------------------------------------
iVector operator +  (iVector  A, iVector B)	{ return iVector(A.x+B.x, A.y+B.y);		}
iVector operator -  (iVector  A, iVector B)	{ return iVector(A.x-B.x, A.y-B.y);		}
iVector operator *  (iVector  A, float s )	{ return iVector(A.x*s, A.y*s);			}
iVector operator *  (float    s, iVector B)	{ return iVector(B.x*s, B.y*s);			}
float   operator *  (iVector  A, iVector B)	{ return A.x*B.x + A.y*B.y;				}
iVector operator /  (iVector  A, float s )	{ return iVector(A.x/s, A.y/s);			}
//------------------------------------------------------------------------------------------------------------------------
Vector operator +   (Vector  A, Vector B)	{ return Vector(A.x+B.x, A.y+B.y, A.z+B.z); }
Vector operator -   (Vector  A, Vector B)	{ return Vector(A.x-B.x, A.y-B.y, A.z-B.z); }
Vector operator +=  (Vector &A, Vector B)	{ return A = A+B; }
Vector operator -=  (Vector &A, Vector B)	{ return A = A-B; }
Vector operator +   (Vector  A)				{ return A; }
Vector operator -   (Vector  A)				{ return Vector(-A.x,-A.y,-A.z); }
Vector operator *   (Vector  A, float s )	{ return Vector(A.x*s, A.y*s, A.z*s); }
Vector operator *   (float   s, Vector B)	{ return Vector(B.x*s, B.y*s, B.z*s); }
Vector operator /   (Vector  A, float s )	{ return s!=0 ? Vector(A.x/s, A.y/s, A.z/s) : Vector(999999,999999,999999); }
float  operator *   (Vector  A, Vector B)	{ return A.x*B.x + A.y*B.y + A.z*B.z; }
Vector operator ^   (Vector  A, Vector B)	{ return Vector((A.y*B.z-A.z*B.y),(A.z*B.x-A.x*B.z),(A.x*B.y-A.y*B.x)); }
Vector operator &   (Vector  A, Vector B)	{ return Vector(A.x*B.x, A.y*B.y, A.z*B.z); }

float  operator ~   (Vector  A)				{ return sqrt(A.x*A.x + A.y*A.y + A.z*A.z); }
Vector operator !   (Vector  A)				{ return A/~A; }
//------------------------------------------------------------------------------------------------------------------------
//Конструкторы копирования векторов:
//----------------------------------
iVector::iVector(const Vector &V)
{
	//x = (int)(V.x + 0.0);
	//y = (int)(V.y + 0.0);
	x = (int)(V.x + 0.5);
	y = (int)(V.y + 0.5);
}
//----------------------------------
Vector::Vector(const iVector &V)
{
	x = V.x;
	y = V.y;
	z = 0;
}
Vector::Vector(const sVector &V)
{
	ww = 1/V.w;
	x  = V.x*ww;
	y  = V.y*ww;
	z  = V.z*ww;
}
//----------------------------------
sVector::sVector(const Vector &V)
{
	x = V.x;
	y = V.y;
	z = V.z;
	w = 1.0;
}
//--------------------------------
//==========================================================================================
//										Матрица
//==========================================================================================
struct Matrix
{
	float m[4][4];

	Matrix(float m11, float m12, float m13, float m14,
		   float m21, float m22, float m23, float m24,
		   float m31, float m32, float m33, float m34,
		   float m41, float m42, float m43, float m44)
	{
		m[0][0]=m11; m[0][1]=m12; m[0][2]=m13; m[0][3]=m14;
		m[1][0]=m21; m[1][1]=m22; m[1][2]=m23; m[1][3]=m24;
		m[2][0]=m31; m[2][1]=m32; m[2][2]=m33; m[2][3]=m34;
		m[3][0]=m41; m[3][1]=m42; m[3][2]=m43; m[3][3]=m44;
	}
	Matrix(int diag_elem = 1)
	{
		for(int i = 0; i<4; i++) for(int j = 0; j<4; j++) m[i][j] = i==j ? (diag_elem) : 0;
	}
	float (&operator [](int i))[4]
	{
		return m[i];
	}
};
//------------------------------------------------------------------------------------------
Matrix operator ! (Matrix M)
{
	for(int i = 0; i<4; i++) for(int j = i; j<4; j++) if(i!=j) swap(M[i][j], M[j][i]);
	return M;
}
Matrix operator * (Matrix A, Matrix B)	
{
	Matrix C = {0};
	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
			for(int k = 0; k<4; k++)
				C.m[i][j] += A[i][k]*B[k][j];
	return C;
}
sVector operator * (sVector V, Matrix M)	
{
	sVector R;
	R.x = V.x*M[0][0] + V.y*M[1][0] + V.z*M[2][0] + V.w*M[3][0];
	R.y = V.x*M[0][1] + V.y*M[1][1] + V.z*M[2][1] + V.w*M[3][1];
	R.z = V.x*M[0][2] + V.y*M[1][2] + V.z*M[2][2] + V.w*M[3][2];
	R.w = V.x*M[0][3] + V.y*M[1][3] + V.z*M[2][3] + V.w*M[3][3];
	return R;
}
Matrix operator - (Matrix A)
{
	Matrix E;	int N = 4; float temp;
    
	for (int k = 0; k < N; k++)
    {
        temp = A[k][k];
        for (int j = 0; j < N; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }
        for (int i = k + 1; i < N; i++)
        {
            temp = A[i][k];
            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int k = N - 1; k > 0; k--)
    for (int i = k - 1; i >= 0; i--)
    {
        temp = A[i][k];
        for (int j = 0; j < N; j++)
        {
            A[i][j] -= A[k][j] * temp;
            E[i][j] -= E[k][j] * temp;
        }
    }

	return E;
}
//------------------------------------------------------------------------------------------
//==========================================================================================
//									Функции создания матриц
//==========================================================================================
Matrix matPOS(Vector pos)
{
	Matrix R;
	R[0][0] = 1;		R[0][1] = 0;		R[0][2] = 0;		R[0][3] = 0;
	R[1][0] = 0;		R[1][1] = 1;		R[1][2] = 0;		R[1][3] = 0;
	R[2][0] = 0;		R[2][1] = 0;		R[2][2] = 1;		R[2][3] = 0;
	R[3][0] = pos.x;	R[3][1] = pos.y;	R[3][2] = pos.z;	R[3][3] = 1;
	return R;
}
Matrix matRX(float fi)
{
	Matrix R;
	R[0][0] = 1;		R[0][1] = 0;		R[0][2] = 0;		R[0][3] = 0;
	R[1][0] = 0;		R[1][1] = cos(fi);	R[1][2] =-sin(fi);	R[1][3] = 0;
	R[2][0] = 0;		R[2][1] = sin(fi);	R[2][2] = cos(fi);	R[2][3] = 0;
	R[3][0] = 0;		R[3][1] = 0;		R[3][2] = 0;		R[3][3] = 1;
	return R;
}
Matrix matRY(float fi)
{
	Matrix R;
	R[0][0] = cos(fi);	R[0][1] = 0;		R[0][2] = sin(fi);	R[0][3] = 0;
	R[1][0] = 0;		R[1][1] = 1;		R[1][2] =0;			R[1][3] = 0;
	R[2][0] = -sin(fi);	R[2][1] = 0;		R[2][2] = cos(fi);	R[2][3] = 0;
	R[3][0] = 0;		R[3][1] = 0;		R[3][2] = 0;		R[3][3] = 1;
	return R;
}
Matrix matRZ(float fi)
{
	Matrix R;
	R[0][0] =  cos(fi);	R[0][1] =-sin(fi);	R[0][2] = 0;		R[0][3] = 0;
	R[1][0] =  sin(fi);	R[1][1] = cos(fi);	R[1][2] = 0;		R[1][3] = 0;
	R[2][0] = 0;		R[2][1] = 0;		R[2][2] = 1;		R[2][3] = 0;
	R[3][0] = 0;		R[3][1] = 0;		R[3][2] = 0;		R[3][3] = 1;
	return R;
}
Matrix matProj(float aspect, float n, float f, float fovY)
{
	//	d -- z-координата  плоскости проецирования 
	//	c -- расстояние от плоскости проецирования до фокуса камеры
	//w(z)	= z*a + b  ==>	w(z=d)	 =     d*a + b = 1	==>	 a = 1/c,  b = 1 - d/c
	//						w(z=d-c) = (d-c)*a + b = 0  (осторожно деление на 0!!!)
	//								по сути решением такой системы мы добиваемся чтобы на расстоянии d (объекта) небыло  искажения, но
	//								это не поможет избежать отрицательного w(z) при делении => своего рода происходит перенос в начало
	//
	float w = 1/aspect;			//  полуширина  плоскости проецирования в условных единицах
	float h = 1;				//  полувысота  плоскости проецирования в условных единицах
	float d = n;				//z-координата  плоскости проецирования (где нет искажения перспективы так сказать)
	float c = 1/tan(fovY/2);	//расстояние от плоскости проецирования до фокуса камеры выраженное через fovY
	Matrix R;												
	R[0][0] = 1/w;				R[0][1] = 0;				R[0][2] = 0;				R[0][3] = 0;
	R[1][0] = 0;				R[1][1] = 1/h;				R[1][2] = 0;				R[1][3] = 0;
	R[2][0] = 0;				R[2][1] = 0;				R[2][2] = (f+n)/(f-n);		R[2][3] = 1/c;	 
	R[3][0] = 0;				R[3][1] = 0;				R[3][2] = -2*f*n/(f-n);		R[3][3] = 1-d/c;
	return R;
}
Matrix matProj0(float aspect, float n, float f, float fovY)
{
	//	d -- z-координата  плоскости проецирования 
	//	c -- расстояние от плоскости проецирования до фокуса камеры
	//w(z)	= z*a + b  ==>	w(z=d)	 =     d*a + b = 1	==>	 a = 1/c,  b = 1 - d/c
	//						w(z=d-c) = (d-c)*a + b = 0  (осторожно деление на 0!!!)
	//								по сути решением такой системы мы добиваемся чтобы на расстоянии d (объекта) небыло искажения,
	//								но это не поможет избежать отрицательного w(z) при делении => своего рода происходит перенос в начало
	//
	float w = 1/aspect;			//  полуширина  плоскости проецирования в условных единицах
	float h = 1;				//  полувысота  плоскости проецирования в условных единицах
	//float d = n;				//z-координата  плоскости проецирования (где нет искажения перспективы так сказать)
	//float c = 1/tan(fovY/2);	//расстояние от плоскости проецирования до фокуса камеры выраженное через fovY
	Matrix R;												
	R[0][0] = 1/w;				R[0][1] = 0;				R[0][2] = 0;				R[0][3] = 0;
	R[1][0] = 0;				R[1][1] = 1/h;				R[1][2] = 0;				R[1][3] = 0;
	R[2][0] = 0;				R[2][1] = 0;				R[2][2] = 1;				R[2][3] = 1;	 
	R[3][0] = 0;				R[3][1] = 0;				R[3][2] = 0;				R[3][3] = 0;
	return R;
}
Matrix matView(float w, float h)// w -- ширина экрана в пикселях
{								// h -- высота экрана в пикселях
	Matrix R;
	R[0][0] = w/2;				R[0][1] = 0;				R[0][2] = 0;				R[0][3] = 0;
	R[1][0] = 0;				R[1][1] = h/2;				R[1][2] = 0;				R[1][3] = 0;
	R[2][0] = 0;				R[2][1] = 0;				R[2][2] = 1;/*0.5*/			R[2][3] = 0;
	R[3][0] = w/2;				R[3][1] = h/2;				R[3][2] = 0;/*0.5*/			R[3][3] = 1;
	return R;
}

Matrix matScreen;
Matrix matProjection,matViewPos, matProjectionViewPos,invmProjectionViewPos;


#endif
