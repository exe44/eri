/*
 *  texture_reader.h
 *  eri
 *
 *  Created by exe on 11/30/09.
 *  Copyright 2009 cobbler. All rights reserved.
 *
 */

#ifndef ERI_TEXTURE_READER_H
#define ERI_TEXTURE_READER_H

#include <stddef.h>

namespace ERI {

	class TextureReader
	{
	public:
		TextureReader(bool generate_immediately) : width_(-1), height_(-1), texture_id_(0), alpha_premultiplied_(false) {}
		virtual ~TextureReader() {}
		
		virtual void Generate() {}
		
		inline int width() { return width_; }
		inline int height() { return height_; }
		
		inline unsigned int texture_id() { return texture_id_; }
		
		inline bool alpha_premultiplied() { return alpha_premultiplied_; }
		
		virtual void* texture_data() { return NULL; }
		
	protected:
		int			width_;
		int			height_;
		
		unsigned int	texture_id_;
		
		bool	alpha_premultiplied_;
	};
}

#endif // ERI_TEXTURE_READER_H
