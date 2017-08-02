// IS_ConstantBuffer.h

#ifndef __IS_ConstantBuffer_H__
#define __IS_ConstantBuffer_H__

#include <d3d11.h>
#include "IS_Buffer.h"

namespace IS
{
	class ConstantBuffer :public Buffer
	{
		friend class RenderDevice11;

	public:
		ConstantBuffer();
		virtual ~ConstantBuffer();

		virtual int GetSize()
		{
			return perSize;
		}

		virtual int GetCount()
		{
			return BufferCount;
		}

		virtual BufferType GetType()
		{
			return BT_Constant;
		}

		virtual void* Map();
		virtual void UnMap();

	private:
		int				BufferCount;
		int				perSize;
		ID3D11Buffer*	BufferData;
	};
} // namespace IS

#endif // __IS_ConstantBuffer_H__
