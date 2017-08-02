// IS_RenderDevice11.h

#ifndef __IS_RenderDevice11_H__
#define __IS_RenderDevice11_H__

#include <d3d11.h>
#include "IS_RenderDevice.h"

namespace IS
{
	class RenderDevice11 : public RenderDevice
	{
	public:
		RenderDevice11();
		virtual ~RenderDevice11();

		virtual DeviceType GetType()
		{
			return DT_Direct3D11;
		}

		ErrorCode Initialize(void* hwnd, int w, int h);
		virtual Buffer* CreateBuffer(BufferType type, void* data, int perSize, int count);
		virtual Texture2D* CreateTexture(enum TextureType type, String filename);
		virtual class Shader* CreateShader(enum ShaderStage stage, class String filename);
		virtual void Render(Buffer* vertex, Buffer* index, Texture* tex, Shader* vs, Shader* ps);

	private:
		ID3D11Device*				Device;
		ID3D11DeviceContext*		Context;
		IDXGISwapChain*				SwapChain;
		IDXGIFactory*				DXGIFactory;
		IDXGIAdapter*				Adapter;
		IDXGIOutput*				FullscreenOutput;
		ID3D11RenderTargetView*		RenderTargetView;
		ID3D11Texture2D*			DepthStencilBuffer;
		ID3D11DepthStencilState*	DepthStencilState;
		ID3D11DepthStencilView*		DepthStencilView;
		ID3D11RasterizerState*		RasterState;
		D3D11_VIEWPORT				D3DViewPort;

		HWND						Window;
		int							ScreenWidth;
		int							ScreenHeight;
		bool						IsInitialized;
	};
} // namespace IS

#endif // __IS_RenderDevice11_H__
