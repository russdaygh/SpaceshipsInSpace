#pragma once

//Forward Declarations
#include "RLVector.h"

namespace RustyLib
{
	namespace Physics
	{
		class RLRay
		{
		private:
			Vector3 *position;
			Vector3 *direction;

		public:
			RLRay(void );
			RLRay(Vector3 *pos, Vector3 *dir);
			~RLRay(void );

			//Accessor methods
			Vector3 GetPosition(void )const;
			void SetPosition(Vector3 *pos);
			Vector3 GetDirection(void )const;
			void SetDirection(Vector3 *dir);

			float SqDistanceToPoint(const Vector3& point)const;
			float DistanceToPoint(const Vector3& point)const;
		};

		typedef RLRay Ray;
	}
}