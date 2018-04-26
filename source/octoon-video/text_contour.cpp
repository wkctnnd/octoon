#include <octoon/video/text_contour.h>

namespace octoon
{
	namespace video
	{
		TextContour::TextContour() noexcept
		{
		}

		TextContour::TextContour(const math::float3s& points) noexcept
		{
			for (auto& it : points_)
				this->addPoints(it);
		}

		TextContour::TextContour(const math::float3& pt1, const math::float3& pt2, std::uint16_t steps) noexcept
		{
			this->addPoints(pt1, pt2, steps);
		}

		TextContour::TextContour(const math::float3& a, const math::float3& b, const math::float3& c, std::uint16_t bezierSteps) noexcept
		{
			this->addPoints(a, b, c, bezierSteps);
		}

		TextContour::TextContour(const math::float3& a, const math::float3& b, const math::float3& c, const math::float3& d, std::uint16_t bezierSteps) noexcept
		{
			this->addPoints(a, b, c, bezierSteps);
		}

		math::float3&
		TextContour::at(std::size_t index) noexcept
		{
			return points_[index];
		}

		const math::float3&
		TextContour::at(std::size_t index) const noexcept
		{
			return points_[index];
		}

		math::float3s&
		TextContour::points() noexcept
		{
			return points_;
		}

		const math::float3s&
		TextContour::points() const noexcept
		{
			return points_;
		}

		std::size_t
		TextContour::count() const noexcept
		{
			return points_.size();
		}

		void
		TextContour::clear() noexcept
		{
			clockwise_ = false;
			points_.clear();
		}

		void
		TextContour::isClockwise(bool clockwise) noexcept
		{
			clockwise_ = clockwise;
		}

		bool
		TextContour::isClockwise() const noexcept
		{
			return clockwise_;
		}

		void
		TextContour::addPoints(const math::float3& point) noexcept
		{
			points_.push_back(point);
		}

		void
		TextContour::addPoints(const math::float3& pt1, const math::float3& pt2, std::uint16_t steps) noexcept
		{
			this->addPoints(pt1);

			for (std::size_t i = 0; i < steps; i++)
			{
				float t = (float)i / steps;
				this->addPoints(math::lerp(pt1, pt2, t));
			}
		}

		void
		TextContour::addPoints(const math::float3& A, const math::float3& B, const math::float3& C, std::uint16_t bezierSteps) noexcept
		{
			for (std::size_t i = 1; i < bezierSteps; i++)
			{
				float t = (float)i / bezierSteps;
				float t2 = 1.0f - t;

				math::float3 u = t2 * A + t * B;
				math::float3 v = t2 * B + t * C;

				this->addPoints(t2 * u + t * v);
			}
		}

		void
		TextContour::addPoints(const math::float3& A, const math::float3& B, const math::float3& C, const math::float3& D, std::uint16_t bezierSteps) noexcept
		{
			for (std::size_t i = 0; i < bezierSteps; i++)
			{
				float t = (float)i / bezierSteps;
				float t2 = 1.0f - t;

				math::float3 u = t2 * A + t * B;
				math::float3 v = t2 * B + t * C;
				math::float3 w = t2 * C + t * D;

				math::float3 m = t2 * u + t * v;
				math::float3 n = t2 * v + t * w;

				this->addPoints(t2 * m + t * n);
			}
		}
	}
}