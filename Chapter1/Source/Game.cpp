#include"Game.h"
namespace origin
{
bool Game::Initialize()
{
	//SDL初期化
	{
		int sr = SDL_Init(SDL_INIT_VIDEO);

		if (sr != 0) {
			SDL_Log("SDL Initialize Failed : %s", SDL_GetError());
			return false;
		}
	}

	//ウィンドウ生成
	{
		m_pWindow = SDL_CreateWindow(
			"Chapter1",
			100,
			100,
			m_kWindowWidth,
			m_kWindowHeight,
			0
		);

		if (!m_pWindow) {
			SDL_Log("CreateWindow Failed: %s", SDL_GetError());
			return false;
		}
	}

	//レンダラー生成
	{
		m_pRenderer = SDL_CreateRenderer(
			m_pWindow,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

		if (!m_pRenderer) {
			SDL_Log("CreateRenderer Failed : %s", SDL_GetError());
			return false;
		}
	}

	//ボール初期化
	for (int i = 0; i < m_kBallNum; i++) {
		BallState ballState;

		ballState.pos = Vector2(m_kWindowWidth / 2.0f, m_kWindowHeight / 2.0f);
		ballState.speed = Vector2(static_cast<float>(rand() % 200 - 100.0f), static_cast<float>(rand() % 200 - 100.0f));

		m_ballStateList.emplace_back(ballState);
	}

	//パドル初期化
	{
		PaddleState paddleState;

		paddleState.pos.x = 10.0f;
		paddleState.pos.y = m_kWindowHeight / 2.0f;

		m_paddleStateList.emplace_back(paddleState);

		paddleState.pos.x = m_kWindowWidth - 10.0f - m_kThickness;
		paddleState.pos.y = m_kWindowHeight / 2.0f;

		m_paddleStateList.emplace_back(paddleState);
	}

	return true;
}

void Game::RunLoop()
{
	while (m_isPlay) {
		ProcessInput();
		Update();
		Draw();
	}
}

void Game::Release()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_isPlay = false;
			break;

		default:
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_ESCAPE]) {
		m_isPlay = false;
	}

	for (auto&& paddleState : m_paddleStateList) {
		paddleState.dir = 0;
	}

	if (keyState[SDL_SCANCODE_W]) {
		m_paddleStateList[0].dir -= 1;
	}
	if (keyState[SDL_SCANCODE_S]) {
		m_paddleStateList[0].dir += 1;
	}

	if (keyState[SDL_SCANCODE_I]) {
		m_paddleStateList[1].dir -= 1;
	}
	if (keyState[SDL_SCANCODE_K]) {
		m_paddleStateList[1].dir += 1;
	}
}

void Game::Update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16));

	float deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	m_ticksCount = SDL_GetTicks();

	for (auto&& paddle : m_paddleStateList) {
		if (paddle.dir != 0) {
			paddle.pos.y += paddle.dir * 300.0f * deltaTime;
			if (paddle.pos.y < (m_kPaddleHeight / 2.0f + m_kThickness)) {
				paddle.pos.y = m_kPaddleHeight / 2.0f + m_kThickness;
			}
			else if (paddle.pos.y > (m_kWindowHeight - m_kPaddleHeight / 2.0f - m_kThickness)) {
				paddle.pos.y = m_kWindowHeight - m_kPaddleHeight / 2.0f - m_kThickness;
			}
		}
	}

	//ボールを動かす
	for (auto&& ball : m_ballStateList) {
		ball.pos.x += ball.speed.x * deltaTime;
		ball.pos.y += ball.speed.y * deltaTime;

		float leftDiff = m_paddleStateList[0].pos.y - ball.pos.y;
		float rightDiff = m_paddleStateList[1].pos.y - ball.pos.y;

		if (
			leftDiff <= (m_kPaddleHeight / 2.0f / 2) &&
			ball.pos.x <= 25.0f && ball.pos.x >= 20.0f &&
			ball.speed.x < 0.0f) {
			ball.speed.x *= -1.0f;
		}
		else if (ball.pos.x <= 0.0f) {
			m_isPlay = false;
		}
		else if (rightDiff <= (m_kPaddleHeight / 2.0f / 2) &&
				 ball.pos.x >= m_kWindowWidth - 25.0f && ball.pos.x <= m_kWindowWidth - 20.0f &&
				 ball.speed.x > 0.0f) {
			ball.speed.x *= -1.0f;
		}
		else if (ball.pos.x >= m_kWindowWidth) {
			m_isPlay = false;
		}

		if (ball.pos.y <= m_kThickness && ball.speed.y < 0.0f) {
			ball.speed.y *= -1;
		}
		else if (ball.pos.y >= (m_kWindowHeight - m_kThickness) &&
				 ball.speed.y > 0.0f) {
			ball.speed.y *= -1;
		}
	}

}

void Game::Draw()
{
	//画面初期化
	{
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 255);
		SDL_RenderClear(m_pRenderer);
	}


	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

	//壁描画
	{
		SDL_Rect wall{ 0,0,1024,m_kThickness };
		SDL_RenderFillRect(m_pRenderer, &wall);

		wall.y = m_kWindowHeight - m_kThickness;
		SDL_RenderFillRect(m_pRenderer, &wall);
	}

	//パドル描画
	for (auto&& paddleState : m_paddleStateList) {
		SDL_Rect paddle{ static_cast<int>(paddleState.pos.x),static_cast<int>(paddleState.pos.y),m_kThickness,m_kPaddleHeight };
		SDL_RenderFillRect(m_pRenderer, &paddle);
	}

	//ボール描画
	for (auto&& ballState : m_ballStateList) {
		Vector2& pos = ballState.pos;

		SDL_Rect ball{ static_cast<int>(pos.x - m_kThickness / 2),static_cast<int>(pos.y - m_kThickness / 2),m_kThickness,m_kThickness };
		SDL_RenderFillRect(m_pRenderer, &ball);
	}

	SDL_RenderPresent(m_pRenderer);
}
}