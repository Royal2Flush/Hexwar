#include "Hexagon.hpp"

CHexagon::CHexagon()
{

}

int CHexagon::Init(int num, int fieldsize, int color, int xcenter, int ycenter, int size, int sHeight)
{
	// sets all the member attributes to default values

	m_number = num;
	m_color = color;
	m_handled = false;
	m_x = -fieldsize + 1;
	int t = 0;
	// where are we (to be rendered)?
	for (int i = -fieldsize + 1; i <= fieldsize - 1; i++)
	{
		int adder = 2 * fieldsize - 1 - abs(i);
		t += adder;
		if (num >= t)
		{
			m_x++;
		}
		else
		{
			t -= adder;
			break;
		}
	}
	m_y = num - t;

	// what are our neighbours?
	if (m_y == 0) // top -> no neighbour in that direction!
	{
		m_neighbors[0] = -1;
	}
	else
	{
		m_neighbors[0] = m_number - 1;
	}
	if (m_y == 2 * fieldsize - 2 - abs(m_x)) // bottom
	{
		m_neighbors[3] = -1;
	}
	else
	{
		m_neighbors[3] = m_number + 1;
	}
	if ( (m_y == 0 && m_x >= 0 ) || m_x == fieldsize - 1) // top right
	{
		m_neighbors[1] = -1;
	}
	else
	{
		m_neighbors[1] = m_number + 2*fieldsize - abs(m_x) - 2 + (m_x < 0);
	}
	if ((m_y == 0 && m_x <= 0) || m_x == -fieldsize + 1) // top left
	{
		m_neighbors[5] = -1;
	}
	else
	{
		m_neighbors[5] = m_number - 2 * fieldsize + abs(m_x) + (m_x <= 0);
	}
	if ((m_y == 2 * fieldsize - 2 - abs(m_x) && m_x >= 0) || m_x == fieldsize - 1) // bottom right
	{
		m_neighbors[2] = -1;
	}
	else
	{
		m_neighbors[2] = m_number + 2 * fieldsize - abs(m_x) - (m_x >= 0);
	}
	if ((m_y == 2 * fieldsize - 2 - abs(m_x) && m_x <= 0) || m_x == -fieldsize + 1) // bottom left
	{
		m_neighbors[4] = -1;
	}
	else
	{
		m_neighbors[4] = m_number - 2 * fieldsize + abs(m_x) + 2 - (m_x > 0);
	}

	// calculate coordinates of our six edges for drawing
	x[0] = xcenter - size + m_x * size * 3 / 2; // left edge
	x[1] = x[0] + size / 2; //top left edge
	x[2] = x[1] + size; //top right edge
	x[3] = x[2] + size / 2; //right edge
	x[4] = x[2]; //bottom right edge
	x[5] = x[1]; //bottom left edge

	y[0] = /*(1-SCALE_FACTOR) * sHeight / 2*/ ycenter + (abs(m_x) + 2 * m_y + 1) * size * SQRT3 / 2; //left edge
	y[1] = y[0] - SQRT3 / 2 * size; //top left edge
	y[2] = y[1]; //top right edge
	y[3] = y[0]; //right edge
	y[4] = y[0] + SQRT3 / 2 * size; //bottom right edge
	y[5] = y[4]; //bottom left edge

	return 0;
} //CHexagon::Init

int CHexagon::Draw(SDL_Renderer *renderer, SDL_Color* colors)
{
	// draws the Hexagon onto the renderer
	
	filledPolygonRGBA(renderer, &x[0], &y[0], 6, colors[m_color].r, colors[m_color].g, colors[m_color].b, colors[m_color].a);
	return 0;
}

void CHexagon::getCoordinates()
{
	// Debug function to print coordinates
	
	std::cout << m_number << " - x: " << m_x << " y: " << m_y << std::endl;
}