#ifndef HEXAGON_HPP
#define HEXAGON_HPP

#define SCALE_FACTOR 0.75
#define SQRT3 1.732050807

#include "AdditionalFunctions.hpp"
#include "SDL2_gfxPrimitives.h"

class CHexagon
{
public:
	CHexagon();
	int Init(int num, int fieldsize, int color, int xcenter, int ycenter, int size, int sHeight);
	int Draw(SDL_Renderer *renderer, SDL_Color* colors);
	void setColor(int color) { m_color = color; }
	bool getHandled() { return m_handled; }
	void setHandled(bool handled) { m_handled = handled; }
	int getColor() { return m_color; }
	void getCoordinates();
	int getNeighbor(int n) { return m_neighbors[n]; }
	int getX() {return m_x; }
	int getY() { return m_y; }

private:
	int m_x;
	int m_y;
	int m_number;
	int m_color;
	int m_neighbors[6];
	Sint16 x[6], y[6];
	bool m_handled;
};

#endif