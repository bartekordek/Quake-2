#include <GL/glew.h>
#include "gl_modern/texture.hpp"
#include "gl_modern/shader.hpp"
#include "gl_modern/gl_image.h"
#include "gl_modern/gl_draw.h"
#include "gl_modern/gl_local.h"
#include "gl_modern/attribute_meta.hpp"
#include "gl_modern/utils.hpp"

#include <vector>

EXTERNC viddef_t vid;

namespace Q2
{
constexpr float g_float_epsilon = 0.000001f;
bool			float_equal (float a, float b)
{
	return std::abs (a - b) < g_float_epsilon;
}

TextureStore &TextureStore::get_instance ()
{
	static TextureStore s_instance;
	return s_instance;
}

TextureStore::TextureStore ()
{
}

Texture *TextureStore::get (const std::string &inName) const
{
	auto it = m_textureCache.find (inName);
	if (it != m_textureCache.end ())
	{
		return it->second.get ();
	}

	return nullptr;
}

Texture *TextureStore::get_or_create (const std::string &inName)
{
	auto it = m_textureCache.find (inName);
	if (it != m_textureCache.end ())
	{
		return it->second.get ();
	}
	std::unique_ptr<Texture> newTexture = std::make_unique<Texture> ();
	Texture					*result		= newTexture.get ();
	result->set_path (inName);

	m_textureCache[inName] = std::move (newTexture);
	return result;
}

TextureStore::~TextureStore ()
{
}

Texture::Texture ()
{
	init ();
}

Texture::Texture (const char *in_name)
{
	init ();
	glGenTextures (1, &m_id);
	set_name (in_name);
}

Texture::Texture (image_t *in_image, const char *in_name)
{
	m_name	= in_name;
	m_image = in_image;
	m_id	= m_image->texnum;
	init ();
	m_colorMode = ColorMode::RGBM;

	set_name (in_name);
}

void Texture::init ()
{
	initialize_data ();
	const std::array<float, 32> vertices  = createBufferData (m_data);
	unsigned int				indices[] = {
		   0, 1, 2,	 // first triangle
		   1, 3, 0	 // second triangle
	   };

	glGenVertexArrays (1, &m_vao);
	glGenBuffers (1, &m_vbo);
	glGenBuffers (1, &m_ebo);

	glBindVertexArray (m_vao);

	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices.data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

	std::vector<AttributeMeta> attributeInfo;
	attributeInfo.push_back (AttributeMeta ("pos", 0, 3, (int) GL_FLOAT, false, 8 * sizeof (float), nullptr));
	attributeInfo.push_back (AttributeMeta ("col", 1, 3, (int) GL_FLOAT, false, 8 * sizeof (float), (void *) (3 * sizeof (float))));
	attributeInfo.push_back (AttributeMeta ("tex", 2, 2, (int) GL_FLOAT, false, 8 * sizeof (float), (void *) (6 * sizeof (float))));

	for (const auto &att : attributeInfo)
	{
		glVertexAttribPointer (att.index, att.size, att.type, att.normalized, att.stride, att.pointer);
		glEnableVertexAttribArray (att.index);
	}

	m_shader = std::make_unique<Shader> ();
	m_shader->init ("../shaders/basic_texture.vert", "../shaders/basic_texture.frag");
}

void Texture::changeScale (float in_scale)
{
	if (float_equal (m_scale, in_scale))
	{
		return;
	}

	m_scale = in_scale;

	update_buffer_data ();
}

void Texture::set_has_alpha (bool inHasAlpha)
{
	m_hasAlpha = inHasAlpha;
}

bool Texture::has_alpha () const
{
	return false;
}

const std::string &Texture::get_path () const
{
	return m_path;
}

void Texture::set_path (const std::string &inPath)
{
	m_path	= inPath;

	m_image = Draw_FindPic (m_path.c_str ());
	if (m_image)
	{
		const bool hasAlpha =
			((gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION)) && !m_image->has_alpha;
		set_has_alpha (m_image->has_alpha);
		m_id	 = m_image->texnum;
		m_width	 = static_cast<float> (width_to_normalized (m_image->width));
		m_height = static_cast<float> (height_to_normalized (m_image->height));

		fetch_uv ();
	}
	else
	{
		const bool hasAlpha = (gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION);
		set_has_alpha (m_image->has_alpha);
	}

	if ((m_id != -1) && (m_id < 10))
	{
		char buffer[1024u];
		sprintf (buffer, "Texture: %s", inPath.c_str ());
		glObjectLabel (GL_TEXTURE,	// object type
					   m_id,		// OpenGL object name
					   -1,			// null-terminated string
					   buffer);
	}

	set_name (inPath.c_str ());
}

void Texture::set_name (const char *in_name)
{
	char buffer[256u];

	glBindTexture (GL_TEXTURE_2D, m_id);

	sprintf (buffer, "Texture: %s", in_name);
	glObjectLabel (GL_TEXTURE, m_id, -1, buffer);

	sprintf (buffer, "VAO: %s", in_name);
	glObjectLabel (GL_VERTEX_ARRAY, m_vao, -1, buffer);

	sprintf (buffer, "VBO: %s", in_name);
	glObjectLabel (GL_BUFFER, m_vbo, -1, buffer);

	sprintf (buffer, "IBO: %s", in_name);
	glObjectLabel (GL_BUFFER, m_ebo, -1, buffer);

	sprintf (buffer, "Shader: %s", in_name);
	m_shader->set_name (buffer);
}

void Texture::set_pos_global (float in_x, float in_y)
{
	m_x = to_x_normalized (in_x);
	m_y = to_y_normalized (in_y);
	update_buffer_data ();
}

void Texture::set_size (float in_w, float in_h)
{
	m_width	 = width_to_normalized (in_w);
	m_height = height_to_normalized (in_h);
	update_buffer_data ();
}

void Texture::set_pos (float in_x, float in_y)
{
	m_x = in_x;
	m_y = in_y;
	update_buffer_data ();
}

void Texture::fetch_uv_and_apply_them ()
{
	fetch_uv ();
	update_buffer_data ();
}

float Texture::get_width () const
{
	return m_image ? m_image->width : 0.f;
}

float Texture::get_height () const
{
	return m_image ? m_image->height : 0.f;
}

void Texture::draw (const PosAndUV &in_data)
{
	m_data[0].X							 = in_data.Data[0].X;
	m_data[0].Y							 = in_data.Data[0].Y;
	m_data[0].U							 = in_data.Data[0].U;
	m_data[0].V							 = in_data.Data[0].V;

	m_data[1].X							 = in_data.Data[1].X;
	m_data[1].Y							 = in_data.Data[1].Y;
	m_data[1].U							 = in_data.Data[1].U;
	m_data[1].V							 = in_data.Data[1].V;

	m_data[2].X							 = in_data.Data[2].X;
	m_data[2].Y							 = in_data.Data[2].Y;
	m_data[2].U							 = in_data.Data[2].U;
	m_data[2].V							 = in_data.Data[2].V;

	m_data[3].X							 = in_data.Data[3].X;
	m_data[3].Y							 = in_data.Data[3].Y;
	m_data[3].U							 = in_data.Data[3].U;
	m_data[3].V							 = in_data.Data[3].V;

	const std::array<float, 32> vertices = createBufferData (m_data);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (float) * vertices.size (), vertices.data ());

