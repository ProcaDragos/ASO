#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <map>
#include <set>

#pragma comment(lib, "psapi.lib")

int main() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        std::cout << "Eroare la deschiderea SCM\n";
        return 1;
    }

    DWORD bytesNeeded = 0, servicesCount = 0, resumeHandle = 0;


    EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        NULL,
        0,
        &bytesNeeded,
        &servicesCount,
        &resumeHandle,
        NULL
    );

    BYTE* buffer = new BYTE[bytesNeeded];

    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        buffer,
        bytesNeeded,
        &bytesNeeded,
        &servicesCount,
        &resumeHandle,
        NULL)) {

        std::cout << "Eroare la enumerarea serviciilor\n";
        delete[] buffer;
        CloseServiceHandle(hSCManager);
        return 1;
    }

    ENUM_SERVICE_STATUS_PROCESS* services =
        (ENUM_SERVICE_STATUS_PROCESS*)buffer;

    std::map<std::string, std::set<DWORD>> dllMap;

    std::wcout << L"=== SERVICII ACTIVE ===\n\n";

    for (DWORD i = 0; i < servicesCount; i++) {

        if (services[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING) {

            DWORD pid = services[i].ServiceStatusProcess.dwProcessId;

            
            std::wcout << L"Serviciu: " << services[i].lpServiceName << std::endl;
            std::wcout << L"Display: " << services[i].lpDisplayName << std::endl;
            std::wcout << L"PID: " << pid << std::endl;
            std::wcout << L"-----------------------------\n";

           
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (!hProcess) continue;

            HMODULE hMods[1024];
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
                for (unsigned int j = 0; j < (cbNeeded / sizeof(HMODULE)); j++) {

                    char szModName[MAX_PATH];

                    if (GetModuleFileNameExA(hProcess, hMods[j], szModName, sizeof(szModName))) {
                        dllMap[szModName].insert(pid);
                    }
                }
            }

            CloseHandle(hProcess);
        }
    }

    std::cout << "\n=== DLL-uri folosite de mai multe servicii ===\n\n";

    for (auto& entry : dllMap) {
        if (entry.second.size() > 1) {

            std::cout << "DLL: " << entry.first << "\n";
            std::cout << "Procese (PID): ";

            for (auto pid : entry.second)
                std::cout << pid << " ";

            std::cout << "\n-----------------------------\n";
        }
    }

    delete[] buffer;
    CloseServiceHandle(hSCManager);

    system("pause");
    return 0;
}