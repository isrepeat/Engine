#ifndef __GEOMETRY__
#define __GEOMETRY__

struct Triangle
{
	Vector a, b, c, N;
	Vector txA[5], txB[5], txC[5];	int NumberTexture;
	Triangle() {}
	Triangle(Vector A, Vector B, Vector C)	{ a = A; b = B; c = C; }

	Triangle& Texture(int n, int type_pos = 1, int flip = 0)
	{
		if(type_pos==1)		txA[0]={0,0}, txB[0]={1,0}, txC[0]={1,1};
		if(type_pos==2)		txA[0]={1,0}, txB[0]={1,1}, txC[0]={0,1};
		if(type_pos==3)		txA[0]={1,1}, txB[0]={0,1}, txC[0]={0,0};
		if(type_pos==4)		txA[0]={0,1}, txB[0]={0,0}, txC[0]={1,0};
		if(flip==0b110)		swap(txA[0], txB[0]);
		if(flip==0b011)		swap(txB[0], txC[0]);
		if(flip==0b101)		swap(txC[0], txA[0]);
		return  *this;
	}
	Triangle& Texture(int n, Vector txAA, Vector txBB, Vector txCC, int flip = 0)
	{
		txA[n] = txAA;
		txB[n] = txBB;
		txC[n] = txCC;
		if(flip==0b110)		swap(txA[n], txB[n]);
		if(flip==0b011)		swap(txB[n], txC[n]);
		if(flip==0b101)		swap(txC[n], txA[n]);
		return  *this;
	}
	Triangle operator ~ () { return N = !((b-a)^(c-a)), *this; }
}; 
struct Object
{
	Vector   Points   [100], points   [100], position, rotation;
	Triangle Triangles[100], triangles[100];
	Matrix   matRotationX,matRotationY,matRotationZ,matPosition;

