#pragma once

class GL_Legacy
{
public:
	static GL_Legacy &Get_instance ();
	void			  LogNewFrame ();

protected:
private:
	GL_Legacy ();
	~GL_Legacy ();
};