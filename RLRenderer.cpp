#include "stdafx.h"
#include <math.h>
#include <windows.h>//This is a botch include to prevent inclusion of gl.h throwing missing definition errors

#include "RLRenderer.h"
#include "RLUtilities.h"

using RustyLib::Utils::DegToRad;
using RustyLib::Utils::Lerp;
using RustyLib::Graphics::RLRenderer;
using RustyLib::Graphics::ClipMode;
using RustyLib::Graphics::RenderMode;
using RustyLib::Graphics::LineAlgorithm;
using RustyLib::Graphics::TriAlgorithm;

const float RLRenderer::AlphaScalar = 0.003921568627451f;

RLRenderer::RLRenderer(void):
	INSIDE(0),//0x0000
	ABOVE (1),//0x0001
	BELOW (2),//0x0010
	LEFT  (4),//0x0100
	RIGHT (8),//0x1000
	currFont(GLUT_BITMAP_TIMES_ROMAN_24)
{
	this->SetPointSize(1);
	this->SetLineColour(1.0f,0.0f,0.0f);
	this->currentClipMode = ClipMode::Basic;
	this->currentRenderMode = RenderMode::SolidColour;
	this->currentLineAlgorithm = LineAlgorithm::Bresenhams;
	this->currentTriAlgorithm = TriAlgorithm::MyOwn;
}

RLRenderer::~RLRenderer(void)
{
	delete(clipRegionMin);
	delete(clipRegionMax);
	delete(lineColour);
}

RLRenderer& RLRenderer::Instance(void )
{
	static RLRenderer _instance;

	return _instance;
}

const ClipMode RLRenderer::GetClipMode(void )const
{
	return currentClipMode;
}

void RLRenderer::SetClipMode(ClipMode clipMode)
{
	currentClipMode = clipMode;
}

void RLRenderer::SetClippingRegion(const Vector3& min, const Vector3& max)
{
	this->clipRegionMin = new Vector3(min);
	this->clipRegionMax = new Vector3(max);
}

void RLRenderer::SetClippingRegion(int left, int top, int right, int bottom)
{
	this->clipRegionMin = new Vector3(left, top, 0.0f);
	this->clipRegionMax = new Vector3(right, bottom, 0.0f);
}

void RLRenderer::RenderClipRegion(void )
{
	RenderRect(*clipRegionMin, *clipRegionMax);
}

void RLRenderer::SetPointSize(unsigned int size)
{
	pointSize = size;
	glPointSize(pointSize);
}

unsigned int RLRenderer::GetPointSize(void )const
{
	return pointSize;
}

Vector3 const& RLRenderer::GetLineColour(void )const
{
	if(lineColour)
	{
		return *lineColour;
	}
	else
	{
		return 0;
	}
}

void RLRenderer::SetLineColour(const Vector3& colour)
{
	lineColour = new Vector3(colour);
	glColor3f(lineColour->X(),lineColour->Y(),lineColour->Z());
}

void RLRenderer::SetLineColour(float r, float g, float b, float a)
{
	lineColour = new Vector3(r,g,b);
	SetAlpha(a);
}

void RLRenderer::SetRenderMode(RenderMode renderMode)
{
	this->currentRenderMode = renderMode;
}

const RenderMode RLRenderer::GetRenderMode(void )const
{
	return this->currentRenderMode;
}

void RLRenderer::SetLineAlgorithm(LineAlgorithm algorithm)
{
	this->currentLineAlgorithm = algorithm;
}

const LineAlgorithm RLRenderer::GetLineAlgorithm(void )const
{
	return this->currentLineAlgorithm;
}

void RLRenderer::SetTriAlgorithm(TriAlgorithm algorithm)
{
	this->currentTriAlgorithm = algorithm;
}

const TriAlgorithm RLRenderer::GetTriAlgorithm(void )const
{
	return this->currentTriAlgorithm;
}

inline void RLRenderer::SetAlpha(const float a)
{
	glColor4f(lineColour->X(),lineColour->Y(),lineColour->Z(),a);
}

inline void RLRenderer::PlotPixel(int x, int y)
{
	glVertex2i(x,y);
}

inline void RLRenderer::PlotPixel(int x, int y, int alpha)
{
	//glVertex2i
}

