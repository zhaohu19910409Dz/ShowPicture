// IS_RenderDevice.h

#ifndef __IS_RenderDevice_H__
#define __IS_RenderDevice_H__

#include "IS.h"

namespace IS
{
	enum DeviceType
	{
		DT_Direct3D9	= 0,
		DT_Direct3D11	= 1,
		DT_Opengl20		= 2,
		DT_Opengl30		= 3
	};

	class RenderDevice
	{
	public:
		virtual ~RenderDevice() {}

		virtual DeviceType GetType() = 0;
		virtual ErrorCode Initialize(void* hwnd, int w, int h) = 0;
		virtual class Buffer* CreateBuffer(enum BufferType type, void* data, int perSize, int count) = 0;
		virtual class Texture2D* CreateTexture(enum TextureType type, class String filename) = 0;
		virtual class Shader* CreateShader(enum ShaderStage stage, class String filename) = 0;
		virtual void Render(Buffer* vertex, Buffer* index, class Texture* tex, Shader* vs, Shader* ps) = 0;
	};

} // namespace IS

#endif // __IS_RenderDevice_H__
