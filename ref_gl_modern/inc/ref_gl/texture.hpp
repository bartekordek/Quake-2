#pragma once

#include "shared/noncopyable.hpp"
#include <string>
#include <cstdint>
#include <array>
#include <memory>
#include <unordered_map>


struct image_s;
typedef image_s image_t;

namespace Q2
{
class Shader;

struct RenderData
{
	std::int32_t id{0};

	std::int32_t x{0};
	std::int32_t y{0};
	std::int32_t w{0};
	std::int32_t h{0};

	std::int32_t imgW{0};
	std::int32_t imgH{0};

	float scale{1.f};

	bool		 alphaTest{false};
	std::int32_t data_type{0};
	std::int32_t format{0};
	std::int32_t internal_format{0};

	void *data{nullptr};
};

struct TextureVertex
{
	float X{0.f};
	float Y{0.f};
	float U{0.f};
	float V{0.f};
};

enum class ColorMode: std::uint8_t
{
	RGBA = 0u,
	RGBM
};

struct PosAndUV
{
	std::array<TextureVertex, 4> Data;
};

class Texture
{
public:
	Texture ();
	Texture (image_t *in_image, const char* in_name);
	void			   init ();
	void			   draw ();
	void			   draw (const RenderData &inData);
	void			   draw (const PosAndUV &in_data);
	void			   changeScale (float in_scale);
	void			   set_has_alpha (bool inHasAlpha);
	bool			   has_alpha () const;
	const std::string &get_path () const;
	void			   set_path (const std::string &inPath);
	void			   set_pos (float in_x, float in_y);
	void			   set_pos_global (float in_x, float in_y);
	void			   fetch_uv_and_apply_them ();
	float			   get_width () const;
	float			   get_height () const;

	~Texture ();

	Q2_NONCOPYABLE (Texture)
protected:
private:
	std::array<float, 32>		 createBufferData (float in_scale) const;
	std::array<float, 32>		 createBufferData (const std::array<TextureVertex, 4> &in_arg) const;
	void						 update_buffer_data ();
	void						 initialize_data ();
	void						 fetch_uv ();
	ColorMode					 m_colorMode{ColorMode::RGBA};
	image_t						*m_image{nullptr};
	std::array<TextureVertex, 4> m_data;
	float						 m_x{-1.f};
	float						 m_y{-1.f};
	float						 m_width{2.f};
	float						 m_height{2.f};
	float						 m_scale{1.0f};
	std::int32_t				 m_id{-1};
	std::string					 m_path;
	bool						 m_hasAlpha{false};
	std::uint32_t				 m_vao{0u};
	std::uint32_t				 m_vbo{0u};
	std::uint32_t				 m_ebo{0u};
	std::unique_ptr<Shader>		 m_shader;
};

class TextureStore
{
public:
	friend Texture;

	static TextureStore &get_instance ();
	Texture				*get (const std::string &inName) const;
	Texture				*get_or_create (const std::string &inName);

protected:
private:
	TextureStore ();
	~TextureStore ();

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textureCache;
};
}  // namespace Q2
