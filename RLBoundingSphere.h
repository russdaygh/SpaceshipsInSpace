#pragma once

//Project Includes
#include "RLVector.h"

namespace RustyLib
{
	namespace Physics
	{
		class RLRay;

		class RLBoundingSphere
		{
		private:
			Vector3 position;
			Vector3 offset;//For maintaining position HACK
			float radius;
			float sq_radius;//for comparison to sqMagnitude

		public:
			RLBoundingSphere(void );
			~RLBoundingSphere(void );

			//Set up methods
			void Initialise(Vector3 *points, const int num_points);
			void Initialise(Vector3& pos, float radius);

			//Methods to create from a set of points
			void CreateFrom(Vector3 *points, const int num_points);

			//Accessor methods
			Vector3 GetPosition(void )const;
			void SetPosition(Vector3& pos);
			float GetRadius(void )const;
			void SetRadius(float rad);

			bool Contains(const RLBoundingSphere& sphere)const;
			bool Contains(const RLRay& ray)const;
			bool Contains(const Vector3& point)const;
		};

		typedef RLBoundingSphere BSphere;
	}
}