#include "..\Include\DynLibrary.h"


namespace ri 
{


DynLibrary::DynLibrary( const std::string &name ) : mName(name), mhInstance(NULL)
{}

DynLibrary::~DynLibrary()
{
	UnLoad();
}

bool DynLibrary::Load()
{
	// In case the Library is already loaded, reload it.
	if(mhInstance) UnLoad();

	if (mName.substr(mName.length() - 4, 4) != ".dll")
		mName += ".dll";

	mhInstance = LoadLibraryA(mName.c_str());

	if(!mhInstance) {
		std::string errormsg = "DynLibrary::Load(): Error loading library: " + mName;
		MessageBoxA(NULL, errormsg.c_str(), "error", MB_OK);
		return false;
	}

	return true;
}

void DynLibrary::UnLoad()
{
	if(!mhInstance) return;

	if(!FreeLibrary(mhInstance)) {
		std::string errormsg = "DynLibrary::Load(): Error releasing library: " + mName;
		MessageBoxA(NULL, errormsg.c_str(), "error", MB_OK);
	}
}

void* DynLibrary::GetSymbol( const std::string &symName ) const
{
	return (void*)GetProcAddress(mhInstance, symName.c_str());
}



} // namespace ri