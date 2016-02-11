#include "StdAfx.h"
#include "Player.h"

#include "LuaUtilities.h"

#include "RLKeyboard.h"
using RustyLib::Input::Keyboard;
using RustyLib::Input::KeyCode;
#include "RLRenderer.h"
using RustyLib::Graphics::Renderer;

#include "Asteroid.h"
#include "Laser.h"

Player::Player(void)
	: GameObject(),
	score(0),
	curr_laser_time(0),
	thrust(0)
{
}


Player::~Player(void)
{
}


Error Player::Initialise(void )
{
	//Read out my config details
	LuaUtils::LoadFile("config/player_config.ini");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Read out starting position
	this->position = LuaUtils::GetVector3("starting_position");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Get player colour
	colour = LuaUtils::GetVector3("colour");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Get the verts that make up the ships
	verts[0] = LuaUtils::GetVector3("v1");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	verts[1] = LuaUtils::GetVector3("v2");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	verts[2] = LuaUtils::GetVector3("v3");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	verts[3] = LuaUtils::GetVector3("v4");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Get the size modifier
	size = LuaUtils::GetFloat("size");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	verts[0] *= size;
	verts[1] *= size;
	verts[2] *= size;
	verts[3] *= size;

	//Set up the BSphere
	b_sphere.CreateFrom(verts, 4);

	//Get movement config variables
	thrust_accel = LuaUtils::GetFloat("thrust_accel");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_THRUST = LuaUtils::GetFloat("MAX_THRUST");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	rot_accel = LuaUtils::GetFloat("rot_accel");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	friction = LuaUtils::GetFloat("friction");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_VELOCITY = LuaUtils::GetFloat("MAX_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Get the key codes
	left_key = LuaUtils::GetChar("left_key");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	right_key = LuaUtils::GetChar("right_key");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	thrust_key = LuaUtils::GetChar("thrust_key");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	fire_key = (char)LuaUtils::GetInt("fire_key");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	LASER_RADIUS = LuaUtils::GetFloat("LASER_RADIUS");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	LASER_VELOCITY = LuaUtils::GetFloat("LASER_VELOCITY");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	LASER_RECHARGE_TIME = LuaUtils::GetFloat("LASER_RECHARGE_TIME");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	//Clear up LuaStack
	LuaUtils::ClearLuaStack();

	//Hard coded hacky slashy AABBox
	play_region.Initialise(
		Vector3(0.0f,0.0f,-b_sphere.GetRadius()-100.0f),
		Vector3(800, 600, b_sphere.GetRadius()+100.0f));

	return Error::RL_NO_ERROR;
}


Error Player::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void Player::Update(const float time_step)
{
	if(Keyboard::IsKeyDown((KeyCode)left_key))
	{
		//rotate left
		rotation += (rot_accel * time_step);
	}
	if(Keyboard::IsKeyDown((KeyCode)right_key))
	{
		//rotate right
		rotation -= (rot_accel * time_step);
	}
	if(Keyboard::IsKeyDown((KeyCode)thrust_key))
	{
		//increase thrust
		thrust += thrust_accel * time_step;
	}
	else
	{
		thrust = 0;
	}

	if(thrust < 0)
	{
		thrust = 0;
	}
	else if(thrust > MAX_THRUST)
	{
		thrust = MAX_THRUST;
	}

	if(Keyboard::IsKeyDown((KeyCode)fire_key))
	{
		//fire a laser
		curr_laser_time -= time_step;
		if(curr_laser_time < 0)
		{
			FireLaser();
			curr_laser_time = LASER_RECHARGE_TIME;
		}
	}

	using RustyLib::Matrix44;

	Matrix44 *rot_mat = Matrix44::CreateRotationZ(rotation);

	acceleration = (*rot_mat * Vector3::INV_Y_AXIS());

	acceleration *= thrust;

	velocity += acceleration;

	float curr_vel = velocity.Magnitude();

	if(curr_vel > MAX_VELOCITY)
	{
		curr_vel = MAX_VELOCITY;
	}

	velocity.Normalise();

	velocity *= curr_vel;

	GameObject::Update(time_step);

	//Do some collision detection
	b_sphere.SetPosition(position);

	//First check whether player is on screen still
	if(! play_region.Contains(b_sphere) )
	{
		//Player needs moving
		Vector3 min = play_region.GetMin();
		Vector3 max = play_region.GetMax();

		if(position.x < min.x - b_sphere.GetRadius())
		{
			position.x = max.x + (b_sphere.GetRadius()-1.0f);
		}
		if(position.x > max.x + b_sphere.GetRadius())
		{
			position.x = min.x - (b_sphere.GetRadius()+1.0f);
		}
		if(position.y < min.y - b_sphere.GetRadius())
		{
			position.y = max.y + (b_sphere.GetRadius()-1.0f);
		}
		if(position.y > max.y + b_sphere.GetRadius())
		{
			position.y = min.y - (b_sphere.GetRadius()+1.0f);
		}
		b_sphere.SetPosition(position);
	}

	std::list<Laser*>::iterator iter = lasers.begin();
	std::list<Laser*> tbd;//lasers to be deleted
	int ct = 0;
	while(iter != lasers.end())
	{
		Laser* l = iter._Ptr->_Myval;
		l->Update(time_step);
		if(!play_region.Contains(l->GetPosition()))
		{
			//laser has left screen
			tbd.push_back(l);
		}
		iter++;
		ct++;
	}
	iter = tbd.begin();
	while(iter != tbd.end())
	{
		lasers.remove(iter._Ptr->_Myval);
		iter++;
	}
	tbd.clear();
}


void Player::Render(void )
{
	Renderer *r = &Renderer::Instance();
	
	r->SetLineColour(colour);

	using RustyLib::Matrix44;

	Matrix44 *mat = Matrix44::CreateRotationZ(rotation);

	Vector3 *transformed_verts[4];
	transformed_verts[0] = *mat * verts[0];
	*transformed_verts[0] += position;
	transformed_verts[1] = *mat * verts[1];
	*transformed_verts[1] += position;
	transformed_verts[2] = *mat * verts[2];
	*transformed_verts[2] += position;
	transformed_verts[3] = *mat * verts[3];
	*transformed_verts[3] += position;

	r->RenderTri(*transformed_verts[0],*transformed_verts[1],*transformed_verts[2]);
	r->RenderTri(*transformed_verts[2],*transformed_verts[3],*transformed_verts[0]);

	//r->RenderCircle(b_sphere.GetPosition(), b_sphere.GetRadius());

	//Render the lasers
	std::list<Laser*>::iterator iter = lasers.begin();
	while(iter != lasers.end())
	{
		iter._Ptr->_Myval->Render();
		iter++;
	}

	std::ostringstream sstr;
	sstr << "Player Score: " << score;
	
	r->RenderText(sstr.str(), 10, 20);

	delete mat, transformed_verts;
}


BSphere& Player::GetBSphere(void )
{
	return b_sphere;
}


void Player::FireLaser(void )
{
	Laser *l = new Laser();
	l->Initialise(position, rotation, LASER_RADIUS, LASER_VELOCITY);

	lasers.push_back(l);
}


void Player::CheckLaserAsteroidCollisions(std::list<Asteroid*>* asteroids)
{
	std::list<Asteroid*>::iterator a_iter = asteroids->begin();
	std::list<Asteroid*> a_tbd;
	std::list<Laser*> l_tbd;

	while(a_iter != asteroids->end())
	{
		Asteroid *ast = a_iter._Ptr->_Myval;
		std::list<Laser*>::iterator l_iter = lasers.begin();

		while(l_iter != lasers.end())
		{
			Laser *las = l_iter._Ptr->_Myval;

			if(ast->GetBSphere().Contains(las->GetPosition()))
			{
				//The laser is inside the asteroid bsphere
				a_tbd.push_back(ast);
				l_tbd.push_back(las);
				AddScore(ast->GetBSphere().GetRadius());
			}
			l_iter++;
		}
		a_iter++;
	}

	//Do the deletions
	std::list<Laser*>::iterator l_iter = l_tbd.begin();
	while(l_iter != l_tbd.end())
	{
		lasers.remove(l_iter._Ptr->_Myval);
		l_iter++;
	}
	a_iter = a_tbd.begin();
	while(a_iter != a_tbd.end())
	{
		asteroids->remove(a_iter._Ptr->_Myval);
		a_iter++;
	}
}


void Player::AddScore(int points)
{
	score += points;
}

const int Player::GetScore(void )const
{
	return score;
}