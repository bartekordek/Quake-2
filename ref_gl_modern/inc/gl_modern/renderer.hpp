#pragma once

namespace gl_modern
{

class Renderer
{
public:
	static Renderer &get_instance ()
	{
		static Renderer instance;
		return instance;
	}

	void initialize ();
	void begin_frame ();
	void end_frame ();

private:
	Renderer ();
	~Renderer ();
};

}  // namespace gl_modern