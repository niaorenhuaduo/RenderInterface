#pragma once

extern HRESULT AnsiToWideCch( WCHAR* dest, const CHAR* src, int charCount);  
extern HRESULT WideToAnsiCch( CHAR* dest, const WCHAR* src, int charCount);  
extern HRESULT GenericToAnsiCch( CHAR* dest, const TCHAR* src, int charCount); 
extern HRESULT GenericToWideCch( WCHAR* dest, const TCHAR* src, int charCount); 
extern HRESULT AnsiToGenericCch( TCHAR* dest, const CHAR* src, int charCount); 
extern HRESULT WideToGenericCch( TCHAR* dest, const WCHAR* src, int charCount);

extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);



#pragma warning(push)
#pragma warning(disable : 4311)

class HashedString
{
public:
	explicit HashedString( char const * const pIdentString )
		: m_ident( hash_name( pIdentString ) )
		, m_identStr( pIdentString )
	{
	}

	unsigned long getHashValue( void ) const
	{

		return reinterpret_cast<unsigned long>( m_ident );
	}

	const std::string & getStr() const
	{
		return m_identStr;
	}

	static
		void * hash_name( char const *  pIdentStr );

	bool operator< ( HashedString const & o ) const
	{
		bool r = ( getHashValue() < o.getHashValue() );
		return r;
	}

	bool operator== ( HashedString const & o ) const
	{
		bool r = ( getHashValue() == o.getHashValue() );
		return r;
	}

private:

	void *             m_ident;
	std::string		   m_identStr;
};
//Remove the warning for warning #4311...
#pragma warning(pop)