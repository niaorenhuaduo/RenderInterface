#include "Core\Application.h"
#include "Debugging\Logger.h"


#ifdef _DEBUG
	#pragma comment(lib, "RId.lib")
#else
	#pragma comment(lib, "RI.lib")
#endif

int WINAPI EngineMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow) {

#ifdef _DEBUG
	  int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	  tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF; // don't free the blocks
	  tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;   // check on each alloc/dealloc			! veeeeeryy slow
	  tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	  _CrtSetDbgFlag(tmpDbgFlag);
#endif

	if(FAILED(gpApp->OnInit(hInstance, "Data\\Options.txt")))
		return E_FAIL;

	int ret = gpApp->Go();	// mainloop

	//_CrtCheckMemory();		// check for leaks
	//_CrtDumpMemoryLeaks();

	return ret;
}