/*
 *  xml_helper.cpp
 *  eri
 *
 *  Created by exe on 2010/10/26.
 *  Copyright 2010 cobbler. All rights reserved.
 *
 */

#include "pch.h"

#include "xml_helper.h"

#include <fstream>
#include "rapidxml_print.hpp"

#include "platform_helper.h"
#include "math_helper.h"

namespace ERI
{
	bool ParseFile(const std::string& path, XmlParseData& out_data)
	{
		ASSERT(!out_data.buffer);

		std::string real_path(path);

#if ERI_PLATFORM == ERI_PLATFORM_WIN
		if (path[1] != ':')
			real_path = GetResourcePath() + std::string("\\") + path;
#else
		if (path[0] != '/')
			real_path = GetResourcePath() + std::string("/") + path;
#endif
		
		std::ifstream ifs;
		ifs.open(real_path.c_str(), std::ios::binary);
		
		if (ifs.fail())
		{
			LOGW("xml parse file %s error!", real_path.c_str());
			return false;
		}
		
		// get length of file:
		ifs.seekg(0, std::ios::end);
		long long length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		
		// allocate memory:
		out_data.buffer = new char[length + 1];
		
		// read data as a block:
		ifs.read(out_data.buffer, static_cast<std::streamsize>(length));
		ifs.close();
		
		out_data.buffer[length] = 0;
		
		out_data.doc.parse<0>(out_data.buffer);
		
		return true;
	}

	rapidxml::xml_attribute<>* GetAttrBool(rapidxml::xml_node<>* node, const char* name, bool& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			out_value = (strcmp(attr->value(), "true") == 0);
			return attr;
		}
		
