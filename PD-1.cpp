#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey, LPCTSTR subkeyPath)
{
    TCHAR    achKey[MAX_KEY_LENGTH];
    DWORD    cbName = 0;
    FILETIME ftLastWriteTime;
    DWORD i = 0, retCode = 0;

    // Try to fetch "ImagePath" for the current key
    DWORD dataSize = MAX_PATH;
    TCHAR valueData[MAX_PATH] = { 0 };
    retCode = RegQueryValueEx(hKey, TEXT("ImagePath"), NULL, NULL, (LPBYTE)valueData, &dataSize);

    if (retCode == ERROR_SUCCESS)
    {
        _tprintf(TEXT("ImagePath for %s: %s\n"), subkeyPath, valueData);
    }

    // Enumerate all subkeys
    DWORD cSubKeys = 0;
    RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    for (i = 0; i < cSubKeys; i++)
    {
        cbName = MAX_KEY_LENGTH;
        retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);

        if (retCode == ERROR_SUCCESS)
        {
            HKEY subKey = 0;
            TCHAR newSubkeyPath[MAX_PATH];
            _stprintf_s(newSubkeyPath, TEXT("%s\\%s"), subkeyPath, achKey);

            if (RegOpenKeyEx(hKey, achKey, 0, KEY_READ, &subKey) == ERROR_SUCCESS)
            {
                QueryKey(subKey, newSubkeyPath);
                RegCloseKey(subKey);
            }
        }
    }
}

int __cdecl _tmain()
{
    HKEY hTestKey = 0;
    LPCTSTR rootPath = TEXT("SYSTEM\\CurrentControlSet\\Services");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, rootPath, 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
    {
        QueryKey(hTestKey, rootPath);
        RegCloseKey(hTestKey);
    }

    return 0;
}
