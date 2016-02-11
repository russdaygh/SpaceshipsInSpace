#include "stdafx.h"
#include "RLBoundingSphere.h"
using RustyLib::Physics::RLBoundingSphere;

//Local includes
#include "RLRay.h"
using RustyLib::Physics::RLRay;

using RustyLib::Vector3;

RLBoundingSphere::RLBoundingSphere(void )
	: position(Vector3()),
		radius(0.0f),
		sq_radius(0.0f)
{
}

RLBoundingSphere::~RLBoundingSphere(void )
{
}

void RLBoundingSphere::Initialise(Vector3 *points, const int num_points)
{
	CreateFrom(points, num_points);
}

void RLBoundingSphere::Initialise(Vector3& pos, float radius)
{
	position = pos;
	this->radius = radius;
	sq_radius = radius*radius;
}

void RLBoundingSphere::CreateFrom(Vector3 *points, const int num_points)
{
	//Loop through the verts and do the following
	//1: Find the center point of the set
	//2: Find the furthest point in the set from that center
	//if(points)
	//{
	//	float top = 0.0f, bottom = 0.0f, 
	//		left = 0.0f, right = 0.0f,
	//		front = 0.0f, back = 0.0f;

	//	for(int i=1; i<num_points; i++)
	//	{
	//		if(points[i].x < left)
	//		{
	//			left = points[i].x;
	//		}
	//		if(points[i].x > right)
	//		{
	//			right = points[i].x;
	//		}
	//		if(points[i].y < top)
	//		{
	//			top = points[i].y;
	//		}
	//		if(points[i].y > bottom)
	//		{
	//			bottom = points[i].y;
	//		}
	//		if(points[i].z < front)
	//		{
	//			front = points[i].z;
	//		}
	//		if(points[i].z > back)
	//		{
	//			back = points[i].z;
	//		}
	//	}
	//	//We should now have the most extreme points (axis-aligned)
	//	float width = right - left;
	//	float height = bottom - top;
	//	float depth = back - front;

	//	offset = Vector3(
	//		(width / 2) + left,
	//		(height / 2) + top,
	//		(depth / 2) + front);

	//	position = offset;

	if(points)
	{
		for(int i=0; i<num_points; i++)
		{
			position += points[i];
		}
		position /= num_points;
		//Now we have the position we can iterate through the verts
		//and find which lies farthest away and in doing so
		//the radius of the resulting BSphere
		float temp_radius = Vector3::GetSquareDistance(position, points[0]);
		for(int i=1; i<num_points; i++)
		{
			float curr_radius = Vector3::GetSquareDistance(position, points[i]);
			if(curr_radius > temp_radius)
			{
				temp_radius = curr_radius;
			}
		}
		this->sq_radius = temp_radius;
		this->radius = sqrt(sq_radius);
	}
}

Vector3 RLBoundingSphere::GetPosition(void )const
{
	return position;
}

void RLBoundingSphere::SetPosition(Vector3& pos)
{
	position = pos + offset;
}

float RLBoundingSphere::GetRadius(void )const
{
	return radius;
}

void RLBoundingSphere::SetRadius(float rad)
{
	radius = rad;
	sq_radius = rad*rad;
}

bool RLBoundingSphere::Contains(const RLBoundingSphere& sphere)const
{
	float checkDist = (this->sq_radius + sphere.sq_radius)*2;
	float actualDist = Vector3::GetSquareDistance(position, sphere.position);
	return (checkDist > actualDist);
}

bool RLBoundingSphere::Contains(const RLRay& ray)const
{
	float shortestDistanceToRay = ray.SqDistanceToPoint(position);
	return (sq_radius > shortestDistanceToRay);
}

bool RLBoundingSphere::Contains(const Vector3& point)const
{
	float distToPoint = (point - position).SquareMagnitude();
	return (sq_radius > distToPoint);
}