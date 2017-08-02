// IS_VertexShader.h

#ifndef __IS_VertexShader_H__
#define __IS_VertexShader_H__

#include <d3d11.h>
#include "IS_Shader.h"

namespace IS
{
	class VertexShader : public Shader
	{
		friend class RenderDevice11;

	public:
		VertexShader();
		virtual ~VertexShader();

		virtual ShaderStage GetStage()
		{
			return SS_Vertex;
		}

		virtual void SetUniformBuffer(class Buffer* buffers, int i = 0);

	private:
		ID3D11VertexShader*		D3DShader;
		ID3D11InputLayout*		VertexIL;
		class Buffer*			D3DBuffer;
	};

} // namespace IS

#endif // __IS_VertexShader_H__
