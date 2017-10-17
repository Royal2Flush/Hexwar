#include "Framework.hpp"
#include "Game.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	int xres = 800;
	int yres = 600;

	if (!g_pFramework->Init(xres, yres, 16, false))
		return -1;

	CGame Game;
	Game.Init(xres, yres);
	Game.Run(); //runs till window closed or "esc" pressed
	Game.Quit();

	//end framework
	g_pFramework->Quit();
	g_pFramework->Del();

	return 0;
}