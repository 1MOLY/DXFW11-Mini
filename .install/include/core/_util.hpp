//
// Created by Lukas on 11 Nov. 2026.
//
#pragma once
#include <algorithm>
#include <string>
#include <array>
#include <random>


namespace dxfw::util
{
	//! @brief ASCII Helper class
	class ASCII {

	public:

		ASCII() = delete;

		static constexpr int LOWER[2] = { 65, 90 };
		static constexpr int UPPER[2] = { 97, 122 };

		static bool IsASCII(const int v)
		{
			return (v >= LOWER[0] && v <= LOWER[1]) || (v >= UPPER[0] && v <= UPPER[1]);
		}

		static bool IsUpperCase(const int v)
		{
			return v >= UPPER[0] && v <= UPPER[1];
		}

		static bool IsLowerCase(const int v)
		{
			return v >= LOWER[0] && v <= LOWER[1];
		}

	};

	//! @brief Math Helper class
	//! Contains math functions for internal stuff and other bs
	class Math {

	public:

		Math() = delete;

		static std::array<float, 2> PixelToNDC(const std::array<int, 2>& pixel,
											   const std::array<int, 2>& displaySize)
		{
			const auto x =
				static_cast<float>(pixel.at(0)) / static_cast<float>(displaySize.at(0));

			const auto y =
				static_cast<float>(pixel.at(1)) / static_cast<float>(displaySize.at(1));

			return { x * 2.f - 1.f, 1.f - y * 2.f };
		}

	};

	//! @brief String Helper class
	class String {

	public:

		String() = delete;

		static std::string LowRiderString(const std::string& String)
		{
			std::string out = String;

			std::ranges::transform(out,
								   out.begin(),
								   [](const unsigned char c) -> char {
									   return static_cast<char>(std::tolower(c));
								   }
			);

			return out;
		}

		static std::string GenerateClassName(const std::string &WindowTitle)
		{
			if (WindowTitle.empty())
			{
				constexpr int C_MAX = 8;

				std::default_random_engine gen{}; // NOLINT(*-msc51-cpp)
				std::uniform_int_distribution dist(
					ASCII::LOWER[0],
					ASCII::LOWER[1]
				);

				char str[C_MAX]{};

				for (char & i : str)
				{
					int rand{};

					do {
						rand = dist(gen);
					} while (!ASCII::IsASCII(rand));

					i = static_cast<char>(rand);
				}

				return str;
			}

			return LowRiderString(WindowTitle);
		}

	};
}
