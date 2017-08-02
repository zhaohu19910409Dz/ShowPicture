// IS_String.h

#ifndef __IS_String_H__
#define __IS_String_H__

#include <windows.h>
#include <tchar.h>

namespace IS
{
	class String
	{
	public:
		String()
		{
			Data = new TCHAR[1];
			memset(Data, 0, sizeof(TCHAR));
			Data[0] = '\0';
		}

		String(TCHAR* t)
		{
			size_t len = _tcslen(t);
			++len;
			if (sizeof(TCHAR) != sizeof(char))
			{
				len *= 2;
			}
			Data = new TCHAR[len];
			memset(Data, 0, sizeof(TCHAR) * len);
			_tcscpy_s(Data, len, t);
		}
		~String() {}

		TCHAR* GetData()
		{
			return Data;
		}

	private:
		TCHAR* Data;
	};
} // namespace IS

#endif // __IS_String_H__