	draw ();
}

void Texture::fetch_uv ()
{
	if (m_image == nullptr)
	{
		return;
	}

	m_width		= width_to_normalized (m_image->width);
	m_height	= height_to_normalized (m_image->height);

	m_data[0].X = m_x + m_width;
	m_data[0].Y = m_y + m_height;
	m_data[0].U = m_image->sl;
	m_data[0].V = m_image->tl;

	m_data[1].X = m_x;
	m_data[1].Y = m_y;
	m_data[1].U = m_image->sh;
	m_data[1].V = m_image->tl;

	m_data[2].X = m_x + m_width;
	m_data[2].Y = m_y;
	m_data[2].U = m_image->sh;
	m_data[2].V = m_image->th;

	m_data[3].X = m_x;
	m_data[3].Y = m_y + m_height;
	m_data[3].U = m_image->sl;
	m_data[3].V = m_image->th;
}

std::array<float, 32> Texture::createBufferData (float in_scale) const
{
	std::array<float, 32> result = {
		// positions               // colors                // texture coords
		1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, in_scale, 0.0f,	   // top right
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		in_scale,  // bottom right
		1.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, in_scale, in_scale,  // bottom left
		-1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,		0.0f	   // top let
	};
	return result;
}

void Texture::update_buffer_data ()
{
	initialize_data ();

	const std::array<float, 32> vertices = createBufferData (m_data);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (float) * vertices.size (), vertices.data ());
}

void Texture::initialize_data ()
{
	// X, Y, U, V
	m_data[0] = TextureVertex{m_x + m_width, m_y + m_height, m_scale, m_scale};
	m_data[1] = TextureVertex{m_x, m_y, 0.f, 0.f};
	m_data[2] = TextureVertex{m_x + m_width, m_y, m_scale, 0.f};
	m_data[3] = TextureVertex{m_x, m_y + m_height, 0.f, m_scale};
}

std::array<float, 32> Texture::createBufferData (const std::array<TextureVertex, 4> &in_arg) const
{
	std::array<float, 32> result = {
		// positions               // colors                // texture coords
		in_arg[0].X, in_arg[0].Y, 0.0f, 1.0f, 0.0f, 0.0f, in_arg[0].U, in_arg[0].V,	 // top right
		in_arg[1].X, in_arg[1].Y, 0.0f, 0.0f, 1.0f, 0.0f, in_arg[1].U, in_arg[1].V,	 // bottom right
		in_arg[2].X, in_arg[2].Y, 0.0f, 0.0f, 0.0f, 1.0f, in_arg[2].U, in_arg[2].V,	 // bottom left
		in_arg[3].X, in_arg[3].Y, 0.0f, 1.0f, 1.0f, 0.0f, in_arg[3].U, in_arg[3].V	 // top let
	};
	return result;
}

void Texture::draw ()
{
	IRenderable::draw ();

	if (has_alpha ())
	{
		glDisable (GL_ALPHA_TEST);
	}

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, m_id);

	// glTexImage2D (GL_TEXTURE_2D, 0, inData.internal_format, inData.imgW, inData.imgH, 0, inData.format, inData.data_type, inData.data);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_shader->use ();
	if (m_colorMode == ColorMode::RGBM)
	{
		m_shader->set_uniform_1i ("mode", 1);
	}

	glBindVertexArray (m_vao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (has_alpha ())
	{
		glEnable (GL_ALPHA_TEST);
	}
}

void Texture::draw (const RenderData &inData)
{
	IRenderable::draw ();

	if (has_alpha ())
	{
		glDisable (GL_ALPHA_TEST);
	}

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, m_id);

	// internal_format = GL_COLOR_INDEX8_EXT = 0x80E5
	// format = GL_COLOR_INDEX 0x1900 =
	glTexImage2D (GL_TEXTURE_2D, 0, inData.internal_format, inData.imgW, inData.imgH, 0, inData.format, inData.data_type, inData.data);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_shader->use ();
	glBindVertexArray (m_vao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (has_alpha ())
	{
		glEnable (GL_ALPHA_TEST);
	}
}

Texture::~Texture ()
{
}

}  // namespace Q2