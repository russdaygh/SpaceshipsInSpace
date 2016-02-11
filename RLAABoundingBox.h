#pragma once

//Local Includes
#include "RLVector.h"

namespace RustyLib
{
	namespace Physics
	{
		class RLBoundingSphere;

		class RLAABoundingBox
		{
		private:
			Vector3 min, max;

		public:
			RLAABoundingBox(void );
			~RLAABoundingBox(void );

			void Initialise(Vector3& min, Vector3& max);

			void CreateFrom(Vector3* points, const int num_points);

			Vector3 GetMin(void )const;
			Vector3 GetMax(void )const;

			bool Contains(const Vector3& point)const;
			bool Contains(const RLBoundingSphere& sphere)const;
		};

		typedef RLAABoundingBox AABBox;
	}
}

