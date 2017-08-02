// IS_Shader.h

#ifndef __IS_Shader_H__
#define __IS_Shader_H__

namespace IS
{
	enum ShaderStage
	{
		SS_Vertex	= 0,
		SS_Geometry = 1,
		SS_Fragment = 2,
		SS_Pixel	= 3,
	};

	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual ShaderStage GetStage() = 0;
		virtual void SetUniformBuffer(class Buffer* buffers, int i = 0) = 0;
	};
} // namespace IS

#endif // __IS_Shader_H__
