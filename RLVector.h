#pragma once

//System includes
#include "StdAfx.h"

#include "RLUtilities.h"

using RustyLib::Utils::EPSILON;

namespace RustyLib
{
	namespace Graphics
	{
		class RLRenderer;
	}
	class RLVector
	{
	public:
		float x, y, z, w;

		//Default constructor
		RLVector(void){x=0.0f;y=0.0f;z=0.0f;w=1.0f;}
		RLVector(const float initVal){x=initVal;y=initVal;z=initVal;w=1.0f;}

		//Constructor taking all 3 components at initialisation
		RLVector(const float& x, const float& y, const float& z, const float& w = 1.0f)
		{ this->x = x; this->y = y; this->z = z; this->w = w;}

		//Destructor
		~RLVector(void){/* Attributes are not pointers, need not be freed */}

		//Accessor functions
		inline float X(void )const{return x;}
		inline float Y(void )const{return y;}
		inline float Z(void )const{return z;}
		inline float W(void )const{return w;}
		inline void X(float f){x=f;}
		inline void Y(float f){y=f;}
		inline void Z(float f){z=f;}
		inline void W(float f){w=f;}

		//Flip all Vector components
		void Invert()
		{ x = -x; y = -y; z = -z; }

		//Magnitude/Length discovery methods
		float Length() const
		{ return sqrtf( x*x + y*y + z*z ); }
		//Returns the magnitude of the vector
		float Magnitude() const
		{ return this->Length(); }
		//Returns the magnitude of the vector without performing sqrt
		//This is still useful for comparing vector magnitudes
		float SquareMagnitude() const
		{ return x*x + y*y + z*z; }
		//Finds the distance between two vectors
		static float GetDistance(const RLVector& v1, const RLVector& v2)
		{
			float dist = GetSquareDistance(v1, v2);
			return dist * dist;
		}
		//Finds the square distance between two vectors
		static float GetSquareDistance(const RLVector& v1, const RLVector& v2)
		{
			RLVector diff = v2 - v1;
			float sqDist = diff.SquareMagnitude();
			return sqDist;
		}
		//Finds the gradient between two vectors
		static float Get2DGradient(const RLVector& v1, const RLVector& v2)
		{
			float dx = (v2.X() - v1.X());
			float dy = (v2.Y() - v1.Y());
			float gradient = 0;
			if(dy != 0 && dx != 0)
			{
				gradient = dx / dy;
			}
			return gradient;
		}
		//Finds the direction component of a given vector
		static float GetDirection(const RLVector& v)
		{
			return atan2(v.y, v.x); 
		}
		//Member functino for getting direction
		float GetDirection(void )
		{
			return atan2(y, x);
		}
		//Transforms into a unit vector
		void Normalise()
		{
			float l = this->Length();
			if(l>0)
			{
				(*this) *=  1.0f / l;
			}
			else
			{
				(*this).ZeroFloat();
			}
		}
		//Collapses all components to zero (0.0f)
		void ZeroFloat()
		{ x = 0.0f; y = 0.0f; z = 0.0f; }
		//Collapses all components to zero (32bit float Epsilon)
		void ZeroEpsilon()
		{ x = EPSILON; y = EPSILON; z = EPSILON; }

		//Array subscript operator (will return w if arg is not x, y or z)
		float& operator[](int i)
		{
			if(i==0)
				return x;
			if(i==1)
				return y;
			if(i==2)
				return z;
			else
				return w;
		}
		//Array subscript operator (for use on const Vectors)
		float const& operator[](int i)const
		{
			if(i==0)
				return x;
			if(i==1)
				return y;
			if(i==2)
				return z;
			else
				return w;
		}

		//Relational Operator EqualsEquals
		bool operator==(const RLVector& v)
		{ return ( x==v.x && y==v.y && z==v.z ); }
		//Relational Operator NotEquals
		bool operator!=(const RLVector& v)
		{ return ( ! (*this == v) ); }

		//Equals Assignment
		void operator=(const RLVector& v)
		{ x = v.X(); y = v.Y(); z = v.Z(); w = v.W(); }
		//Equals Assignment (Pointer)
		void operator=(const RLVector* v)
		{ x = v->X(); y = v->Y(); z = v->Z(); w = v->W(); }
		//Equals assignment (scalar)
		void operator=(const float s)
		{ x = s; y = s; z = s; }

