#ifndef _VECTOR_3D
#define _VECTOR_3D

class Vector3d
{
	public:
		float x;
		float y;
		float z;

		Vector3d();
		Vector3d(float x, float y, float z);
		~Vector3d();

		Vector3d operator+(Vector3d vec);
		Vector3d operator-(Vector3d vec);
		Vector3d operator*(float factor);
		Vector3d operator/(float div);

		float length();
		Vector3d rotate(float angle);
};

#endif
