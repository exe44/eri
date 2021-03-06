/*
 *  texture_reader_libpng.h
 *  eri
 *
 *  Created by exe on 10/19/10.
 *  Copyright 2010 cobbler. All rights reserved.
 *
 */

#ifndef ERI_TEXTURE_READER_LIBPNG_H
#define ERI_TEXTURE_READER_LIBPNG_H

#include "pch.h"

#ifdef ERI_TEXTURE_READER_LIBPNG

#include "texture_reader.h"

#include <string>

struct FIBITMAP;

namespace ERI {
	
	class TextureReaderLibPNG : public TextureReader
	{
	public:
		TextureReaderLibPNG(const std::string& path, bool generate_immediately);
		virtual ~TextureReaderLibPNG();
		
		virtual void Generate();
		
		virtual void* texture_data() { return texture_data_; }
		
	private:
		void* texture_data_;
	};
	
}

#endif // ERI_TEXTURE_READER_LIBPNG

#endif // ERI_TEXTURE_READER_LIBPNG_H