		//Scalar Multiplication Operators
		void operator*=(const float scalar)
		{ x *= scalar; y *= scalar; z *= scalar; }

		RLVector operator*(const float scalar)const
		{ return RLVector(x*scalar,y*scalar,z*scalar); }

		//Scalar division operators
		void operator/=(const float scalar)
		{ assert(scalar != 0); x /= scalar; y /= scalar; z/= scalar; }

		RLVector operator/(const float scalar)
		{ assert(scalar != 0); return RLVector(x /= scalar, y /= scalar, z/= scalar); }

		void operator/=(const int scalar)
		{ assert(scalar != 0); x /= scalar; y /= scalar; z/= scalar; }

		RLVector operator/(const int scalar)
		{ assert(scalar != 0); return RLVector(x /= scalar, y /= scalar, z/= scalar); }

		//Additive Operations
		void operator+=(const RLVector& v)
		{ x += v.x; y += v.y; z +=v.z; }

		//Additive operator
		RLVector operator+(const RLVector& v)const
		{ return RLVector(x+v.X(),y+v.Y(),z+v.Z()); }

		//Subtractive operations
		void operator-=(const RLVector& v)
		{ x -= v.x; y -= v.y; z -= v.z; }

		RLVector operator-(const RLVector& v)const
		{ return RLVector(x-v.x,y-v.y,z-v.z); }

		//Addition of a vector multiplied by a scalar (single operation)
		void AddScaledVector(const RLVector& v, const float scalar)
		{ x += v.x*scalar; y += v.y*scalar; z += v.z*scalar; }

		//Basic multiplication
		RLVector ComponentProduct(const RLVector& v)const
		{ return RLVector(x*v.x,y*v.y,z*v.z); }

		void ComponentProductUpdate(const RLVector& v)
		{ x *= v.x; y *= v.y; z *= v.z; }

		//Dot/Scalar product operations
		//Both operands should be normalised
		float DotProduct(const RLVector& v)const
		{ return x*v.x + y*v.y + z*v.z; }
		//Dot/Scalar product operations
		//Both operands should be normalised
		float operator*(const RLVector& v)const
		{ return x*v.x + y*v.y + z*v.z; }

		//Cross or Vector product operations
		//The cross product uses the right hand rule
		//wherein the first argVector correspons to your 
		//forefinger, the second becomes your middle
		//finger and the resultant vector is your thumb

		//Vec1  \  |VecResult
		//       \ |
		//		  \|
		//		  /
		//		 /
		//Vec2  /

		//TODO: FIX FOR 3D
		RLVector* CrossProduct(const RLVector& v)const
		{ 
			return new RLVector(
				y*v.z - v.y*z,
				z*v.x - v.z*x,
				x*v.y - v.x*y ); }
		//Cross product operator overload
		void operator%=(const RLVector& v)
		{ *this = this->CrossProduct(v); }
		//Cross product operator overload
		RLVector* operator%(const RLVector& v)const
		{ return this->CrossProduct(v); }

		//Overload of the output operator
		friend std::ostream& operator<<(std::ostream& out, const RLVector& v)
		{
			out << std::endl << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
			return out;
		}

		//Some nice and handy pre-made vector definitions
		const static RLVector& VectorZero(void )
		{ static RLVector zero(0.0f,0.0f,0.0f); return zero; }
		const static RLVector& VectorOne(void )
		{ static RLVector one(1.0f,1.0f,1.0f); return one; }

		const static RLVector& X_AXIS(void )
		{ static RLVector x_axis(1.0f,0.0f,0.0f); return x_axis; }
		const static RLVector& INV_X_AXIS(void )
		{ static RLVector inv_x_axis(-1.0f,0.0f,0.0f); return inv_x_axis; }

		const static RLVector& Y_AXIS(void )
		{ static RLVector y_axis(0.0f,1.0f,0.0f); return y_axis; }
		const static RLVector& INV_Y_AXIS(void )
		{ static RLVector inv_y_axis(0.0f,-1.0f,0.0f); return inv_y_axis; }

		const static RLVector& Z_AXIS(void )
		{ static RLVector z_axis(0.0f,0.0f,1.0f); return z_axis; }
		const static RLVector& INV_Z_AXIS(void )
		{ static RLVector inv_z_axis(0.0f,0.0f,-1.0f); return inv_z_axis; }
	};

	//Rename the class tag for code use
	typedef RLVector Vector3;
}