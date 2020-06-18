#include <cstdlib>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <conio.h>
#include <stdio.h>
#include <sys/stat.h>

#define WIN32_LEAN_AND_MEAN
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)
BOOL Inject(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);

using namespace std;
bool fileExists(const std::string& filename) {
    struct stat buf;
    if(stat(filename.c_str(), &buf) != -1) {
        return true;
    }

    return false;
}

int main() {
    string str, clientVersion, processName, dllName;
    processName = "Tibia.exe";
    dllName = "860.dll";
    int charactersLimit = 100;

    SetConsoleTitle("Tibia UnBinder");
    cout << ">> Tibia UnBinder v0.3\n>> Made by Rutserzim (DLLs made by Chojrak and Beziak)\n\n>> Initializing unbinder... done!";
    while(true) {
        cout << "\n> ";
        getline(cin, str);
        if(str == "help") {
            cout << ">> help - Diplays this content.\n>> process param - Sets process of client (Default is Tibia).\n>> client param - Sets client version (Avaible: 800, 831, 840, 842, 850, 852, 853, 854, 860). (!) THIS SHOULD BE NUMBER, WITHOUT ',' OR '.' (!)\n>> inject - Starts the cracking process.\n>> clear - Clear the screen.\n>> exit - Shut downs application.";
        } else if(str.find("client") != string::npos) {
            clientVersion = str.substr(7, 9);
            dllName = clientVersion + ".dll";
            cout << ">> The client has been set to " << clientVersion << ".";
        } else if(str.find("process") != string::npos) {
            processName = str.substr(8, charactersLimit);
            cout << ">> Process has been set to " << processName << ".exe";
        } else if(str == "clear") {
            system("CLS");
            cout << ">> Tibia UnBinder v0.3\n>> Made by Rutserzim (DLLs made by Chojrak and Beziak)\n\n>> Initializing unbinder... done!";
        } else if(str == "inject") {
            cout << ">> Injecting .DLL file...\n>> .DLL has been injected sucessfully, waiting for .DLL response...";

            if(!fileExists(dllName.c_str()))
                cout << "\n>> DLL not found " << dllName.c_str() << "!";
            else {
                char buf[MAX_PATH] = {0};
                GetFullPathName(dllName.c_str(), MAX_PATH, buf, NULL);
                cout << endl;
                
                if(!Inject(GetTargetThreadIDFromProcName(processName.c_str()), buf))
                    cout << ">> DLL not injected, process '" << processName << "' not found!";
                else
                    cout << ">> DLL injected!";
            }
        } else if(str == "exit") {
            cout << ">> De-initializing unbinder...\n>> All modules has been closed.\n";
            return system("PAUSE");
        } else {
            cout << ">> Unknown command, type 'help' to view full list of commands.";
        }
    }
}

BOOL Inject(DWORD pID, const char * DLL_NAME) {
    HANDLE Proc;
    HMODULE hLib;
    char buf[50] = {0};
    LPVOID RemoteString, LoadLibAddy;

    if(!pID) {
        return false;
    }

    Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if(!Proc) {
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);
    CloseHandle(Proc);
}

DWORD GetTargetThreadIDFromProcName(const char * ProcName) {
    PROCESSENTRY32 pe;
    HANDLE thSnapShot;
    BOOL retval, ProcFound = false;

    thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(thSnapShot == INVALID_HANDLE_VALUE) {
        cout << ">> Error: Unable to create toolhelp snapshot!";
        return false;
    }

    pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapShot, &pe);
    while(retval) {
        if(StrStrI(pe.szExeFile, ProcName)) {
            return pe.th32ProcessID;
        }

        retval = Process32Next(thSnapShot, &pe);
    }
}
