// IS_IndexBuffer.h

#ifndef __IS_IndexBuffer_H__
#define __IS_IndexBuffer_H__

#include <d3d11.h>
#include "IS_Buffer.h"

namespace IS
{
	class IndexBuffer :public Buffer
	{
		friend class RenderDevice11;

	public:
		IndexBuffer();
		virtual ~IndexBuffer();

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
			return BT_Index;
		}

		virtual void* Map();
		virtual void UnMap();

	private:
		int				BufferCount;
		int				perSize;
		ID3D11Buffer*	BufferData;
	};
} // namespace IS

#endif // __IS_IndexBuffer_H__
