//ƒQ[ƒ€‚Ìå‚Èˆ—‚ğˆêŠ‡‚ÅŠÇ—‚·‚é
#pragma once
#include "SDL/SDL.h"

namespace origin 
{
struct Vector2
{
	float x;
	float y;

	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}
};

class Game
{
	public:
		bool Initialize();
		void RunLoop();
		void Release();

	private:
		void ProcessInput();
		void Update();
		void Draw();

		struct BallState
		{
			Vector2 pos;
			Vector2 speed;
		};

		struct PaddleState
		{
			Vector2 pos;
			int dir = 0;
		};

		//SDL
		SDL_Window* m_pWindow = nullptr;
		SDL_Renderer* m_pRenderer = nullptr;

		bool m_isPlay = true;

		Uint32 m_ticksCount = 0;

		//Game
		std::vector<BallState> m_ballStateList;
		std::vector<PaddleState> m_paddleStateList;

		//’è”
		static inline constexpr int m_kPaddleHeight = 100;
		static inline constexpr int m_kBallNum = 15;
		static inline constexpr int m_kThickness = 15;
		static inline constexpr int m_kWindowWidth = 1024;
		static inline constexpr int m_kWindowHeight = 768;
};
}
