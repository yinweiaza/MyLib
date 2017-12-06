#pragma once

#include "../P2PServer/P2PServerInterface.h"

#ifdef DEBUG
	#pragma comment(lib, "P2PServerD.lib")
#else
	#pragma comment(lib, "P2PServer.lib")
#endif