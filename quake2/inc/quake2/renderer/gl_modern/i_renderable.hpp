#pragma once

#include "shared/noncopyable.hpp"

namespace Q2
{
class IRenderable
{
public:
	IRenderable ()			   = default;

	virtual ~IRenderable ()	   = default;
	virtual void draw ();


	Q2_NONCOPYABLE (IRenderable)
protected:
private:
};
}  // namespace Q2
