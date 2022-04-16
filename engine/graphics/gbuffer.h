#pragma once

#include "../skiff.h"
#include <raylib.h>

namespace Skiff
{
	class SKF_GBuffer
	{
	private:
		u32 _width;
		u32 _height;
	public:
		u32 id;
		Texture2D colorTex; // Color
		Texture2D specTex; // Specular Information (Spec Color, Gloss)
		Texture2D normTex; // Viewspace Normal
		Texture2D depthTex; // Hardware Depth

		SKF_GBuffer(u32 width, u32 height);

		~SKF_GBuffer();

		void Begin();

		void End(u32 width, u32 height);

		void Unload();
	};
}
