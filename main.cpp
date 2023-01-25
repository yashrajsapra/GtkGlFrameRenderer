#include "gui.hpp"
#include "program.h"
#include "view.h"
// Application entry point
int
main (int argc, char **argv)
{
	// initProgram();
	// initZVal();
	return gui_init(&argc, &argv) && gui_run() ? 0 : 1;
}
