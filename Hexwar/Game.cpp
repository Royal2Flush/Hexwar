#include "Game.hpp"

CGame::CGame()
{
} // Constructor

int CGame::Init( int screenWidth, int ScreenHeight )
{
	// set all class attributes to their default values
	
    m_pRenderer = g_pFramework->GetScreen();

    m_sWidth = screenWidth;
    m_sHeight = ScreenHeight;
	m_numColors = 8;
	m_fieldSize = 10;
	int outerBorder = m_sHeight * (1 - SCALE_FACTOR ) / 2;
	m_hexSize = m_sHeight * SCALE_FACTOR / (m_fieldSize) / 3;
	m_centerY = m_sHeight / 2;
	m_centerX = m_centerY;
	m_hexNum = 3 * (m_fieldSize)* (m_fieldSize - 1) + 1;

	m_colors[0] = { 255, 0, 0, 255 };
	m_colors[1] = { 0, 255, 0, 255 };
	m_colors[2] = { 0, 0, 255, 255 };
	m_colors[3] = { 255, 255, 0, 255 };
	m_colors[4] = { 255, 0, 255, 255 };
	m_colors[5] = { 0, 255, 255, 255 };
	m_colors[6] = { 0, 0, 0, 255 };
	m_colors[7] = { 200, 200, 200, 255 };

    srand((unsigned)time(NULL));
    m_fElapsedTime = 0;

	// create the hexagons
	for (int i = 0; i < m_hexNum; i++)
	{
		CHexagon h;
		h.Init(i, m_fieldSize, rand() % m_numColors, m_centerX, outerBorder, m_hexSize, m_sHeight);
		m_hex.push_back(h);
	}

	m_playerStart[0] = 0;
	m_playerStart[1] = m_hexNum - 1;
	m_playerStart[2] = m_fieldSize - 1;
	m_playerStart[3] = m_hexNum - m_fieldSize;
	m_playerStart[4] = m_hexNum / 2 - m_fieldSize + 1;
	m_playerStart[5] = m_hexNum / 2 + m_fieldSize - 1;

	for (int i = 0; i < 6; i++)
	{
		m_playerPlay[i] = false;
		m_playerAI[i] = false;
		m_points[i] = 0;
		m_times[i] = 0.;
		m_names[i] = "P ";
		m_names[i].append(toString(i));
	}
	m_numPlayers = 0;
	
	// Temporary solution for player number
	int numActive, numHumans;
	cout << "Number of overall players: ";
	cin >> numActive;
	cout << "Number of human players: ";
	cin >> numHumans;

	for (int i = 0; i < numActive; i++)
	{
		m_playerPlay[i] = true;
		m_numPlayers++;
		m_points[i] = 1;
		m_UIPoints[i].Init(FONTFILE, 32, "   1", m_colors[i], 255);
	}
	for (int i = numHumans; i < m_numPlayers; i++)
	{
		m_playerAI[i] = true;
	}
	/*for (int i = 0; i < m_numPlayers; i++)
	{
		if (m_playerPlay[i])
		{
			m_numPlayers++;
			m_points[i] = 1;
		}
		m_UIPoints[i].Init(FONTFILE, 32, "0000", m_colors[i], 255);
	}*/

	m_UIPoints[0].setPos(0, 0);
	m_UIPoints[1].setPos(0, 30);
	m_UIPoints[2].setPos(0, 60);
	m_UIPoints[3].setPos(0, 90);
	m_UIPoints[4].setPos(0, 120);
	m_UIPoints[5].setPos(0, 150);

	do
	{
		m_activePlayer = rand() % 6;
	} while (!m_playerPlay[m_activePlayer]);

	int refWidth = (m_sWidth - m_sHeight) * 0.25;
	int refDist = (m_sWidth - m_sHeight) * 0.1;
	for (int i = 0; i < MAX_COLORS; i++)
	{
		m_UIColors[i].Init(FONTFILE, 32, "FREE", m_colors[i], 255);
		m_UIColors[i].setPos(m_sHeight + 2 * refDist + refWidth, refDist + i*(refDist + refWidth));
	}

	for (int i = 0; i < m_numPlayers; i++)
	{
		if (m_playerPlay[i])
		{
			bool colorUsed = true;
			while (!colorUsed)
			{
				colorUsed = false;
				for (int j = 0; j < i; j++)
				{
					if (m_hex[m_playerStart[j]].getColor() == m_hex[m_playerStart[i]].getColor() && m_playerPlay[j])
					{
						colorUsed = true;
						break;
					}
				}
				m_hex[m_playerStart[i]].setColor(rand() % m_numColors);
			}
			m_colorInUse[m_hex[m_playerStart[i]].getColor()] = true;
			m_UIPoints[i].setColor(m_colors[m_hex[m_playerStart[i]].getColor()], 255);
			m_UIColors[m_hex[m_playerStart[i]].getColor()].setText(m_names[i]);
		}
	}
	
    m_bRunning = true;

    return 0;
}// CGame::Init

