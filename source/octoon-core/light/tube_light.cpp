#include <octoon/light/tube_light.h>

namespace octoon::light
{
	OctoonImplementSubClass(TubeLight, Light, "TubeLight")

	TubeLight::TubeLight() noexcept
	{
	}

	TubeLight::~TubeLight() noexcept
	{
	}

	std::shared_ptr<video::RenderObject>
	TubeLight::clone() const noexcept
	{
		auto light = std::make_shared<TubeLight>();
		return light;
	}
}