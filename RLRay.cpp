#include "stdafx.h"
#include "RLRay.h"

namespace RustyLib
{
	namespace Physics
	{
		RLRay::RLRay(void )
			: position(new Vector3()),
			  direction(new Vector3())
		{
		}

		RLRay::RLRay(Vector3 *pos, Vector3 *dir)
			: position(pos),
			  direction(dir)
		{
			dir->Normalise();
		}

		RLRay::~RLRay(void )
		{
			delete(position);
			delete(direction);
		}

		Vector3 RLRay::GetPosition(void )const
		{
			return *position;
		}

		void RLRay::SetPosition(Vector3 *pos)
		{
			position = pos;
		}

		Vector3 RLRay::GetDirection(void )const
		{
			return *direction;
		}

		void RLRay::SetDirection(Vector3 *dir)
		{
			direction = dir;
		}

		float RLRay::SqDistanceToPoint(const Vector3& point)const
		{
			//       point
			//       /|
			//      / |
			//    w/  |
			//    /0  |
			//   /---->dir
			//  pos
			Vector3 w = point - *this->position;

			float wsq = w.DotProduct(w);

			float proj = w.DotProduct(*direction);

			return wsq - proj*proj;
		}

		float RLRay::DistanceToPoint(const Vector3& point)const
		{
			float sqDist = this->SqDistanceToPoint(point);
			return sqrt(sqDist);
		}
	}
}