int RLRenderer::RenderPoint(const Vector3& p)
{
	if(currentClipMode)
	{
		if(!ClipPoint(p))
		{
			PlotPixel(p.x, p.y);
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		PlotPixel(p.x, p.y);
		return 0;
	}
}

int RLRenderer::RenderLine(const Vector3& p1, const Vector3& p2)
{
	SetAlpha(1.0f);
	if(currentLineAlgorithm == LineAlgorithm::DirectScan)
	{
		DirectScan(p1, p2);
	}
	else if(currentLineAlgorithm == LineAlgorithm::DDA)
	{
		DDA(p1, p2);
	}
	else if(currentLineAlgorithm == LineAlgorithm::Bresenhams)
	{
		Bresenhams(p1, p2);
	}
	else if(currentLineAlgorithm == LineAlgorithm::AndyBresenhams)
	{
		AndyBresenhams(p1, p2);
	}
	else if(currentLineAlgorithm == LineAlgorithm::Wu)
	{
		Wu(p1, p2);
	}
	return 0;
}

void RLRenderer::DirectScan(const Vector3& p1, const Vector3& p2)
{
	/*Direct Scan Conversion:
	We begin at the left most (lowest X value) point
	and interpolate toward the second point
	
	This is because the algorithm operates on two basis
	of +/- on Y but always + on X*/

	//Vars to hold the left most and right most X points
	float minX;
	float maxX;

	//Determine the correct X boundaries
	if(p1.X() < p2.X())
	{
		minX = p1.X();
		maxX = p2.X();
	}
	else
	{
		minX = p2.X();
		maxX = p1.X();
	}

	//The gradient of the line (left to right)
	float gradient = Vector3::Get2DGradient(p1, p2);

	float yIntercept = p1.Y() - gradient * p1.X();

	glBegin(GL_POINTS);

	for (int currX = minX; currX < maxX; currX++)
	{
		PlotPixel(currX, (gradient * currX) + yIntercept);
	}

	glEnd();
}

void RLRenderer::DDA(const Vector3& p1, const Vector3& p2)
{
	const Vector3* leftMost;
	const Vector3* rightMost;
	if(p1.X() < p2.X())
	{
		leftMost = &p1;
		rightMost = &p2;
	}
	else
	{
		leftMost = &p2;
		rightMost = &p1;
	}
	//Select (lowest-X) point
	float gradient = Vector3::Get2DGradient(*leftMost, *rightMost);

	if(gradient <= 1.0f)
	{
		//We increment this by the gradient for
		//each point along X
		float currY = leftMost->Y();

		glBegin(GL_POINTS);

		for(int currX = leftMost->X(); currX <= rightMost->X(); currX++)
		{
			PlotPixel(currX, currY);
			currY += gradient;
		}

		glEnd();
	}
	else
	{
		//We increment this by the gradient for
		//each point along Y
		float currX = leftMost->X();

		glBegin(GL_POINTS);

		for(int currY = leftMost->Y(); currY <= rightMost->Y(); currY++)
		{
			PlotPixel(currX, currY);
			currX += 1 / gradient;
		}

		glEnd();
	}
}

void RLRenderer::Bresenhams(const Vector3& p1, const Vector3& p2)
{
	//Taken from wikipedias optimised algorithm
	int x0 = p1.X(), y0 = p1.Y();
	int x1 = p2.X(), y1 = p2.Y();

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if(steep)
	{
		//If the angle is steep we change the 
		//axis we iterate along (the major axis)
		int t0 = x0;
		x0 = y0;
		y0 = t0;
		int t1 = x1;
		x1 = y1;
		y1 = t1;
	}

	if(x0 > x1)
	{
		//If we are working with a negative gradient along the major axis
		int tX = x0;
		x0 = x1;
		x1 = tX;
		int tY = y0;
		y0 = y1;
		y1 = tY;
	}

	int deltaX = x1 - x0;
	int deltaY = abs(y1 - y0);
	int error = deltaX / 2;
	int yStep;
	int y = y0;
	if(y0 < y1)
	{
		yStep = 1;
	}
	else
	{
		yStep = -1;
	}

	glBegin(GL_POINTS);

	for(int x = x0; x < x1; x++)
	{
		if(steep)
		{
			PlotPixel(y, x);
		}
		else
		{
			PlotPixel(x, y);
		}
		error -= deltaY;
		if(error < 0)
		{
			y += yStep;
			error += deltaX;
		}
	}

	glEnd();
}

void RLRenderer::AndyBresenhams(const Vector3& p1, const Vector3& p2)
{
	int x0 = p1.X(), y0 = p1.Y();
	int x1 = p2.X(), y1 = p2.Y();

	int deltaX = abs(x1 - x0);
	int deltaY = abs(y1 - y0);

	int p = (2 * deltaY) - deltaX;

	int xStep = 2 * deltaY;
	int yStep = xStep - (2 * deltaX);

	int x = x0;
	int y = y0;

	glBegin(GL_POINTS);

	glVertex2f(x, y);

	while(x < x1)
	{
		if(p <= 0)
		{
			p += xStep;
			x++;
		}
		else
		{
			p += yStep;
			x++;
			y++;
		}
		glVertex2f(x, y);
	}

	glEnd();
}

void RLRenderer::Wu(const Vector3& p1, const Vector3& p2)
{
	/*int x0, y0, x1, y1;
	if(p1.X() < p2.X())
	{
		x0 = p1.X(); y0 = p1.Y();
		x1 = p2.X(); y1 = p2.Y();
	}
	else
	{
		x0 = p2.X(); y0 = p2.Y();
		x1 = p1.X(); y0 = p1.Y();
	}*/

	int x0 = p1.X(), y0 = p1.Y();
	int x1 = p2.X(), y1 = p2.Y();

	//Make sure we are traversing from top to bottom
	if(y0 > y1)
	{
		int tX = x0;
		x0 = x1;
		x1 = tX;
		int tY = y0;
		y0 = y1;
		y1 = tY;
	}

	//Draw the first pixel as (assuming 1px thickness) it wont need
	//weighting
	glBegin(GL_POINTS);

	glVertex2i(x0, y0);

	//We ensure we travel positive on X
	int DeltaX, XDir;
	if((DeltaX = x1 - x0) >= 0)
	{
		XDir = 1;
	}
	else
	{
		XDir = -1;
		DeltaX = 0-DeltaX;//Make DeltaX positive
	}

	//If we have perfect straight or diagonal lines
	//thus no weighting necessary
	int DeltaY = y1 - y0;
	if(DeltaY == 0)
	{
		//Horixontal Line
		while(DeltaX-- != 0)
		{
			x0 += XDir;
			glVertex2i(x0, y0);
		}
		return;
	}
	if(DeltaX == 0)
	{
		//Vertical Line
		do
		{
			y0++;
			glVertex2i(x0, y0);
		}while(--DeltaY != 0);
		return;
	}
	if(DeltaX == DeltaY)
	{
		//Diagonal Line
		do
		{
			x0 += XDir;
			y0++;
			glVertex2i(x0, y0);
		}while(--DeltaY != 0);
		glEnd();
		return;
	}

	//Line is not a special case and must be calculated
	//Error accumulation
	unsigned short ErrorAdj;
	unsigned short ErrorAccTemp, Weighting;

	unsigned short ErrorAcc = 0;//Error accumulator  = 0

	//Is the line X major or Y major?
	if(DeltaY > DeltaX)
	{
		//Y major
		//Calculate the 16bit fp that will increment X
		//each time Y increments by 1, truncating the result so we dont ovverun endpoint
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		//Draw all pixels between the endpoints
		while(--DeltaY)
		{
			ErrorAccTemp = ErrorAcc;//Remember current error accumulation
			ErrorAcc += ErrorAdj;//calcaulate the error to the next pixel
			if(ErrorAcc <= ErrorAccTemp)
			{
				//The error has accumulated sufficiently
				x0 += XDir;
			}
			y0++;//Increment Y

			//Calculate the intensity from the accumulation of error
			//and also the intensity of the pixels pair
			Weighting = ErrorAcc >> 8;

			//Pixel lying closest to line
			SetAlpha( (255 - Weighting) * AlphaScalar);
			glVertex2i(x0, y0);

			//Adjacent pixel
			SetAlpha(Weighting * AlphaScalar);
			glVertex2i(x0 + XDir, y0);
		}
		//Set intensity to normal
		//Draw the end pixel
		SetAlpha(1.0f);
		glVertex2i(x1, y1);
		glEnd();
		return;
	}
	//Else it is an X major line
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	while (--DeltaX)
	{
		ErrorAccTemp = ErrorAcc;
		ErrorAcc += ErrorAdj;
		if(ErrorAcc <= ErrorAccTemp)
		{
			y0++;
		}
		x0 += XDir;

		Weighting = ErrorAcc >> 8;

		//Pixel lying closest to line
		SetAlpha( (255 - Weighting) * AlphaScalar);
		glVertex2i(x0,y0);

		//Adjacent pixel
		SetAlpha(Weighting * AlphaScalar);
		glVertex2i(x0, y0 + 1);
	}
	//Draw the end pixel which will be sat on the line end perfectly
	//and so needs no weighting
	SetAlpha(1.0f);
	glVertex2i(x1, y1);//plus BaseColor

	glEnd();

	return;
}

int RLRenderer::RenderRect(int minX, int minY, int maxX, int maxY)
{
	if(minX > clipRegionMax->X() ||  //
		maxX < clipRegionMin->X() || //Circumstances under which we do not 
		minY > clipRegionMax->Y() || //rasterise the rectangle
		maxY < clipRegionMin->Y())   //
	{
		return 1;
	}
	else
	{
		//top-left -> top-right
		this->RenderLine(
			Vector3(minX, minY, 0.0f),
			Vector3(maxX, minY, 0.0f));
		//top-right -> bottom-right
		this->RenderLine(
			Vector3(maxX, minY, 0.0f),
			Vector3(maxX, maxY, 0.0f));
		//bottom-left -> bottom-right
		this->RenderLine(
			Vector3(minX, maxY, 0.0f),
			Vector3(maxX, maxY, 0.0f));
		//top-left -> bottom-left
		this->RenderLine(
			Vector3(minX, minY, 0.0f),
			Vector3(minX, maxY, 0.0f));
	}
}

int RLRenderer::RenderRect(const Vector3& min, const Vector3& max)
{
	return (this->RenderRect(min.x, min.y, max.x, max.y));
}

int RLRenderer::RenderTri(const Tri& tri)
{
	if(this->currentRenderMode == RenderMode::SolidColour)
	{
		if(currentClipMode)
		{
			if(!ClipTri(tri))
			{
				switch(currentTriAlgorithm)
				{
				case TriAlgorithm::DevMasterNick:
					RenderTriDevMasterNick(tri);
					break;
				case TriAlgorithm::FoleyVanDam:
					RenderTriFoleyVanDam(tri);
					break;
				case TriAlgorithm::MyOwn:
					RenderTriMyOwn((Tri&)tri);
					break;
				case TriAlgorithm::Take2:
					RenderTriTake2((Tri&)tri);
					break;
				default:
					RenderTriDevMasterNick(tri);
					break;
				}
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			switch(currentTriAlgorithm)
			{
			case TriAlgorithm::DevMasterNick:
				RenderTriDevMasterNick(tri);
				break;
			case TriAlgorithm::FoleyVanDam:
				RenderTriFoleyVanDam(tri);
				break;
			case TriAlgorithm::MyOwn:
				RenderTriMyOwn((Tri&)tri);
				break;
			case TriAlgorithm::Take2:
				RenderTriTake2((Tri&)tri);
				break;
			default:
				RenderTriDevMasterNick(tri);
				break;
			}
			return 0;
		}
	}
	else if(this->currentRenderMode == RenderMode::Wireframe)
	{
		//Draw the edges only
		if(!ClipTri(tri))
		{
			RenderTriWire(tri);
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

int RLRenderer::RenderTri(const Vector3* verts)
{
	Tri tri;
	try
	{		
		tri.verts[0] = verts[0];
		tri.verts[1] = verts[1];
		tri.verts[2] = verts[2];
	}
	catch(std::exception e)
	{
		return 1;
	}

	return RenderTri(tri);
}

int RLRenderer::RenderTri(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Tri tri;
	tri.verts[0] = v1;
	tri.verts[1] = v2;
	tri.verts[2] = v3;

	return RenderTri(tri);
}

bool RLRenderer::ClipTri(const Tri& tri)
{
	bool offScreen = true;

	if(!ClipPoint(tri.verts[0]))
	{
		offScreen = false;
	}
	else if(!ClipPoint(tri.verts[1]))
	{
		offScreen = false;
	}
	else if(!ClipPoint(tri.verts[2]))
	{
		offScreen = false;
	}

	return offScreen;
}

bool RLRenderer::ClipPoint(const Vector3& p)
{
	bool offScreen = true;

	//Do code
	if( (p.x < clipRegionMax->x) && (p.x > clipRegionMin->x) )
	{
		if( (p.y < clipRegionMax->y) && (p.y > clipRegionMin->y) )
		{
			offScreen = false;
		}
	}

	return offScreen;
}

char RLRenderer::CohenSutherlandCode(const Vector3& p)
{
	//code = 0x0000
	char code = INSIDE;

	//Assign the appropriate region codes
	if(p.x < clipRegionMin->x)
	{
		code |= LEFT;
	}
	else if(p.x > clipRegionMax->x)
	{
		code |= RIGHT;
	}
	if(p.y < clipRegionMin->y)
	{
		code |= ABOVE;
	}
	else if(p.y > clipRegionMax->y)
	{
		code |= BELOW;
	}

	return code;
}

void RLRenderer::RenderTriWire(const Tri& tri)
{
	if(currentClipMode == ClipMode::CohenSutherland)
	{
		this->CohenSutherland(tri.verts[0], tri.verts[1]);
		this->CohenSutherland(tri.verts[1], tri.verts[2]);
		this->CohenSutherland(tri.verts[2], tri.verts[0]);
	}
	else
	{
		this->RenderLine(tri.verts[0], tri.verts[1]);
		this->RenderLine(tri.verts[1], tri.verts[2]);
		this->RenderLine(tri.verts[2], tri.verts[0]);
	}
}

void RLRenderer::RenderTriMyOwn(Tri& tri)
{
	//Temp array for sorting vertices
	Vector3* verts[3];
	//Temp pointer for sorting
	Vector3* temp = 0;
	//xMin and xMax for span conversion
	int startX = 0, endX = 0;
	//vars for precalculation before scan loops
	float v0x = 0.0f, v0y = 0.0f;

	//Assign verts to temp array
	verts[0] = &tri.verts[0];
	verts[1] = &tri.verts[1];
	verts[2] = &tri.verts[2];

	//Sort vertices by ascending y value
	if (verts[0]->y > verts[1]->y)
	{
		temp = verts[0];
		verts[0] = verts[1];
		verts[1] = temp;
	}
	if (verts[1]->y > verts[2]->y)
	{
		temp = verts[1];
		verts[1] = verts[2];
		verts[2] = temp;
		if (verts[0]->y > verts[1]->y)
		{
			temp = verts[0];
			verts[0] = verts[1];
			verts[1] = temp;
		}
	}
	if (verts[0]->y > verts[2]->y)
	{
		temp = verts[0];
		verts[0] = verts[2];
		verts[2] = temp;
	}
	/********Verts sorted********/

	
	/********Convert scan lines********/	
	//Set up precalculable values for upper tri portion
	float leftSlope = 0.0f, rightSlope = 0.0f;
	Vector3 *leftVert, *rightVert;

	/****Make checks to ensure scan lines go from left to right***/
	//If the next vert along Y lies to the left of the startPoint
	if(verts[1]->x <= verts[2]->x)
	{
		leftVert = verts[1];
		rightVert = verts[2];
	}
	else
	{
		leftVert = verts[2];
		rightVert = verts[1];
	}

	//Now the left hand scanline is ascertained
	//We can calculate the slope of each descending line as below
	//
	//			/.\
	//leftSlope/   \rightSlope
	//		  v		v
	//
	leftSlope = Vector3::Get2DGradient(*verts[0], *leftVert);
	rightSlope = Vector3::Get2DGradient(*verts[0], *rightVert);

	//Now we know the gradients of either boundary scan lines
	//we can compute and plot the spans
	v0x = verts[0]->x;
	v0y = verts[0]->y;
	int v1y = verts[1]->y;
	glBegin(GL_POINTS);//Don't forget to tell GL you're gonna be doing this!
	for(int currY = v0y; currY < v1y; currY++)
	{
		float dY = currY - v0y;
		startX = v0x + leftSlope * dY;
		endX = v0x + rightSlope * dY;
		PlotScanLine(startX, endX, currY);
	}/********Completed Upper portion of tri***********/

	/*******Convert lower portion of tri**********/
	//Check that lowest verts don't share the same scan line
	if(verts[2]->y != verts[1]->y)
	{
		//They are on different scan lines
		//and need to be rasterised

		//Check which verts correspond to each side of the tri
		if(verts[1]->x > verts[0]->x)
		{
			leftVert = verts[0];
			rightVert = verts[1];
		}
		else
		{
			leftVert = verts[1];
			rightVert = verts[0];
		}

		//Verts are now sorted, calculate the slopes as below
		//
		//		  ^	    ^
		//leftSlope\   /rightSlope
		//			\ /
		//			 *
		//
		leftSlope = Vector3::Get2DGradient(*leftVert, *verts[2]);
		rightSlope = Vector3::Get2DGradient(*rightVert, *verts[2]);

		//Now we know the gradients of either boundary scan lines
		//we can compute and plot the spans
		float lVx = leftVert->x;
		float lVy = leftVert->y;
		float rVx = rightVert->x;
		float rVy = rightVert->y;
		int v2y = verts[2]->y;
		for(int currY = verts[1]->y; currY < v2y; currY++)
		{
			float leftDy = currY - lVy;
			float rightDy = currY - rVy;
			startX = lVx + leftSlope * leftDy;
			endX = rVx + rightSlope * rightDy;
			PlotScanLine(startX, endX, currY);
		}/********Completed Lower portion of tri***********/
	}
	glEnd();//Don't forget to tell GL you're done drawing
}

void RLRenderer::RenderTriTake2(Tri& tri)
{
	//Temp array for sorting vertices
	Vector3* verts[3];
	//Temp pointer for sorting
	Vector3* temp = 0;

	//Assign verts to temp array
	verts[0] = &tri.verts[0];
	verts[1] = &tri.verts[1];
	verts[2] = &tri.verts[2];

	//Sort vertices by ascending y value
	if (verts[0]->y > verts[1]->y)
	{
		temp = verts[0];
		verts[0] = verts[1];
		verts[1] = temp;
	}
	if (verts[1]->y > verts[2]->y)
	{
		temp = verts[1];
		verts[1] = verts[2];
		verts[2] = temp;
		if (verts[0]->y > verts[1]->y)
		{
			temp = verts[0];
			verts[0] = verts[1];
			verts[1] = temp;
		}
	}
	if (verts[0]->y > verts[2]->y)
	{
		temp = verts[0];
		verts[0] = verts[2];
		verts[2] = temp;
	}

	//Calculate the 'fractional vertical distance' between the top and bottom
	//verts (the X value of the corresponding span to the center)
	float t = (verts[1]->y - verts[0]->y) / (verts[2]->y - verts[0]->y);
	
	Vector3 d = Vector3(
		Lerp(verts[2]->x, verts[0]->x, t),
		verts[1]->y,
		0.0f);

	Vector3 *left, *right;
	if(d.x < verts[1]->x)
	{
		left = &d;
		right = verts[1];
	}
	else
	{
		left = verts[1];
		right = &d;
	}

	//Now we can treat each portion of the tri as a trapezoid
	//ignoring the fact we have 'degenerate' sides

	glBegin(GL_POINTS);
	//Draw each scan line for the top portion
	for(int i=abs(verts[0]->y); i<=d.y; i++)
	{
		float step = (i - verts[0]->y) / (left->y - verts[0]->y);
		int lX = Lerp(left->x, verts[0]->x, step);
		int rX = Lerp(right->x, verts[0]->x, step);
		PlotScanLine(lX,rX,i);
	}

	//Draw each scan line for the lower portion
	//only if the lower points do not share the same scanline
	if(left->y != verts[2]->y || right->y != verts[2]->y)
	{
		for(int i=abs(left->y); i<=verts[2]->y; i++)
		{
			float step = abs(i - left->y) / (verts[2]->y - left->y);
			int lX = Lerp(verts[2]->x, left->x, step);
			int rX = Lerp(verts[2]->x, right->x, step);
			PlotScanLine(lX,rX,i);
		}
	}
	glEnd();

	//We done yet?
}

void RLRenderer::PlotScanLine(int x0, int x1, int y)
{
	while(x0 < x1)
	{
		PlotPixel(x0,y);
		x0++;
	}
}

void RLRenderer::CohenSutherland(const Vector3& p1, const Vector3& p2)
{
	char p1Code = CohenSutherlandCode(p1);
	char p2Code = CohenSutherlandCode(p2);

	//We now test the combined region codes
	if((p1Code | p2Code) == INSIDE)
	{
		//Test to see if the regions are the correct ones
		if(!(p1Code & p2Code))
		{
			RenderLine(p1,p2);
		}
	}
	else if((p1Code & p2Code) == INSIDE)
	{
		//One of the points is outside the clip region
		//Find out which one and clip it

		Vector3 *clippedPoint1;
		//p1 is outside clip region
		//we can clip along the axis that the region code specifies
		if(p1Code == INSIDE)
		{
			clippedPoint1 = new Vector3(p1);
		}
		else if(p1Code & ABOVE)
		{
			//So we clip to the height of the clip region
			clippedPoint1 = new Vector3(
				p1.x + (p2.x - p1.x) * (clipRegionMin->y - p1.y) / (p2.y - p1.y),
				clipRegionMin->y,
				0.0f);
		}
		else if(p1Code & BELOW)
		{
			//So we clip off the bottom where it leaves the clip region
			clippedPoint1 = new Vector3(
				p1.x + (p2.x - p1.x) * (clipRegionMax->y - p1.y) / (p2.y - p1.y),
				clipRegionMax->y,
				0.0f);
		}
		else if(p1Code & LEFT)
		{
			//We clip the left hand end
			clippedPoint1 = new Vector3(
				clipRegionMin->x,
				p1.y + (p2.y - p1.y) * (clipRegionMin->x - p1.x) / (p2.x - p1.x),
				0.0f);
		}
		else if(p1Code & RIGHT)
		{
			//We clip the right hand end
			clippedPoint1 = new Vector3(
				clipRegionMax->x,
				p1.y + (p2.y - p1.y) * (clipRegionMax->x - p1.x) / (p2.x - p1.x),
				0.0f);
		}

		Vector3 *clippedPoint2;
		//p1 is outside clip region
		//we can clip along the axis that the region code specifies
		if(p2Code == INSIDE)
		{
			clippedPoint2 = new Vector3(p2);
		}
		else if(p2Code & ABOVE)
		{
			//So we clip to the height of the clip region
			clippedPoint2 = new Vector3(
				p1.x + (p2.x - p1.x) * (clipRegionMin->y - p1.y) / (p2.y - p1.y),
				clipRegionMin->y,
				0.0f);
		}
		else if(p2Code & BELOW)
		{
			//So we clip off the bottom where it leaves the clip region
			clippedPoint2 = new Vector3(
				p1.x + (p2.x - p1.x) * (clipRegionMax->y - p1.y) / (p2.y - p1.y),
				clipRegionMax->y,
				0.0f);
		}
		else if(p2Code & LEFT)
		{
			//We clip the left hand end
			clippedPoint2 = new Vector3(
				clipRegionMin->x,
				p1.y + (p2.y - p1.y) * (clipRegionMin->x - p1.x) / (p2.x - p1.x),
				0.0f);
		}
		else if(p2Code & RIGHT)
		{
			//We clip the right hand end
			clippedPoint2 = new Vector3(
				clipRegionMax->x,
				p1.y + (p2.y - p1.y) * (clipRegionMax->x - p1.x) / (p2.x - p1.x),
				0.0f);
		}

		RenderLine(*clippedPoint1, *clippedPoint2);
			
		delete(clippedPoint1);
		delete(clippedPoint2);
	}
}

void RLRenderer::RenderTriDevMasterNick(const Tri& tri)
{
	//Extract the coordinates
	float y1 = tri.verts[0].Y();
	float y2 = tri.verts[1].Y();
	float y3 = tri.verts[2].Y();

	float x1 = tri.verts[0].X();
	float x2 = tri.verts[1].X();
	float x3 = tri.verts[2].X();

	//Deltas
	float Dx12 = x1 - x2;
	float Dx23 = x2 - x3;
	float Dx31 = x3 - x1;

	float Dy12 = y1 - y2;
	float Dy23 = y2 - y3;
	float Dy31 = y3 - y1;

	//Define a bounding rectangle
	int minx = x1;
	if(x2 < minx) minx = x2;
	if(x3 < minx) minx = x3;
	int maxx = x1;
	if(x2 > maxx) maxx = x2;
	if(x3 > maxx) maxx = x3;
	int miny = y1;
	if(y2 < miny) miny = y2;
	if(y3 < miny) miny = y3;
	int maxy = y1;
	if(y2 > maxy) maxy = y2;
	if(y3 > maxy) maxy = y3;

	//Find the constant half-edge functions
	//Explanation available online
	float C1 = Dy12 * x1 - Dx12 * y1;
    float C2 = Dy23 * x2 - Dx23 * y2;
    float C3 = Dy31 * x3 - Dx31 * y3;

    float Cy1 = C1 + Dx12 * miny - Dy12 * minx;
    float Cy2 = C2 + Dx23 * miny - Dy23 * minx;
    float Cy3 = C3 + Dx31 * miny - Dy31 * minx;

	glBegin(GL_POINTS);
    // Scan through bounding rectangle
    for(int y = miny; y < maxy; y++)
    {
        // Start value for horizontal scan
        float Cx1 = Cy1;
        float Cx2 = Cy2;
        float Cx3 = Cy3;

        for(int x = minx; x < maxx; x++)
        {
            if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0)
            {
                PlotPixel(x, y);
            }

            Cx1 -= Dy12;
            Cx2 -= Dy23;
            Cx3 -= Dy31;
        }

        Cy1 += Dx12;
        Cy2 += Dx23;
        Cy3 += Dx31;
    }
	glEnd();
}

void RLRenderer::RenderTriFoleyVanDam(const Tri& tri)
{

}

int RLRenderer::RenderCircle(const Circle& circle)
{
	return(this->RenderCircle(circle.position, circle.radius));
}

int RLRenderer::RenderCircle(const Vector3& center, const float radius)
{
	bool clipped = false;
	char regionCode = CohenSutherlandCode(center);

	if(regionCode == INSIDE)
	{
		clipped = false;
	}
	else
	{
		if(regionCode == ABOVE)
		{
			float dist = clipRegionMin->Y() - center.Y();
			if(dist > radius)
			{
				clipped = true;
			}
		}
		else if(regionCode == BELOW)
		{
			float dist = center.Y() - clipRegionMax->Y();
			if(dist > radius)
			{
				clipped = true;
			}
		}
		else if(regionCode == LEFT)
		{
			float dist = clipRegionMin->X() - center.X();
			if(dist > radius)
			{
				clipped = true;
			}
		}
		else if(regionCode == RIGHT)
		{
			float dist = center.X() - clipRegionMin->X();
			if(dist > radius)
			{
				clipped = true;
			}
		}
	}

	if(!clipped)
	{
		float theta = 0.0f;
		int x, y;
		int dX = center.X();
		int dY = center.Y();

		glBegin(GL_POINTS);

		while(theta < 45.0f)
		{
			//Calculate an X and Y value for current angle
			float radians = DegToRad(theta);
		
			x = abs(radius * cos(radians));
			y = abs(radius * sin(radians));

			glVertex2i(dX + x, dY +  y);
			glVertex2i(dX + y, dY +  x);
			glVertex2i(dX + y, dY + -x);
			glVertex2i(dX + x, dY + -y);
			glVertex2i(dX +-x, dY + -y);
			glVertex2i(dX +-y, dY + -x);
			glVertex2i(dX +-y, dY +  x);
			glVertex2i(dX +-x, dY +  y);

			theta++;
		}

		glEnd();

		return 0;
	}
	else
	{
		return 1;
	}
}

int RLRenderer::RenderText(const std::string& name, const int minX, const int minY)
{
	const char* str = name.c_str();

	int i = 0;
	char c = str[i];

	glRasterPos2f(minX, minY);

	while(c != '\0')
	{
		glutBitmapCharacter(currFont, c);
		i++;
		c = str[i];
	}

	return 0;
}