	int cPoints, cTriangles;
	Object() 
	{
		cPoints = cTriangles = 0;
	}
};
//==========================================================================================
//									 Текстурирование
//==========================================================================================
//==========================================================================================
//									Создание объектов
//==========================================================================================
Object CreatePoint(Vector center = Vector(0, 0, 0))
{
	Object mesh;

	mesh.Points[mesh.cPoints++] = center;

	return mesh;
}
Object CreateTriangle(float w, float h, Vector center = Vector(0, 0, 0), int typePos = 1, int flip = 0)
{
	Object mesh;

	Vector a = center + Vector(-w/2, -h/2, 0);
	Vector b = center + Vector(+w/2, -h/2, 0);
	Vector c = center + Vector(+w/2, +h/2, 0);

	mesh.Points[mesh.cPoints++] = a;
	mesh.Points[mesh.cPoints++] = b;
	mesh.Points[mesh.cPoints++] = c;
	
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(a, b, c).Texture(0, {0,0},{1,0},{1,1})
														  .Texture(1, {255,0,0},{0,255,0},{0,0,255});														  					
	
	return mesh;
}
Object CreatePlane(float w, float h, Vector center = Vector(0, 0, 0))
{
	Object mesh;

	Vector a = center + Vector(-w/2, -h/2, 0),		tA = {0,0}, ttA = {255,0,0};
	Vector b = center + Vector(-w/2, +h/2, 0),		tB = {0,1}, ttB = {0,255,0};
	Vector c = center + Vector(+w/2, +h/2, 0),		tC = {1,1}, ttC = {0,0,255};
	Vector d = center + Vector(+w/2, -h/2, 0),		tD = {1,0}, ttD = {0,255,0};

	mesh.Points[mesh.cPoints++] = a;
	mesh.Points[mesh.cPoints++] = b;
	mesh.Points[mesh.cPoints++] = c;
	mesh.Points[mesh.cPoints++] = d;

	mesh.Triangles[mesh.cTriangles++] = ~Triangle(a, b, c).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(c, d, a).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	return mesh;
}
Object CreateBox(float w, float h,float l, Vector center = Vector(0, 0, 0))
{
	Object mesh;

	Vector p[8];
	p[0] = center + Vector(-w/2, -h/2, -l/2);	Vector	tA = {0,0}, ttA = {255,0,0};
	p[1] = center + Vector(-w/2, +h/2, -l/2);	Vector	tB = {0,1}, ttB = {0,255,0};
	p[2] = center + Vector(+w/2, +h/2, -l/2);	Vector	tC = {1,1}, ttC = {0,0,255};
	p[3] = center + Vector(+w/2, -h/2, -l/2);	Vector	tD = {1,0}, ttD = {0,255,0};
	p[4] = center + Vector(-w/2, -h/2, +l/2);
	p[5] = center + Vector(-w/2, +h/2, +l/2);
	p[6] = center + Vector(+w/2, +h/2, +l/2);
	p[7] = center + Vector(+w/2, -h/2, +l/2);

	mesh.Points[mesh.cPoints++] = p[0];
	mesh.Points[mesh.cPoints++] = p[1];
	mesh.Points[mesh.cPoints++] = p[2];
	mesh.Points[mesh.cPoints++] = p[3];
	mesh.Points[mesh.cPoints++] = p[4];
	mesh.Points[mesh.cPoints++] = p[5];
	mesh.Points[mesh.cPoints++] = p[6];
	mesh.Points[mesh.cPoints++] = p[7];

	// Front
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[0], p[1], p[2]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[2], p[3], p[0]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	// Left
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[4], p[5], p[1]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);	
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[1], p[0], p[4]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);
	
	// Back
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[7], p[6], p[5]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);		
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[5], p[4], p[7]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	// Right
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[3], p[2], p[6]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);	
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[6], p[7], p[3]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	// Top
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[1], p[5], p[6]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[6], p[2], p[1]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	// Bottom
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[4], p[0], p[3]).Texture(0, tA,tB,tC).Texture(1, ttA,ttB,ttC);
	mesh.Triangles[mesh.cTriangles++] = ~Triangle(p[3], p[7], p[4]).Texture(0, tC,tD,tA).Texture(1, ttC,ttD,ttA);

	return mesh;
}
//==========================================================================================
//									Модификация объектов
//==========================================================================================
void Position (Object *mesh, Vector pos)					{ mesh->position = pos; }
void Rotation (Object *mesh, Vector rot)					{ mesh->rotation = rot; }
void Rotation (Object *mesh, float rx, float ry, float rz)	{ mesh->rotation = { rx, ry, rz }; }
void Translate(Object *mesh, Vector pos)					{ mesh->position += pos; }

void ModifyObject(Object *mesh)
{
	Matrix matRotationX = matRX(mesh->rotation.x);	mesh->matRotationX = matRotationX;
	Matrix matRotationY = matRY(mesh->rotation.y);	mesh->matRotationY = matRotationY;
	Matrix matRotationZ = matRZ(mesh->rotation.z);	mesh->matRotationZ = matRotationZ;
	Matrix matPosition  = matPOS(mesh->position);	mesh->matPosition  = matPosition;
	Matrix mat = matRotationX * matRotationY * matRotationZ * matPosition * matProjectionViewPos;

	for(int i = 0; i<mesh->cPoints; i++) 
		mesh->points[i] = mesh->Points[i]*mat;

	for(int i = 0; i<mesh->cTriangles; i++)
		mesh->triangles[i]   = mesh->Triangles[i],
		mesh->triangles[i].a = mesh->triangles[i].a*mat,
		mesh->triangles[i].b = mesh->triangles[i].b*mat,
		mesh->triangles[i].c = mesh->triangles[i].c*mat;
}
//==========================================================================================
//										BoundBoxes
//==========================================================================================
void triAABB(iVector &MIN, iVector &MAX, iVector a, iVector b, iVector c)
{
	MIN = a;
	MAX = a;

	if(b.x < MIN.x) MIN.x = b.x;	if(b.x > MAX.x) MAX.x = b.x;	
	if(c.x < MIN.x) MIN.x = c.x;	if(c.x > MAX.x) MAX.x = c.x;	
	
	if(b.y < MIN.y) MIN.y = b.y;	if(b.y > MAX.y) MAX.y = b.y;
	if(c.y < MIN.y) MIN.y = c.y;	if(c.y > MAX.y) MAX.y = c.y;
}


#endif
