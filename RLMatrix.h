#pragma once

//System includes
#include "StdAfx.h"

namespace RustyLib
{
	class RLMatrix
	{
	private:

		//Column major matrix
		//00 10 20 30
		//01 11 21 31
		//02 12 22 32
		//03 13 23 33
		float mat[16];

	public:

		//Default ctor (makes a garbage Matrix)
		RLMatrix(void)
		{
		}
		//Destructor
		~RLMatrix(void)
		{
		}

		//Make all elements equal 0.0f
		void Zero()
		{
			*this = 0.0f;
		}
		//Returns a zero matrix
		static RLMatrix CreateZero()
		{
			RLMatrix m;
			m.Zero();
			return m;
		}
		//Make matrix into identity
		void LoadIdentity()
		{
			//Set all but diagonal to 0.0f
			mat[1]  = mat[2]  = mat[3]  =
			mat[4]  = mat[6]  = mat[7]  =
			mat[8]  = mat[9]  = mat[11] =
			mat[12] = mat[13] = mat[14] = 0.0f;
			//Set diagonal to 1.0f
			mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
		}
		//Returns an identity matrix
		static RLMatrix CreateIdentity()
		{
			RLMatrix m;
			m.LoadIdentity();
			return m;
		}

		//Method to flip elements across the main diagonal
		void Transpose()
		{
			RLMatrix copy = *this;
			//Flip all elements over
			mat[1]  = copy.mat[1] ; mat[2]  = copy.mat[2] ; mat[3]   = copy.mat[3] ;
			mat[4]  = copy.mat[4] ; mat[6]  = copy.mat[6] ; mat[7]   = copy.mat[7] ;
			mat[8]  = copy.mat[8] ; mat[9]  = copy.mat[9] ; mat[11]  = copy.mat[11];
			mat[12] = copy.mat[12]; mat[13] = copy.mat[13]; mat[14]  = copy.mat[14];
		}
		//Method to return a copy of the transpose
		RLMatrix& TransposeCopy()
		{
			RLMatrix* copy = this;
			copy->Transpose();
			return *copy;
		}

		//Method set the diagonal components of the matrix (w is optional)
		void SetDiagonal(const float x, const float y, const float z, const float w = 1.0f)
		{
			mat[0]  = x;
			mat[5]  = y;
			mat[10] = z;
			mat[15] = w;
		}

		//Create a translation matrix from float components
		static RLMatrix* CreateTranslation(float x, float y, float z, float w = 1.0f)
		{
			RLMatrix *m = new RLMatrix();
			m->LoadIdentity();

			//{ 1, 0, 0, x }
			//{ 0, 1, 0, y }
			//{ 0, 0, 1, z }
			//{ 0, 0, 0, 1 }

			m->mat[3]  = x;
			m->mat[7]  = y;
			m->mat[11] = z;
			m->mat[15] = w;

			return m;
		}
		//Create a translation matrix from a vector
		static RLMatrix* CreateTranslation(const RLVector& v)
		{
			RLMatrix *m = new RLMatrix();
			m->LoadIdentity();

			//{ 1, 0, 0, x }
			//{ 0, 1, 0, y }
			//{ 0, 0, 1, z }
			//{ 0, 0, 0, 1 }

			m->mat[3]  = v.X();
			m->mat[7]  = v.Y();
			m->mat[11] = v.Z();

			return m;
		}
		//Create a rotation on the X axis
		static RLMatrix* CreateRotationX(float angle)
		{
			RLMatrix *m = new RLMatrix();
			m->LoadIdentity();

			//{1 0          0           0}
			//{0 cos(angle) -sin(angle) 0}
			//{0 sin(angle) cos(angle)  0}
			//{0 0          0           1}

			m->mat[5]  =  cos(angle);
			m->mat[6]  = -sin(angle);
			m->mat[9]  =  sin(angle);
			m->mat[10] =  cos(angle);

			return m;
		}
		//Create a rotation on the Y axis
		static RLMatrix* CreateRotationY(float angle)
		{
			RLMatrix *m = new RLMatrix();
			m->LoadIdentity();

			//{cos(angle)  0          sin(angle)  0}
			//{0           1          0           0}
			//{-sin(angle) 0          cos(angle)  0}
			//{0           0          0           1}

			m->mat[0]  =  cos(angle);
			m->mat[2]  =  sin(angle);
			m->mat[8]  = -sin(angle);
			m->mat[10] =  cos(angle);

			return m;
		}
		//Create a rotation on the Z axis
		static RLMatrix* CreateRotationZ(float angle)
		{
			RLMatrix *m = new RLMatrix();
			m->LoadIdentity();

			//{cos(angle) -sin(angle) 0 0}
			//{sin(angle) cos(angle)  0 0}
			//{0          0           1 0}
			//{0          0           0 1}

			m->mat[0] =  cos(angle);
			m->mat[1] = -sin(angle);
			m->mat[4] =  sin(angle);
			m->mat[5] =  cos(angle);

			return m;
		}

