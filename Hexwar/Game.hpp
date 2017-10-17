#ifndef GAME_HPP
#define GAME_HPP

#define MAX_COLORS 8
#define FONTFILE "Fonts/DroidSansMono.ttf"

#include "SDL.h"
#include "AdditionalFunctions.hpp"
#include "Framework.hpp"
#include "Timer.hpp"
#include "Text.hpp"
#include "Hexagon.hpp"

#include <vector>

class CGame
{
public:
	CGame();

	int Init(int screenWidth, int ScreenHeight);
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void ClickEvent(int x, int y);
	void Draw();
	void RenderGUI();
	void playerChangeColorParent(int color);
	int changeColor(vector<CHexagon>* field, int num, int color);
	void nextPlayer();
	void ProcessAI();

	SDL_Renderer *m_pRenderer;
	int m_sWidth;
	int m_sHeight;
	int m_centerX;
	int m_centerY;
	float m_fElapsedTime;

	bool m_bRunning;
	int m_numColors;
	int m_fieldSize;
	int m_hexSize;
	int m_hexNum;
	int m_numPlayers;
	int m_activePlayer;
	int m_playerStart[6];
	int m_points[6];
	float m_times[6];
	bool m_playerPlay[6];
	bool m_playerAI[6];
	bool m_colorInUse[MAX_COLORS];

	SDL_Color m_colors[MAX_COLORS];
	vector<CHexagon> m_hex;
	CText m_UIPoints[6];
	CText m_UINames[6];
	CText m_UIColors[MAX_COLORS];
	string m_names[6];
};

#endif
