#include <windows.h>
#include <stdio.h>

int main() {
    HKEY hKey;

    RegOpenKeyEx(
        HKEY_CURRENT_USER,
        L"Software",   
        0,
        KEY_READ,
        &hKey
    );

    DWORD index = 0;
    wchar_t subKeyName[256];   
    DWORD size;

    while (1) {
        size = 256;

        LONG result = RegEnumKeyEx(
            hKey,
            index,
            subKeyName,
            &size,
            NULL,
            NULL,
            NULL,
            NULL
        );

        if (result == ERROR_NO_MORE_ITEMS)
            break;

        if (result == ERROR_SUCCESS) {
            wprintf(L"%ls\n", subKeyName); 
        }

        index++;
    }

    RegCloseKey(hKey);
    return 0;
}