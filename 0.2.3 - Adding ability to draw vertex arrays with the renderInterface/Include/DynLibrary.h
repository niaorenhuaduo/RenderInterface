#pragma once

/********************************************************************/
// TODO: Replace error msg box with error logging
/********************************************************************/



/********************************************************************/
// INCLUDES
/********************************************************************/
#include "RI.h"
#include <windows.h>
#include <string>


#pragma warning (disable : 4251)		// for the private std::string


namespace ri 
{

////////////////////////////////////////////////////////////
/// \brief Encapsulates a dynamic library. Used to load libraries and get symbols
///
////////////////////////////////////////////////////////////
class RI_EXPORT DynLibrary {
public:
	////////////////////////////////////////////////////////////
	/// \brief Does nothing, you have to call load before using this class
	///
	////////////////////////////////////////////////////////////
	DynLibrary(const std::string &name);
	~DynLibrary();

	////////////////////////////////////////////////////////////
	/// \brief Loads the library from file
	///
	/// \return true or false, false if the library couldn't be found or couldn't be loaded
	///			In case the loading failed a message window with the error message will appear
	///
	////////////////////////////////////////////////////////////
	bool Load();

	////////////////////////////////////////////////////////////
	/// \brief Unloads the library
	///
	////////////////////////////////////////////////////////////
	void UnLoad();

	////////////////////////////////////////////////////////////
	/// \brief Returns the address to the given symbol
	///
	/// \return On success: handle to the symbol
	///         On failure: NULL
	///
	////////////////////////////////////////////////////////////
	void* GetSymbol(const std::string &symName) const;

	const std::string& GetName() const   { return mName; }
	HINSTANCE		   GetModule() const { return mhInstance; }

private:
	std::string mName;
	HINSTANCE	mhInstance;
};



} // namespace ri