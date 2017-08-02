// IS_Texture2D.h

#ifndef __IS_Texture2D_H__
#define __IS_Texture2D_H__

#include <d3d11.h>
#include "IS_Texture.h"

namespace IS
{
	class Texture2D : public Texture
	{
		friend class RenderDevice11;

	public:
		Texture2D();
		virtual ~Texture2D();

		virtual TextureType GetType()
		{
			return TT_Texture2D;
		}

		virtual TextureFormat GetFormat()
		{
			return Format;
		}

		virtual int GetWidth()
		{
			return Width;
		}

		virtual int GetHeight()
		{
			return Height;
		}

		virtual int GetDepth()
		{
			return -1;
		}

		virtual void* Map();
		virtual void UnMap();

	private:
		int							Width;
		int							Height;
		int							Samples;
		TextureFormat				Format;

		ID3D11Texture2D*			Tex;
		ID3D11ShaderResourceView*	TexSv;
		ID3D11RenderTargetView*		TexRtv;
		ID3D11DepthStencilView*		TexDsv;
	};
} // namespace IS

#endif // __IS_Texture2D_H__