		//Returns the 'trace' of the matrix
		float Trace()
		{
			return mat[0] + mat[5] + mat[10] + mat[15];
		}

		//Element access operator
		inline float& operator()(const unsigned int x, const unsigned int y)
		{
			//Check the values are valid
			assert(x<4);
			assert(y<4);
			return mat[x + (y*4)];
		}

		inline float& operator[](const unsigned int e)
		{
			//Check for valid element
			assert(e<16);
			return mat[e];
		}

		//IsEqual logical operator (const-safe)
		bool operator==(const RLMatrix& m)const
		{
			return(
			mat[0]  == m.mat[0]  && mat[1]  == m.mat[1]  && mat[2]  == m.mat[2]  && mat[3]  == m.mat[3]  &&
			mat[4]  == m.mat[4]  && mat[5]  == m.mat[5]  && mat[6]  == m.mat[6]  && mat[7]  == m.mat[7]  &&
			mat[8]  == m.mat[8]  && mat[9]  == m.mat[9]  && mat[10] == m.mat[10] && mat[11] == m.mat[11] &&
			mat[12] == m.mat[12] && mat[13] == m.mat[13] && mat[14] == m.mat[14] && mat[15] == m.mat[15]
			);
		}

		//Assignment of another matrix
		void operator=(const RLMatrix& m)
		{
			mat[0]  = m.mat[0] ; mat[1]  = m.mat[1] ; mat[2]  = m.mat[2] ; mat[3]  = m.mat[3] ;
			mat[4]  = m.mat[4] ; mat[5]  = m.mat[5] ; mat[6]  = m.mat[6] ; mat[7]  = m.mat[7] ;
			mat[8]  = m.mat[8] ; mat[9]  = m.mat[9] ; mat[10] = m.mat[10]; mat[11] = m.mat[11];
			mat[12] = m.mat[12]; mat[13] = m.mat[13]; mat[13] = m.mat[13]; mat[15] = m.mat[15];
		}
		//Assignment of a scalar to all elements
		void operator=(const float s)
		{
			mat[0]  = mat[1]  = mat[2]  = mat[3]  =
			mat[4]  = mat[5]  = mat[6]  = mat[7]  =
			mat[8]  = mat[9]  = mat[10] = mat[11] =
			mat[12] = mat[13] = mat[14] = mat[15] = s;
		}

		//Matrix subtraction operator (every element)
		void operator-=(const RLMatrix& m)
		{
			mat[0]  -= m.mat[0] ; mat[1]  -= m.mat[1] ; mat[2]  -= m.mat[2] ; mat[3]  -= m.mat[3] ;
			mat[4]  -= m.mat[4] ; mat[5]  -= m.mat[5] ; mat[6]  -= m.mat[6] ; mat[7]  -= m.mat[7] ;
			mat[8]  -= m.mat[8] ; mat[9]  -= m.mat[9] ; mat[10] -= m.mat[10]; mat[11] -= m.mat[11];
			mat[12] -= m.mat[12]; mat[13] -= m.mat[13]; mat[14] -= m.mat[14]; mat[15] -= m.mat[15];
		}
		//Matrix subtraction operator (every element)
		RLMatrix& operator-(const RLMatrix& m)
		{
			RLMatrix* r = new RLMatrix();
			r->mat[0]  = mat[0]  - m.mat[0] ; r->mat[1]  = mat[1]  - m.mat[1] ; r->mat[2]  = mat[2]  - m.mat[2] ; r->mat[3]  = mat[3]  - m.mat[3] ; 
			r->mat[4]  = mat[4]  - m.mat[4] ; r->mat[5]  = mat[5]  - m.mat[5] ; r->mat[6]  = mat[6]  - m.mat[6] ; r->mat[7]  = mat[7]  - m.mat[7] ; 
			r->mat[8]  = mat[8]  - m.mat[8] ; r->mat[9]  = mat[9]  - m.mat[9] ; r->mat[10] = mat[10] - m.mat[10]; r->mat[11] = mat[11] - m.mat[11]; 
			r->mat[12] = mat[12] - m.mat[12]; r->mat[13] = mat[13] - m.mat[13]; r->mat[14] = mat[14] - m.mat[14]; r->mat[15] = mat[15] - m.mat[15]; 
			return *r;
		}

