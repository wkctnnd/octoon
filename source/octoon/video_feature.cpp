#if defined(OCTOON_FEATURE_VIDEO_ENABLE)
#include <octoon/video_feature.h>
#include <octoon/video/renderer.h>
#include <octoon/video/render_scene.h>

#include <octoon/input_feature.h>
#include <octoon/input/input_event.h>

#include <octoon/game_app.h>
#include <octoon/game_server.h>
#include <octoon/game_listener.h>

#include <octoon/hal_feature.h>
#include <octoon/runtime/except.h>

namespace octoon
{
	OctoonImplementSubClass(VideoFeature, GameFeature, "VideoFeature")

	VideoFeature::VideoFeature() noexcept
		: framebuffer_w_(0)
		, framebuffer_h_(0)
		, camera_(nullptr)
	{
	}

	VideoFeature::VideoFeature(std::uint32_t framebuffer_w, std::uint32_t framebuffer_h) noexcept
		: framebuffer_w_(framebuffer_w)
		, framebuffer_h_(framebuffer_h)
		, camera_(nullptr)
	{
	}

	VideoFeature::~VideoFeature() noexcept
	{
	}

	void
	VideoFeature::setMainCamera(CameraComponent* camera) noexcept
	{
		if (camera_ != camera)
		{
			if (camera_ && camera)
				camera_->setCameraType(CameraType::Custom);
			camera_ = camera;
		}
	}
	
	CameraComponent*
	VideoFeature::getMainCamera() const noexcept
	{
		return camera_;
	}

	void
	VideoFeature::setFramebufferScale(std::uint32_t w, std::uint32_t h) noexcept
	{
		if (framebuffer_w_ != w || framebuffer_h_ != h)
		{
			framebuffer_w_ = w;
			framebuffer_h_ = h;

			video::Renderer::instance()->setFramebufferSize(w, h);
		}
	}

	void
	VideoFeature::getFramebufferScale(std::uint32_t& w, std::uint32_t& h) noexcept
	{
		video::Renderer::instance()->getFramebufferSize(w, h);
	}

	const hal::GraphicsFramebufferPtr&
	VideoFeature::getFramebuffer() const noexcept
	{
		return video::Renderer::instance()->getFramebuffer();
	}

	void
	VideoFeature::setGlobalIllumination(bool enable) noexcept
	{
		video::Renderer::instance()->setGlobalIllumination(enable);
	}

	bool
	VideoFeature::getGlobalIllumination() const noexcept
	{
		return video::Renderer::instance()->getGlobalIllumination();
	}

	void
	VideoFeature::setOverrideMaterial(const std::shared_ptr<material::Material>& material) noexcept
	{
		video::Renderer::instance()->setOverrideMaterial(material);
	}

	std::shared_ptr<material::Material>
	VideoFeature::getOverrideMaterial() const noexcept
	{
		return video::Renderer::instance()->getOverrideMaterial();
	}

	void
	VideoFeature::onActivate() except
	{
		auto graphics = this->getFeature<GraphicsFeature>();
		if (graphics)
		{
			video::Renderer::instance()->setup(graphics->getContext(), framebuffer_w_, framebuffer_h_);

			this->addMessageListener("feature:input:event", std::bind(&VideoFeature::onInputEvent, this, std::placeholders::_1));
		}
		else
		{
			throw runtime::runtime_error::create("failure to get the HAL from features");
		}
	}

	void
	VideoFeature::onDeactivate() noexcept
	{
		this->removeMessageListener("feature:input:event", std::bind(&VideoFeature::onInputEvent, this, std::placeholders::_1));
		video::Renderer::instance()->close();
	}

	void
	VideoFeature::onInputEvent(const std::any& data) noexcept
	{
		auto event = std::any_cast<input::InputEvent>(data);
		switch (event.event)
		{
		case input::InputEvent::SizeChange:
		case input::InputEvent::SizeChangeDPI:
			if (event.change.w > 0 && event.change.h > 0)
				video::Renderer::instance()->setFramebufferSize(event.change.w, event.change.h);
			break;
		default:
			return;
		}
	}

	void
	VideoFeature::onFrameBegin() noexcept
	{
	}

	void
	VideoFeature::onFrame() noexcept
	{
		try
		{
			video::Renderer::instance()->render(*video::RenderScene::instance());
		}
		catch (const std::exception& e)
		{
			this->getGameListener()->onMessage(e.what());
		}
	}

	void
	VideoFeature::onFrameEnd() noexcept
	{
	}
}
#endif