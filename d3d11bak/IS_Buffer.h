// IS_Buffer.h

#ifndef __IS_Buffer_H__
#define __IS_Buffer_H__

namespace IS
{
	enum BufferType
	{
		BT_Vertex	= 0,
		BT_Index	= 1,
		BT_Constant = 2,
	};

	class Buffer
	{
	public:
		virtual int GetSize() = 0;
		virtual int GetCount() = 0;
		virtual BufferType GetType() = 0;
		virtual void* Map() = 0;
		virtual void UnMap() = 0;
	};
} // namespace IS

#endif
