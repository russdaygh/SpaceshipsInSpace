#include "StdAfx.h"
#include "Laser.h"


#include "RLMatrix.h"
#include "RLRenderer.h"


Laser::Laser(void)
	: GameObject()
{
}


Laser::~Laser(void)
{
}


Error Laser::Initialise(void )
{
	return Error::RL_NO_ERROR;
}


Error Laser::Initialise(Vector3& pos, float rot, float radius, float vel)
{
	position = pos;
	size = radius;

	using RustyLib::Matrix44;

	Matrix44 *mat = Matrix44::CreateRotationZ(rot);

	velocity = *mat * Vector3::INV_Y_AXIS();

	velocity.Normalise();

	velocity *= vel;

	delete mat;

	return Error::RL_NO_ERROR;
}


Error Laser::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void Laser::Update(const float time_step)
{
	GameObject::Update(time_step);
}


void Laser::Render(void )
{
	RustyLib::Graphics::Renderer *r = &RustyLib::Graphics::Renderer::Instance();

	r->RenderCircle(position, size);
}