		//Matrix addition operator (every element)
		void operator+=(const RLMatrix& m)
		{
			mat[0]  += m.mat[0] ; mat[1]  += m.mat[1] ; mat[2]  += m.mat[2] ; mat[3]  += m.mat[3] ;
			mat[4]  += m.mat[4] ; mat[5]  += m.mat[5] ; mat[6]  += m.mat[6] ; mat[7]  += m.mat[7] ;
			mat[8]  += m.mat[8] ; mat[9]  += m.mat[9] ; mat[10] += m.mat[10]; mat[11] += m.mat[11];
			mat[12] += m.mat[12]; mat[13] += m.mat[13]; mat[14] += m.mat[14]; mat[15] += m.mat[15];
		}
		//Matrix addition operator (every element)
		RLMatrix& operator+(const RLMatrix& m)
		{
			RLMatrix* r = new RLMatrix();
			r->mat[0]  = mat[0]  + m.mat[0] ; r->mat[1]  = mat[1]  + m.mat[1] ; r->mat[2]  = mat[2]  + m.mat[2] ; r->mat[3]  = mat[3]  + m.mat[3] ; 
			r->mat[4]  = mat[4]  + m.mat[4] ; r->mat[5]  = mat[5]  + m.mat[5] ; r->mat[6]  = mat[6]  + m.mat[6] ; r->mat[7]  = mat[7]  + m.mat[7] ; 
			r->mat[8]  = mat[8]  + m.mat[8] ; r->mat[9]  = mat[9]  + m.mat[9] ; r->mat[10] = mat[10] + m.mat[10]; r->mat[11] = mat[11] + m.mat[11]; 
			r->mat[12] = mat[12] + m.mat[12]; r->mat[13] = mat[13] + m.mat[13]; r->mat[14] = mat[14] + m.mat[14]; r->mat[15] = mat[15] + m.mat[15]; 
			return *r;
		}

