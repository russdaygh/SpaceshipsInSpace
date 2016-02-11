#include "StdAfx.h"
#include "StarField.h"


#include "RLRenderer.h"
using RustyLib::Graphics::Renderer;


StarField::StarField(void)
	:stars(0),
	NUM_STARS(0)
{
}


StarField::~StarField(void)
{
	delete [] stars;
}


Error StarField::Initialise(void )
{
	star_colour = Vector3(255.0f);
	return Error::RL_NO_ERROR;
}


Error StarField::Initialise(const Vector3& min, const Vector3& max, int num_stars)
{
	star_colour = Vector3(255.0f);
	NUM_STARS = num_stars;
	GenerateStars(min, max);

	return Error::RL_NO_ERROR;
}


Error StarField::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void StarField::Update(const float time_step)
{
}


void StarField::Render(void )
{
	Renderer *r = &Renderer::Instance();

	r->SetPointSize(2);
	r->SetLineColour(star_colour);

	for(int i=0; i<NUM_STARS; i++)
	{
		r->RenderPoint(stars[i]);
	}

	r->SetPointSize(1);
}


void StarField::GenerateStars(const Vector3& min, const Vector3& max)
{
	srand(timeGetTime());

	int minX = min.x,
		minY = min.y,
		minZ = min.z,
		maxX = max.x,
		maxY = max.y,
		maxZ = max.z;

	stars = new Vector3[NUM_STARS];
	for(int i=0; i<NUM_STARS; i++)
	{
		stars[i] = Vector3(
			rand() % maxX - minX,
			rand() % maxY - minY,
			rand() % maxZ - minZ);
	}
}
