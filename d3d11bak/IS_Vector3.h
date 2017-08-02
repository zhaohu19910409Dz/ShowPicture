// IS_Vector3.h

#ifndef __IS_Vector3_H__
#define __IS_Vector3_H__

namespace IS
{
	class Vector3
	{
	public:
		Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3(float x_, float y_, float z_)
			: x(x_)
			, y(y_)
			, z(z_)
		{
		}

	public:
		float x;
		float y;
		float z;
	};
}

#endif // __IS_Vector3_H__