		//Matrix Product
		void operator*=(const RLMatrix& m)
		{
			//order of multiplication:
			//1  2  3  4
			//5  6  7  8
			//9  10 11 12
			//13 14 15 16
			mat[0]  = (mat[0]  * m.mat[0]) + (mat[1]  * m.mat[4]) + (mat[2]  * m.mat[8] ) + (mat[3]  * m.mat[12]);
			mat[1]  = (mat[0]  * m.mat[1]) + (mat[1]  * m.mat[5]) + (mat[2]  * m.mat[9] ) + (mat[3]  * m.mat[13]);
			mat[2]  = (mat[0]  * m.mat[2]) + (mat[1]  * m.mat[6]) + (mat[2]  * m.mat[10]) + (mat[3]  * m.mat[14]);
			mat[3]  = (mat[0]  * m.mat[3]) + (mat[1]  * m.mat[7]) + (mat[2]  * m.mat[11]) + (mat[3]  * m.mat[15]);

			mat[4]  = (mat[4]  * m.mat[0]) + (mat[5]  * m.mat[4]) + (mat[6]  * m.mat[8] ) + (mat[7]  * m.mat[12]);
			mat[5]  = (mat[4]  * m.mat[1]) + (mat[5]  * m.mat[5]) + (mat[6]  * m.mat[9] ) + (mat[7]  * m.mat[13]);
			mat[6]  = (mat[4]  * m.mat[2]) + (mat[5]  * m.mat[6]) + (mat[6]  * m.mat[10]) + (mat[7]  * m.mat[14]);
			mat[7]  = (mat[4]  * m.mat[3]) + (mat[5]  * m.mat[7]) + (mat[6]  * m.mat[11]) + (mat[7]  * m.mat[15]);

			mat[8]  = (mat[8]  * m.mat[0]) + (mat[9]  * m.mat[4]) + (mat[10] * m.mat[8] ) + (mat[11] * m.mat[12]);
			mat[9]  = (mat[8]  * m.mat[1]) + (mat[9]  * m.mat[5]) + (mat[10] * m.mat[9] ) + (mat[11] * m.mat[13]);
			mat[10] = (mat[8]  * m.mat[2]) + (mat[9]  * m.mat[6]) + (mat[10] * m.mat[10]) + (mat[11] * m.mat[14]);
			mat[11] = (mat[8]  * m.mat[3]) + (mat[9]  * m.mat[7]) + (mat[10] * m.mat[11]) + (mat[11] * m.mat[15]);

			mat[12] = (mat[12] * m.mat[0]) + (mat[13] * m.mat[4]) + (mat[14] * m.mat[8] ) + (mat[15] * m.mat[12]);
			mat[13] = (mat[12] * m.mat[1]) + (mat[13] * m.mat[5]) + (mat[14] * m.mat[9] ) + (mat[15] * m.mat[13]);
			mat[14] = (mat[12] * m.mat[2]) + (mat[13] * m.mat[6]) + (mat[14] * m.mat[10]) + (mat[15] * m.mat[14]);
			mat[15] = (mat[12] * m.mat[3]) + (mat[13] * m.mat[7]) + (mat[14] * m.mat[11]) + (mat[15] * m.mat[15]);
		}

		//Matrix Product with Matrix (const-safe)
		RLMatrix* operator*(const RLMatrix& m)const
		{
			RLMatrix *r = new RLMatrix();

			//order of multiplication:
			//1  2  3  4
			//5  6  7  8
			//9  10 11 12
			//13 14 15 16
			r->mat[0]  = (mat[0]  * m.mat[0]) + (mat[1]  * m.mat[4]) + (mat[2]  * m.mat[8] ) + (mat[3]  * m.mat[12]);
			r->mat[1]  = (mat[0]  * m.mat[1]) + (mat[1]  * m.mat[5]) + (mat[2]  * m.mat[9] ) + (mat[3]  * m.mat[13]);
			r->mat[2]  = (mat[0]  * m.mat[2]) + (mat[1]  * m.mat[6]) + (mat[2]  * m.mat[10]) + (mat[3]  * m.mat[14]);
			r->mat[3]  = (mat[0]  * m.mat[3]) + (mat[1]  * m.mat[7]) + (mat[2]  * m.mat[11]) + (mat[3]  * m.mat[15]);

			r->mat[4]  = (mat[4]  * m.mat[0]) + (mat[5]  * m.mat[4]) + (mat[6]  * m.mat[8] ) + (mat[7]  * m.mat[12]);
			r->mat[5]  = (mat[4]  * m.mat[1]) + (mat[5]  * m.mat[5]) + (mat[6]  * m.mat[9] ) + (mat[7]  * m.mat[13]);
			r->mat[6]  = (mat[4]  * m.mat[2]) + (mat[5]  * m.mat[6]) + (mat[6]  * m.mat[10]) + (mat[7]  * m.mat[14]);
			r->mat[7]  = (mat[4]  * m.mat[3]) + (mat[5]  * m.mat[7]) + (mat[6]  * m.mat[11]) + (mat[7]  * m.mat[15]);

			r->mat[8]  = (mat[8]  * m.mat[0]) + (mat[9]  * m.mat[4]) + (mat[10] * m.mat[8] ) + (mat[11] * m.mat[12]);
			r->mat[9]  = (mat[8]  * m.mat[1]) + (mat[9]  * m.mat[5]) + (mat[10] * m.mat[9] ) + (mat[11] * m.mat[13]);
			r->mat[10] = (mat[8]  * m.mat[2]) + (mat[9]  * m.mat[6]) + (mat[10] * m.mat[10]) + (mat[11] * m.mat[14]);
			r->mat[11] = (mat[8]  * m.mat[3]) + (mat[9]  * m.mat[7]) + (mat[10] * m.mat[11]) + (mat[11] * m.mat[15]);

			r->mat[12] = (mat[12] * m.mat[0]) + (mat[13] * m.mat[4]) + (mat[14] * m.mat[8] ) + (mat[15] * m.mat[12]);
			r->mat[13] = (mat[12] * m.mat[1]) + (mat[13] * m.mat[5]) + (mat[14] * m.mat[9] ) + (mat[15] * m.mat[13]);
			r->mat[14] = (mat[12] * m.mat[2]) + (mat[13] * m.mat[6]) + (mat[14] * m.mat[10]) + (mat[15] * m.mat[14]);
			r->mat[15] = (mat[12] * m.mat[3]) + (mat[13] * m.mat[7]) + (mat[14] * m.mat[11]) + (mat[15] * m.mat[15]);

			return r;
		}

