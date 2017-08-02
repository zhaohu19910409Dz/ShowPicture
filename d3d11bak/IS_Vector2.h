// IS_Vector2.h

#ifndef __IS_Vector2_H__
#define __IS_Vector2_H__

namespace IS
{
	class Vector2
	{
	public:
		Vector2()
		{
			x = 0;
			y = 0;
		}

		Vector2(float x_, float y_)
			: x(x_)
			, y(y_)
		{
		}

	public:
		float x;
		float y;
	};
} // namespace IS

#endif // __IS_Vector2_H__