void CGame::Quit()
{

}

void CGame::Run()
{
    while( m_bRunning )
    {
		if (m_playerAI[m_activePlayer] && m_playerPlay[m_activePlayer]) // if active player is AI and plays
		{
			ProcessAI();
		}

        ProcessEvents(); // game logic happens in here

        g_pFramework->Update();
        g_pFramework->Clear();
        m_fElapsedTime += g_pTimer->GetElapsed();
		//cout << g_pTimer->GetElapsed() << endl;

        float fpscap = ( 0.0167f - g_pTimer->GetElapsed()) * 1000; // hard-lock to 60 fps; wait remaining time
        if( fpscap > 0 )
        {
            SDL_Delay( fpscap );
        }

        Draw();
        g_pFramework->Render();
    }
}// CGame::Run

void CGame::ProcessEvents()
{
    SDL_Event Event;

    if( SDL_PollEvent( &Event ) ) // is there a new event?
    {
        switch( Event.type ) // which one?
        {
            case ( SDL_QUIT ):
            {
                m_bRunning = false;

            } break;
            case ( SDL_KEYDOWN ):
            {
                switch( Event.key.keysym.sym ) // which key?
                {
                    case ( SDLK_ESCAPE ): // user wants to quit
                    {
                        m_bRunning = false;

                    } break;
					// num keys 1-0 for changing the color of the active player
					case (SDLK_1) : 
					{
									  playerChangeColorParent(0);
					} break;
					case (SDLK_2) :
					{
									  playerChangeColorParent(1);
					} break;
					case (SDLK_3) :
					{
									  playerChangeColorParent(2);
					} break;
					case (SDLK_4) :
					{
									  playerChangeColorParent(3);
					} break;
					case (SDLK_5) :
					{
									  playerChangeColorParent(4);
					} break;
					case (SDLK_6) :
					{
									  playerChangeColorParent(5);
					} break;
					case (SDLK_7) :
					{
									  playerChangeColorParent(6);
					} break;
					case (SDLK_8) :
					{
									  playerChangeColorParent(7);
					} break;
					case (SDLK_9) :
					{
									  playerChangeColorParent(8);
					} break;
					case (SDLK_0) :
					{
									  playerChangeColorParent(9);
					} break;
                }
            } break;
            case ( SDL_MOUSEBUTTONUP ): // does nothing yet
            {
                int mouseX, mouseY;
                SDL_GetMouseState( &mouseX, &mouseY );
                ClickEvent( mouseX, mouseY );
            }
                break;
        }
    }
}// CGame:: ProcessEvents

void CGame::ClickEvent( int x, int y )
{
    // does nothing yet.
	
	// plan: make colors on the right clickable to change color of active player
}

void CGame::playerChangeColorParent(int color)
{
	// change the color of the UI elements of the active player, initializes the board color cascade and calls nextPlayer.
	
	if (!m_colorInUse[color] && !m_playerAI[m_activePlayer])
	{
		m_colorInUse[color] = true;
		m_UIColors[color].setText(m_names[m_activePlayer]);
		m_colorInUse[m_hex[m_playerStart[m_activePlayer]].getColor()] = false;
		m_UIColors[m_hex[m_playerStart[m_activePlayer]].getColor()].setText("FREE");
		
		changeColor(&m_hex, m_playerStart[m_activePlayer], color);
		
		nextPlayer();
	}
} //CGame::playerChangeColorParent

void CGame::ProcessAI()
{
	// finds the move that brings the largest immediate amount of points, changes to that color and calls nextPlayer

	int maxPoints = 0;
	int bestColor = 0;
	for (int i = 0; i < MAX_COLORS; i++) // try all possible colors
	{
		if (!m_colorInUse[i])
		{
			vector<CHexagon> AIField = m_hex; // copy the current field. Important because we can not reverse a color change that absorbs new tiles.
			changeColor(&AIField, m_playerStart[m_activePlayer], i);
			for (int j = 0; j < m_hexNum; j++)
			{
				AIField[j].setHandled(false);
			}
			int p = changeColor(&AIField, m_playerStart[m_activePlayer], i); // how many tiles do we have with the new color?
			cout << "trying color " << i << ": " << p << " points" << endl;
			if (p > maxPoints) // better than the other options. Good, save!
			{
				maxPoints = p;
				bestColor = i;
			}
			if (maxPoints == p) // as good as a previous option. Choose one of the two at random (Note: Has systematic preference for later colors, but not important)
			{
				if (rand() % 2 == 1)
					bestColor = i;
			}
		}
	}
	cout << "Changing color from " << m_hex[m_playerStart[m_activePlayer]].getColor() << " to " << bestColor << endl;
	m_colorInUse[bestColor] = true;
	m_UIColors[bestColor].setText(m_names[m_activePlayer]);
	m_colorInUse[m_hex[m_playerStart[m_activePlayer]].getColor()] = false;
	m_UIColors[m_hex[m_playerStart[m_activePlayer]].getColor()].setText("FREE");

	changeColor(&m_hex, m_playerStart[m_activePlayer], bestColor);

	nextPlayer();
} //CGame::ProcessAI

