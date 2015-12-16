#pragma once

/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include <string>


namespace ri
{


#define Log(str, ...) LogFile::LogText(false, str, __VA_ARGS__);
#define LogE(str, ...) LogFile::LogText(true, str, __VA_ARGS__);
	


class LogFile : private UnCopyable {
public:
	static void Init(const std::string &file) { 
		assert(file.c_str() > 0);
		mpFile = fopen(file.c_str(), "w");
		assert(mpFile && "Couldn't open LogFile!");
	}
	static void	Release() { if(mpFile) { fflush(mpFile); fclose(mpFile); } }

	static void LogText(bool bError, const char* chString, ...) {
	    assert(mpFile && "Log not initialized");
		static char res[2048];
	    static char ch[2048];
	    char *pArgs;
   
		if(bError) 
			sprintf(res, "#ERROR#|\0");
		else sprintf(res, "#INFO#|\0");

	    pArgs = (char*) &chString + sizeof(chString);
	    vsprintf(ch, chString, pArgs);
		strcat(res, ch);
	    fprintf(mpFile, res);
   
	    #ifdef _DEBUGFLUSH_
	    fflush(mpFile);
	    #endif
	}

private:
	static FILE *mpFile;

	LogFile() {}
	~LogFile() {}
};



} // namespace ri