		return NULL;
	}

	rapidxml::xml_attribute<>* GetAttrInt(rapidxml::xml_node<>* node, const char* name, int& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			out_value = atoi(attr->value());
			return attr;
		}
		
		return NULL;
	}

	rapidxml::xml_attribute<>* GetAttrFloat(rapidxml::xml_node<>* node, const char* name, float& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			out_value = static_cast<float>(atof(attr->value()));
			return attr;
		}
		
		return NULL;
	}

	rapidxml::xml_attribute<>* GetAttrDouble(rapidxml::xml_node<>* node, const char* name, double& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			out_value = atof(attr->value());
			return attr;
		}
		
		return NULL;
	}

	rapidxml::xml_attribute<>* GetAttrStr(rapidxml::xml_node<>* node, const char* name, std::string& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			out_value = attr->value();
			return attr;
		}
		
		return NULL;
	}
	
	rapidxml::xml_attribute<>* GetAttrVector2(rapidxml::xml_node<>* node, const char* name, Vector2& out_value)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			std::string s = attr->value();
			size_t pos = s.find(',');
			
			if (pos != std::string::npos)
			{
				out_value.x = static_cast<float>(atof(s.substr(0, pos).c_str()));
				out_value.y = static_cast<float>(atof(s.substr(pos + 1).c_str()));
			}
			
			return attr;
		}
		
		return NULL;	
	}

	rapidxml::xml_attribute<>* GetAttrColor(rapidxml::xml_node<>* node, const char* name, Color& out_value, bool base_255 /*= false*/)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			std::string s = attr->value();
			size_t pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_value.r = static_cast<float>(atof(s.c_str()));
				if (base_255) out_value.r /= 255;
				return attr;
			}
			else
			{
				out_value.r = static_cast<float>(atof(s.substr(0, pos).c_str()));
				if (base_255) out_value.r /= 255;
			}

			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_value.g = static_cast<float>(atof(s.c_str()));
				if (base_255) out_value.g /= 255;
				return attr;
			}
			else
			{
				out_value.g = static_cast<float>(atof(s.substr(0, pos).c_str()));
				if (base_255) out_value.g /= 255;
			}
			
			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_value.b = static_cast<float>(atof(s.c_str()));
				if (base_255) out_value.b /= 255;
				return attr;
			}
			else
			{
				out_value.b = static_cast<float>(atof(s.substr(0, pos).c_str()));
				if (base_255) out_value.b /= 255;
			}

			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_value.a = static_cast<float>(atof(s.c_str()));
				if (base_255) out_value.a /= 255;
			}
			else
			{
				out_value.a = static_cast<float>(atof(s.substr(0, pos).c_str()));
				if (base_255) out_value.a /= 255;
			}
			
			return attr;
		}
		
		return NULL;	
	}
	
	rapidxml::xml_attribute<>* GetAttrRect(rapidxml::xml_node<>* node, const char* name, Vector2& out_left_top, Vector2& out_right_bottom)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr)
		{
			std::string s = attr->value();
			size_t pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_left_top.x = static_cast<float>(atof(s.c_str()));
				return attr;
			}
			else
			{
				out_left_top.x = static_cast<float>(atof(s.substr(0, pos).c_str()));
			}
			
			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_left_top.y = static_cast<float>(atof(s.c_str()));
				return attr;
			}
			else
			{
				out_left_top.y = static_cast<float>(atof(s.substr(0, pos).c_str()));
			}
			
			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_right_bottom.x = static_cast<float>(atof(s.c_str()));
				return attr;
			}
			else
			{
				out_right_bottom.x = static_cast<float>(atof(s.substr(0, pos).c_str()));
			}		
			
			s = s.substr(pos + 1);
			pos = s.find(',');
			if (pos == std::string::npos)
			{
				out_right_bottom.y = static_cast<float>(atof(s.c_str()));
			}
			else
			{
				out_right_bottom.y = static_cast<float>(atof(s.substr(0, pos).c_str()));
			}
			
			return attr;
		}

		return NULL;
	}
	
	bool SaveFile(const std::string& path, const XmlCreateData& data)
	{
		std::string s;
		rapidxml::print(std::back_inserter(s), data.doc, 0);
		
		std::ofstream ofs;
		ofs.open(path.c_str());
		
		if (ofs.fail())
		{
			LOGW("xml save file %s error!", path.c_str());
			return false;
		}
		
		ofs << s;
		ofs.close();
		
		return true;
	}
	
	rapidxml::xml_node<>* CreateNode(rapidxml::xml_document<>& doc, const char* name)
	{
		char* alloc_name = doc.allocate_string(name);
		return doc.allocate_node(rapidxml::node_element, alloc_name);
	}
	
	void PutAttrBool(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, bool value)
	{
		char* alloc_name = doc.allocate_string(name);
		char* alloc_value = doc.allocate_string(value ? "true" : "false");
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}
	
	void PutAttrInt(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, int value)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[32];
		sprintf(buf, "%d", value);
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}
	
	void PutAttrFloat(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, float value)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[32];
		sprintf(buf, "%g", value);
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}

	void PutAttrDouble(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, double value)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[32];
		sprintf(buf, "%g", value);
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}

	void PutAttrStr(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, const std::string& value)
	{
		char* alloc_name = doc.allocate_string(name);
		char* alloc_value = doc.allocate_string(value.c_str());
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}
	
	void PutAttrVector2(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, const Vector2& value)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[32];
		sprintf(buf, "%g,%g", value.x, value.y);
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}
	
	void PutAttrColor(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, const Color& value, bool base_255 /*= false*/)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[64];
		
		if (base_255)
		{
			sprintf(buf, "%d,%d,%d,%d",
					static_cast<int>(value.r * 255 + 0.5f),
					static_cast<int>(value.g * 255 + 0.5f),
					static_cast<int>(value.b * 255 + 0.5f),
					static_cast<int>(value.a * 255 + 0.5f));
		}
		else
		{
			sprintf(buf, "%g,%g,%g,%g", value.r, value.g, value.b, value.a);
		}
		
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}

	void PutAttrRect(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, const Vector2& left_top, const Vector2& right_bottom)
	{
		char* alloc_name = doc.allocate_string(name);
		char buf[64];
		sprintf(buf, "%g,%g,%g,%g", left_top.x, left_top.y, right_bottom.x, right_bottom.y);
		char* alloc_value = doc.allocate_string(buf);
		rapidxml::xml_attribute<>* attr = doc.allocate_attribute(alloc_name, alloc_value);
		node->append_attribute(attr);
	}
}
