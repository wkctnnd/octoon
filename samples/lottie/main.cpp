#include <octoon/octoon.h>
#include <octoon/octoon-c.h>
#include <octoon/runtime/json.h>
#include <octoon/transform_anim_component.h>

#include <fstream>

#include "transform_helper.h"

class LottieApp
{
	using json = octoon::runtime::json;

	enum class LayerTypes 
	{
		precomp = 0,
		solid = 1,
		still = 2,
		nullLayer = 3,
		shape = 4,
		text = 5,
		audio = 6,
		pholderVideo = 7,
		imageSeq = 8,
		video = 9,
		pholderStill = 10,
		guide = 11,
		adjustment = 12,
		camera = 13,
		light = 14
	};

public:
	LottieApp()
	{
	}

	bool prepareCanvas(const json& j)
	{
		width_ = j["w"];
		height_ = j["h"];
		fps_ = j["fr"];

		return ::OctoonOpenWindow("Octoon Studio", width_, height_);
	}

	bool prepareAssets(const json& j)
	{
		for (auto& asset : j["assets"])
		{
			auto id = asset["id"].get<json::string_t>();

			if (id.compare(0, 5, "image") == 0)
			{
				auto u = "file:C:\\Users\\Administrator\\Desktop\\" + asset["u"].get<json::string_t>();
				auto p = asset["p"].get<json::string_t>();

				_textures[id] = octoon::GamePrefabs::instance()->createTexture(u + p);
			}
			else if (id.compare(0, 4, "comp") == 0)
			{

			}
		}

		return true;
	}

	bool prepareLayers(const json& j)
	{
		camera_ = octoon::GamePrefabs::instance()->createCamera2D();
		camera_->getComponent<octoon::CameraComponent>()->setClearColor(octoon::math::float4(0.0f, 0.0f, 0.0f, 1.0));

		for (auto& layer : j["layers"])
		{
			octoon::GameObjectPtr object;

			auto type = (LayerTypes)layer["ty"].get<json::number_unsigned_t>();
			switch (type)
			{
			case LayerTypes::precomp:
			break;
			case LayerTypes::solid:
			break;
			case LayerTypes::still:
			{
				auto refid = layer["refId"].get<json::string_t>();
				auto tex = _textures[refid];
				object = octoon::GamePrefabs::instance()->createSprite(tex, tex->getTextureDesc().getWidth(), tex->getTextureDesc().getHeight());
				object->getComponent<octoon::Transform>()->setLocalScale(octoon::math::float3(tex->getTextureDesc().getWidth(), tex->getTextureDesc().getHeight(), 1.0f));
			}
			break;
			case LayerTypes::nullLayer:
			break;
			case LayerTypes::shape:
			break;
			case LayerTypes::text:
			{
				auto& t = layer["t"]["d"]["k"][0]["s"];
				auto& fc = t["fc"];
				auto text = t["t"].get<json::string_t>();
				auto size = t["s"].get<json::number_unsigned_t>();
				auto color = octoon::math::float4(fc[0].get<json::number_float_t>(), fc[1].get<json::number_float_t>(), fc[2].get<json::number_float_t>(), 1.0f);
				
				object = octoon::GamePrefabs::instance()->createText(text.c_str(), size);
				object->getComponent<octoon::RenderComponent>()->getMaterial()->getParameter("color")->uniform4f(color);
			}
			break;
			case LayerTypes::audio:
			break;
			case LayerTypes::pholderVideo:
			break;
			case LayerTypes::imageSeq:
			break;
			case LayerTypes::video:
			break;
			case LayerTypes::pholderStill:
			break;
			case LayerTypes::guide:
			break;
			case LayerTypes::adjustment:
			break;
			case LayerTypes::camera:
			break;
			case LayerTypes::light:
			break;
			default:
				break;
			}

			auto& ks = layer["ks"];
			if (!ks.is_null())
			{
				TransformHelper t(ks, fps_);

				auto transform = object->addComponent<octoon::TransformAnimComponent>();
				transform->setTranslate(t.pos);
				transform->setScale(t.scale);
				transform->setRotation(t.rotation);
			}

			layers_.push_back(std::move(object));
		}

		return true;
	}

private:
	float fps_;
	std::uint32_t width_, height_;

	octoon::GameObjects layers_;
	octoon::GameObjectPtr camera_;
	std::map<std::string, octoon::graphics::GraphicsTexturePtr> _textures;
};

int main(int argc, const char* argv[])
{
	if (!::OctoonInit(argv[0], ""))
		return 1;

	auto json = octoon::runtime::json::parse(std::ifstream("C:\\Users\\Administrator\\Desktop\\data.json"));

	LottieApp app;

	if (app.prepareCanvas(json))
	{
		app.prepareAssets(json);
		app.prepareLayers(json);

		::OctoonMainLoop();
	}

	::OctoonTerminate();
	return 0;
}