		//Matrix Product with Vector (const-safe)
		RLVector* operator*(const RLVector& v)const
		{
			RLVector *r = new RLVector();

			(*r)[0] = (mat[0] * v[0]) + (mat[4] * v[1]) + (mat[8]  * v[2]) + (mat[12] * v[3]);
			(*r)[1] = (mat[1] * v[0]) + (mat[5] * v[1]) + (mat[9]  * v[2]) + (mat[13] * v[3]);
			(*r)[2] = (mat[2] * v[0]) + (mat[6] * v[1]) + (mat[10] * v[2]) + (mat[14] * v[3]);
			(*r)[3] = (mat[3] * v[0]) + (mat[7] * v[1]) + (mat[11] * v[2]) + (mat[15] * v[3]);

			return r;
		}

		//Multiply all elements by a scalar value (const-safe)
		RLMatrix& operator*(const float scalar)const
		{
			RLMatrix *r = new RLMatrix();

			r->mat[0]  = mat[0]  * scalar; r->mat[1]  = mat[4] * scalar; r->mat[2]  = mat[2]  * scalar; r->mat[3]  = mat[3]  * scalar;
			r->mat[4]  = mat[4]  * scalar; r->mat[5]  = mat[5] * scalar; r->mat[6]  = mat[6]  * scalar; r->mat[7]  = mat[7]  * scalar;
			r->mat[8]  = mat[8]  * scalar; r->mat[9]  = mat[6] * scalar; r->mat[10] = mat[10] * scalar; r->mat[11] = mat[11] * scalar;
			r->mat[12] = mat[12] * scalar; r->mat[13] = mat[7] * scalar; r->mat[14] = mat[14] * scalar; r->mat[15] = mat[15] * scalar;

			return *r;
		}
		//Multiply by a scalar value
		void operator*=(const float scalar)
		{
			mat[0]  *= scalar; mat[1]  *= scalar; mat[2]  *= scalar; mat[3]  *= scalar;
			mat[4]  *= scalar; mat[5]  *= scalar; mat[6]  *= scalar; mat[7]  *= scalar;
			mat[8]  *= scalar; mat[9]  *= scalar; mat[10] *= scalar; mat[11] *= scalar;
			mat[12] *= scalar; mat[13] *= scalar; mat[14] *= scalar; mat[15] *= scalar;
		}

		//Overload of the output operator
		friend std::ostream& operator<<(std::ostream& out, const RLMatrix& m)
		{
			out << std::endl;
			out << "{ " << m.mat[0]  << ", " << m.mat[1]  << ", " << m.mat[2]  << ", " << m.mat[3]  << " }" << std::endl;
			out << "{ " << m.mat[4]  << ", " << m.mat[5]  << ", " << m.mat[6]  << ", " << m.mat[7]  << " }" << std::endl;
			out << "{ " << m.mat[8]  << ", " << m.mat[9]  << ", " << m.mat[10] << ", " << m.mat[11] << " }" << std::endl;
			out << "{ " << m.mat[12] << ", " << m.mat[13] << ", " << m.mat[14] << ", " << m.mat[15] << " }" << std::endl;
			out << std::endl;
			return out;
		}
	};

	typedef RLMatrix Matrix44;
}