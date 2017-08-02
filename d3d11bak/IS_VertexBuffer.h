// IS_VertexBuffer.h

#ifndef __IS_VertexBuffer_H__
#define __IS_VertexBuffer_H__

#include <d3d11.h>
#include "IS_Buffer.h"

namespace IS
{
	class VertexBuffer : public Buffer
	{
		friend class RenderDevice11;

	public:
		VertexBuffer();
		virtual ~VertexBuffer();

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
			return BT_Vertex;
		}

		virtual void* Map();
		virtual void UnMap();

	private:
		int				BufferCount;
		int				perSize;
		ID3D11Buffer*	BufferData;
	};
} // namespace IS

#endif // __IS_VertexBuffer_H__
