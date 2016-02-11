#include "StdAfx.h"
#include "RLAABoundingBox.h"
using RustyLib::Physics::RLAABoundingBox;

//Match the forward declarations
#include "RLBoundingSphere.h"
using RustyLib::Physics::RLBoundingSphere;

using RustyLib::Vector3;


RLAABoundingBox::RLAABoundingBox(void )
{
}


RLAABoundingBox::~RLAABoundingBox(void)
{
}


void RLAABoundingBox::Initialise(Vector3& min, Vector3& max)
{
	this->min = min;
	this->max = max;
	//We should probably sort these out
	//to make sure they are 'min' and 'max'
	Vector3 points[2];
	points[0] = min;
	points[1] = max;
	CreateFrom(points, 2);
}


void RLAABoundingBox::CreateFrom(Vector3* points, const int num_points)
{
	//Loop through and get all the values from the points
	if(points)
	{
		for(int i=1; i<num_points; i++)
		{
			if(points[i].x < min.x)
			{
				min.x = points[i].x;
			}
			if(points[i].x > max.x)
			{
				max.x = points[i].x;
			}
			if(points[i].y < min.y)
			{
				min.y = points[i].y;
			}
			if(points[i].y > max.y)
			{
				max.y = points[i].y;
			}
			if(points[i].z < min.z)
			{
				min.z = points[i].z;
			}
			if(points[i].z > max.z)
			{
				max.z = points[i].z;
			}
		}
	}
}


Vector3 RLAABoundingBox::GetMin(void )const
{
	return min;
}


Vector3 RLAABoundingBox::GetMax(void )const
{
	return max;
}


bool RLAABoundingBox::Contains(const Vector3& point)const
{
	bool is_contained = false;

	if(point.x > min.x &&
		point.x < max.x &&
		point.y > min.y &&
		point.y < max.y &&
		point.z > min.z &&
		point.z < max.z)
	{
		is_contained = true;
	}

	return is_contained;
}


bool RLAABoundingBox::Contains(const RLBoundingSphere& sphere)const
{
	bool is_contained = false;

	Vector3 point = sphere.GetPosition();
	float d = sphere.GetRadius();

	if(point.x - d > min.x &&
		point.x + d < max.x &&
		point.y - d > min.y &&
		point.y + d < max.y &&
		point.z - d > min.z &&
		point.z + d < max.z)
	{
		is_contained = true;
	}

	return is_contained;
}