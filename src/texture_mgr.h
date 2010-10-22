/*
 *  texture_mgr.h
 *  eri
 *
 *  Created by exe on 11/30/09.
 *  Copyright 2009 cobbler. All rights reserved.
 *
 */

#ifndef ERI_TEXTURE_MGR_H
#define ERI_TEXTURE_MGR_H

#include <string>
#include <map>

#include "math_helper.h"

namespace ERI {
	
	struct Color;
	
	enum TextureFilter
	{
		FILTER_NEAREST,
		FILTER_LINEAR
	};
	
	enum TextureWrap
	{
		WRAP_REPEAT,
		WRAP_CLAMP_TO_EDGE,
	};
	
	struct TextureParams
	{
		TextureParams() :
			filter_min(FILTER_NEAREST),
			filter_mag(FILTER_NEAREST),
			wrap_s(WRAP_REPEAT),
			wrap_t(WRAP_REPEAT)
		{
		}
		
		TextureFilter	filter_min, filter_mag;
		TextureWrap		wrap_s, wrap_t;
	};
	
	enum TextureEnvMode
	{
		MODE_REPLACE,
		MODE_MODULATE,
		MODE_DECAL,
		MODE_BLEND,
		MODE_ADD,
		MODE_COMBINE
	};
	
	enum TextureEnvOp
	{
		OP_REPLACE,
		OP_MODULATE,
		OP_ADD,
		OP_ADD_SIGNED,
		OP_INTERPOLATE,
		OP_SUBTRACT,
		
		OP_DOT3_RGB,
		OP_DOT3_RGBA
	};
	
	enum TextureEnvSrc
	{
		SRC_TEXTURE,
		//SRC_TEXTUREn, ?
		SRC_CONSTANT,
		SRC_PRIMARY_COLOR,
		SRC_PREVIOUS
	};
	
	enum TextureEnvOperand
	{
		OPERAND_SRC_COLOR,
		OPERAND_ONE_MINUS_SRC_COLOR,
		OPERAND_SRC_ALPHA,
		OPERAND_ONE_MINUS_SRC_ALPHA
	};
	
	struct TextureEnvs
	{
		TextureEnvs() :
			mode(MODE_MODULATE),
			src0_rgb(SRC_PREVIOUS),
			src1_rgb(SRC_TEXTURE),
			src2_rgb(SRC_CONSTANT),
			src0_alpha(SRC_PREVIOUS),
			src1_alpha(SRC_TEXTURE),
			src2_alpha(SRC_CONSTANT),
			operand0_rgb(OPERAND_SRC_COLOR),
			operand1_rgb(OPERAND_SRC_COLOR),
			operand2_rgb(OPERAND_SRC_COLOR),
			operand0_alpha(OPERAND_SRC_ALPHA),
			operand1_alpha(OPERAND_SRC_ALPHA),
			operand2_alpha(OPERAND_SRC_ALPHA)
		{
		}
		
		TextureEnvMode		mode;
		TextureEnvOp		combine_rgb, combine_alpha;
		TextureEnvSrc		src0_rgb, src1_rgb, src2_rgb,
							src0_alpha, src1_alpha, src2_alpha;
		TextureEnvOperand	operand0_rgb, operand1_rgb, operand2_rgb,
							operand0_alpha, operand1_alpha, operand2_alpha;
		
		bool				is_use_constant_color;
		Color				constant_color;
	};
	
	struct Texture
	{
		Texture(int _id, int _width, int _height);		
		~Texture();
		
		void CopyPixels(const void* _data);
		bool GetPixelColor(Color& out_color, int x, int y) const;
		
		int		id;
		int		width;
		int		height;
		
		void*	data;

		int		bind_frame_buffer;
		
		mutable TextureParams	current_params;
	};
	
	class TextureMgr
	{
	public:
		~TextureMgr();
		
		const Texture* GetTexture(const std::string& resource_path, bool keep_texture_data = false);
		const Texture* CreateTexture(const std::string& name, int width, int height, const void* data);
		void UpdateTexture(Texture* tex, const void* data);

		const Texture* GetTxtTexture(const std::string& txt, const std::string& font_name, float font_size, float w, float h);
		
		const Texture* GenerateRenderToTexture(int width, int height);
		
		void ReleaseTexture(const Texture* texture);
		
	private:
		std::map<std::string, Texture*>	texture_map_;
	};
	
#pragma mark RenderToTexture
	
	class CameraActor;
	
	class RenderToTexture
	{
	public:
		RenderToTexture(int width, int height, CameraActor* render_cam = NULL);
		~RenderToTexture();
		
		void Init();
		void Release();
		void ProcessRender();
		
		void CopyPixels(void* out_copy_pixels);
		
		inline const Texture* texture() { return texture_; }
		
	private:
		int				width_, height_;
		const Texture*	texture_;
		
		CameraActor*	render_cam_;
		
		void*			out_copy_pixels_;
	};
	
}

#endif // ERI_TEXTURE_MGR_H