int CGame::changeColor(vector<CHexagon>* field, int num, int color)
{
	/* changes the color of one tile of the board FIELD at position NUM to color COLOR
	 * recursively calls changeColor for neighboring tiles with the same color that haven't been changed this turn
	 * returns the number of tiles that have been changed in the current (sub)cascade */
	 
	int n = 0; // number of tiles changed in the current (sub)cascade
	(*field)[num].setHandled(true); // we already changed this tile. If we do not do this we jump in an infinite loop between two tiles
	for (int i = 0; i < 6; i++) // check all neighboring tiles
	{
		//cout << num << " - " << i << ": " << m_hex[num].getNeighbor(i) << endl;
		if ((*field)[num].getNeighbor(i) >= 0) // there is a neighbor (as opposed to edge of board)
		{
			if ((*field)[(*field)[num].getNeighbor(i)].getColor() == (*field)[num].getColor() && !(*field)[(*field)[num].getNeighbor(i)].getHandled())
			{ // is the neighbor the same color and not yet handled?
				n += changeColor(field, (*field)[num].getNeighbor(i), color); // make new cascade starting from that neighbor and add the number of tiles changed in there to n
			}
		}
	}
	(*field)[num].setColor(color); // finally change our own color
	return n + 1;
} //CGame::changeColor

void CGame::nextPlayer()
{
	// changes the active player
	
	// reset handled states and change color to our current color to determine points. We do not set points in changeColor because of the AI simulation
	for (int i = 0; i < m_hexNum; i++)
	{
		m_hex[i].setHandled(false);
	}
	m_points[m_activePlayer] = changeColor(&m_hex, m_playerStart[m_activePlayer], m_hex[m_playerStart[m_activePlayer]].getColor());
	
	// reset handled states on the board for the next player
	for (int i = 0; i < m_hexNum; i++)
	{
		m_hex[i].setHandled(false);
	}

	// update points GUI counter
	char buff[100];
	sprintf_s(buff, "%4i", m_points[m_activePlayer]);
	string p = buff;
	m_UIPoints[m_activePlayer].setText(p);
	m_UIPoints[m_activePlayer].setColor(m_colors[m_hex[m_playerStart[m_activePlayer]].getColor()], 255);

	do // cycle through the six players until the next one in the game is found
	{
		m_activePlayer = (m_activePlayer + 1) % 6;
	} while (!m_playerPlay[m_activePlayer]);

	cout << "Active Player: " << m_activePlayer;
	if (m_playerAI[m_activePlayer])
		cout << " (AI)";
	cout << endl;
} //CGame::nextPlayer

void CGame::Draw()
{
	// Draws the board onto the renderer and renders
	
    SDL_SetRenderDrawColor(m_pRenderer, 50, 50, 50, 255);
	SDL_RenderClear(m_pRenderer);
	for (int i = 0; i < m_hexNum; i++)
	{
		m_hex[i].Draw(m_pRenderer, &m_colors[0]);
	}
		
	RenderGUI();
} //CGame::Draw

void CGame::RenderGUI()
{
	// draws the GUI elements onto the renderer
	
	int GUIWidth = m_sWidth - m_sHeight;
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 127);
	SDL_RenderFillRect(m_pRenderer, new SDL_Rect({m_sHeight, 0, GUIWidth, m_sHeight }));

	for (int i = 0; i < 6; i++)
	{
		if (m_playerPlay[i])
		{
			m_UIPoints[i].Render();
		}
	}

	int refWidth = GUIWidth * 0.25;
	int refDist = GUIWidth * 0.1;
	for (int i = 0; i < m_numColors; i++)
	{
		SDL_SetRenderDrawColor(m_pRenderer, m_colors[i]);
		SDL_Rect t = { m_sHeight + refDist, refDist + i*(refDist + refWidth), refWidth, refWidth };
		SDL_RenderFillRect(m_pRenderer, &t);
		m_UIColors[i].Render();
	}
} //CGame::RenderGUI