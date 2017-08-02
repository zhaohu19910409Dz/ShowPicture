// IS_PixelShader.h

#ifndef __IS_PixelShader_H__
#define __IS_PixelShader_H__

#include <d3d11.h>
#include "IS_Shader.h"

namespace IS
{
	class PixelShader : public Shader
	{
		friend class RenderDevice11;

	public:
		PixelShader();
		virtual ~PixelShader();

		virtual ShaderStage GetStage()
		{
			return SS_Pixel;
		}

		virtual void SetUniformBuffer(class Buffer* buffers, int i = 0);

	private:
		ID3D11PixelShader*		D3DShader;
		class Buffer*			D3DBuffer;
	};
} // namespace IS

#endif // __IS_PixelShader_H__