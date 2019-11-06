#pragma once

#include <windows.h>

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		utils.h -	A file that contains all the utility methods for this application
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					static LPCWSTR strToLPCWSTR(const char * content)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		(N/A)
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- Include this header for generic operations such as string conversion.
----------------------------------------------------------------------------------------------------------------------*/
namespace utils {

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	strToLPCWSTR
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	LPCWSTR strToLPCWSTR(const char * content)
	--					char * content:	string content to convert to LPCWSTR
	--
	-- RETURNS:		LPCWSTR
	--
	-- NOTES:
	-- Call this function to convert a string character to a wide byte character.
	----------------------------------------------------------------------------------------------------------------------*/
	static LPCWSTR strToLPCWSTR(const char * content) {
		size_t outSize;
		size_t size = strlen(content) + 1;
		wchar_t* wtext = new wchar_t[size];
		mbstowcs_s(&outSize, wtext, size, content, size - 1);
		return (LPCWSTR)wtext;
	}
}
