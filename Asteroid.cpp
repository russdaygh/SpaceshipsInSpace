#include "StdAfx.h"
#include "Asteroid.h"

#include "LuaUtilities.h"
#include "RLRenderer.h"
using RustyLib::Graphics::Renderer;

//Define some vars
Vector3 Asteroid::colour;
float Asteroid::MIN_VELOCITY;
float Asteroid::MAX_VELOCITY;
float Asteroid::MIN_ROT_VELOCITY;
float Asteroid::MAX_ROT_VELOCITY;
float Asteroid::MIN_SIZE;
float Asteroid::MAX_SIZE;

Asteroid::Asteroid(void)
	: GameObject()
{
}


Asteroid::~Asteroid(void)
{
}


Error Asteroid::ClassInitialise(void )
{
	//Read out my config details
	LuaUtils::LoadFile("config/asteroid_config.ini");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	colour = LuaUtils::GetVector3("colour");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	MIN_VELOCITY = LuaUtils::GetFloat("MIN_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_VELOCITY = LuaUtils::GetFloat("MAX_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	MIN_ROT_VELOCITY = LuaUtils::GetFloat("MIN_ROT_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_ROT_VELOCITY = LuaUtils::GetFloat("MAX_ROT_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	MIN_SIZE = LuaUtils::GetFloat("MIN_SIZE");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_SIZE = LuaUtils::GetFloat("MAX_SIZE");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Clear up LuaStack
	LuaUtils::ClearLuaStack();

	return Error::RL_NO_ERROR;
}


Error Asteroid::Initialise(void )
{
	size = MAX_SIZE;

	if(HullGenerate())
	{
		return Error::RL_ERROR;
	}

	//make the bsphere from the hull
	b_sphere.CreateFrom(m_hull, num_verts);

	SetVelocities();

	return Error::RL_NO_ERROR;
}


Error Asteroid::Initialise(float size)
{
	if(size > MAX_SIZE)
	{
		this->size = MAX_SIZE;
	}
	else if(size < MIN_SIZE)
	{
		size = MIN_SIZE;
	}
	else
	{
		this->size = size;
	}

	if(HullGenerate())
	{
		return Error::RL_ERROR;
	}

	//Make the bsphere from the hull
	b_sphere.CreateFrom(m_hull, num_verts);

	SetVelocities();

	return Error::RL_NO_ERROR;
}


void Asteroid::SetVelocities(void )
{
	srand(timeGetTime());

	//Set up a velocity
	float angle = rand() % 360;
	float velocity_scalar = (rand() % (int)MAX_VELOCITY) + MIN_VELOCITY;
	velocity_scalar /= 100;

	using RustyLib::Matrix44;

	Matrix44 *rot = Matrix44::CreateRotationZ(angle);

	velocity = *rot * Vector3::INV_Y_AXIS();

	velocity *= velocity_scalar;

	delete rot;

	//Set the rotational velocity
	rot_velocity = (rand() % (int)MAX_ROT_VELOCITY) + MIN_ROT_VELOCITY;
	rot_velocity /= 1000;//we can only use integers with rand because
	//the values required are so low they sometimes truncate to zero
	//which throws with %, so we divide by 10 after to get that low value
}


Error Asteroid::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void Asteroid::Update(const float time_step)
{
	rotation += rot_velocity * time_step;

	GameObject::Update(time_step);

	b_sphere.SetPosition(position);
}


void Asteroid::Render(void )
{
	Renderer *r = &Renderer::Instance();
	
	r->SetLineColour(colour);

	using RustyLib::Matrix44;

	Matrix44 *rot = Matrix44::CreateRotationZ(rotation);

	//This should in theory draw triangles from asteroid center
	//out to its hull points
	int i = 0;
	for(; i < num_verts-1; i++)
	{
		Vector3 *v1 = *rot*m_hull[i];
		Vector3 *v2 = *rot*m_hull[i+1];
		*v1 += position;
		*v2 += position;
		r->RenderTri(position,*v1,*v2);
		delete v1,v2;
	}
	Vector3 *v1 = *rot*m_hull[i];
	Vector3 *v2 = *rot*m_hull[0];
	*v1 += position;
	*v2 += position;
	r->RenderTri(position,*v1,*v2);

	//Draw the b_sphere
	//r->RenderCircle(b_sphere.GetPosition(),b_sphere.GetRadius());

	delete rot,v1,v2;
}


Error Asteroid::HullGenerate(void )
{
	//The number of verts will be based on 
	//the size of the asteroid
	Vector3 *verts;
	num_verts = (int)size * 2;
	verts = new Vector3[num_verts];

	srand(timeGetTime());
	for(int i=0; i<num_verts; i++)
	{
		verts[i] = Vector3(
			(rand() % (int)size*2 - size),
			(rand() % (int)size*2) - size,
			0.0f);
	}

	//GRAHAM SCAN
	//Sort through the verts that have been randomised and 
	//make a convex hull by sorting them in to order


	//Step 1: Find the bottom right point
	Vector3 *scan_start_point = &verts[0];

	for(int i=1; i<num_verts; i++)
	{
		//If it is lower in space
		if(verts[i].y > scan_start_point->y)
		{
			//Swap
			scan_start_point = &verts[i];
		}
		//Otherwise if they are the same
		else if(verts[i].y == scan_start_point->y)
		{
			//Check which is right most
			if(verts[i].x > scan_start_point->x)
			{
				//Swap
				scan_start_point = &verts[i];
			}
		}
	}

	//Now we get the angles relative to the start point
	//and set them up in a pair
	struct graham_pair
	{
		Vector3 *p;
		float angle;
	};

	graham_pair *sorted = new graham_pair[num_verts];

	for(int i=0; i<num_verts; i++)
	{
		sorted[i].p = &verts[i];
		Vector3 w = *sorted[i].p - *scan_start_point;
		w.Normalise();
		sorted[i].angle = w.DotProduct(Vector3::X_AXIS());
	}

	//Then we sort them in order of the angle they are associated with
	graham_pair temp,temp2;
 	for(int i = 1; i< num_verts; i++)
	{
		for(int j = 1; j < num_verts-1; j++)
		{
			if(sorted[j+1].angle < sorted[j].angle)
			{
				temp = sorted[j];
				temp2 = sorted[j+1];
				sorted[j] = temp2;
				sorted[j+1] = temp;
			}
		}
	}

	std::vector<Vector3*> *hull = new std::vector<Vector3*>();
	//We can push the starter point
	hull->push_back(sorted[0].p);
	//Push the next point as it is certain to be part of the hull
	hull->push_back(sorted[1].p);

	int currHullPointCount = hull->size();
	while (currHullPointCount < num_verts)
	{
		Vector3* p1 = (*hull)[hull->size()-2];
		Vector3* p2 = (*hull)[hull->size()-1];

		//If we have to turn left to touch it
		//then it is next in the hull
		if( IsLeftTurn(*p1, *p2, *sorted[currHullPointCount].p) )
		{
			//Place it on the *ahem* 'stack'
			hull->push_back(sorted[currHullPointCount].p);
		}
		//if it is not a left turn then it means the previous
		//verts is not in the convex hull
		else
		{
			if(hull->size() > 2)
			{
				hull->pop_back();
			}
		}
		currHullPointCount++;
	}
	//Push the start point to connect the polygon
	hull->push_back(sorted[0].p);

	//Now we must read that hull in to our verts
	m_hull = new Vector3[hull->size()];

	int i=0;
	std::vector<Vector3*>::iterator iter = hull->begin();
	while(iter != hull->end())
	{
		m_hull[i] = *iter._Ptr;
		i++;
		iter++;
	}

	//Set the number of verts in the hull to the final count
	num_verts = hull->size();

	hull->clear();
	delete hull;
	delete sorted;
	delete [] verts;

	return Error::RL_NO_ERROR;
}


bool Asteroid::IsLeftTurn(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	bool isLeftTurn = false;

	float area2 = ( p2.x - p1.x ) * ( p3.y - p1.y ) -
					( p3.x - p1.x ) * ( p2.y - p1.y );

	isLeftTurn = ( area2 > 0.0 );

	return isLeftTurn;
}


BSphere& Asteroid::GetBSphere(void )
{
	return b_sphere;
}