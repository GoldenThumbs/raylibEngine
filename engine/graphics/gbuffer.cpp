#include "gbuffer.h"
#include <rlgl.h>

namespace Skiff
{
	SKF_GBuffer::SKF_GBuffer(u32 width, u32 height)
	{
		_width = width;
		_height = height;

		id = rlLoadFramebuffer(_width, _height);

		if (id > 0)
		{
			colorTex.id = rlLoadTexture(NULL, _width, _height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			colorTex.width = _width;
			colorTex.height = _height;
			colorTex.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			colorTex.mipmaps = 1;
			rlTextureParameters(colorTex.id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(colorTex.id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(colorTex.id, RL_TEXTURE_WRAP_S, RL_TEXTURE_WRAP_CLAMP);
			rlTextureParameters(colorTex.id, RL_TEXTURE_WRAP_T, RL_TEXTURE_WRAP_CLAMP);

			specTex.id = rlLoadTexture(NULL, _width, _height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			specTex.width = _width;
			specTex.height = _height;
			specTex.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			specTex.mipmaps = 1;
			rlTextureParameters(specTex.id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(specTex.id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(specTex.id, RL_TEXTURE_WRAP_S, RL_TEXTURE_WRAP_CLAMP);
			rlTextureParameters(specTex.id, RL_TEXTURE_WRAP_T, RL_TEXTURE_WRAP_CLAMP);

			normTex.id = rlLoadTexture(NULL, _width, _height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			normTex.width = _width;
			normTex.height = _height;
			normTex.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			normTex.mipmaps = 1;
			rlTextureParameters(normTex.id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(normTex.id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(normTex.id, RL_TEXTURE_WRAP_S, RL_TEXTURE_WRAP_CLAMP);
			rlTextureParameters(normTex.id, RL_TEXTURE_WRAP_T, RL_TEXTURE_WRAP_CLAMP);

			depthTex.id = rlLoadTextureDepth(_width, _height, false);
			depthTex.width = _width;
			depthTex.height = _height;
			depthTex.format = 19;
			depthTex.mipmaps = 1;
			rlTextureParameters(depthTex.id, RL_TEXTURE_MIN_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(depthTex.id, RL_TEXTURE_MAG_FILTER, RL_TEXTURE_FILTER_NEAREST);
			rlTextureParameters(depthTex.id, RL_TEXTURE_WRAP_S, RL_TEXTURE_WRAP_CLAMP);
			rlTextureParameters(depthTex.id, RL_TEXTURE_WRAP_T, RL_TEXTURE_WRAP_CLAMP);

			rlFramebufferAttach(id, colorTex.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, specTex.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, normTex.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, depthTex.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

			rlEnableFramebuffer(id);
			rlActiveDrawBuffers(3);

			if (rlFramebufferComplete(id))
				TraceLog(LOG_INFO, "GBUFFER: [ID %i] Creation succeeded", id);
			else
				TraceLog(LOG_WARNING, "GBUFFER: [ID %i] Creation Failed", id);

			rlDisableFramebuffer();
		} else
			TraceLog(LOG_WARNING, "GBUFFER: Creation Failed, no id assigned");
	}

	SKF_GBuffer::~SKF_GBuffer()
	{
		Unload();
	}

	void SKF_GBuffer::Begin()
	{
		rlDrawRenderBatchActive();
		rlEnableFramebuffer(id);

		rlClearColor(127, 127, 127, 255);
		rlClearScreenBuffers();

		rlViewport(0, 0, _width, _height);

		rlMatrixMode(RL_PROJECTION);
		rlLoadIdentity();

		rlOrtho(0.0, (double)_width, (double)_height, 0.0, 0.0, 1.0);

		rlMatrixMode(RL_MODELVIEW);
		rlLoadIdentity();

		rlDisableColorBlend();
	}

	void SKF_GBuffer::End(u32 width, u32 height)
	{
		rlDrawRenderBatchActive();
		rlDisableFramebuffer();

		rlViewport(0, 0, width, height);

		rlMatrixMode(RL_PROJECTION);
		rlLoadIdentity();

		rlOrtho(0.0, (double)width, (double)height, 0.0, 0.0, 1.0);

		rlMatrixMode(RL_MODELVIEW);
		rlLoadIdentity();

		rlEnableColorBlend();
	}

	void SKF_GBuffer::Unload()
	{
		rlUnloadTexture(colorTex.id);
		rlUnloadTexture(specTex.id);
		rlUnloadTexture(normTex.id);
		rlUnloadFramebuffer(id);
	}
}