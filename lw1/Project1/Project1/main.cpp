#include <windows.h>
#include <string>
#include <iostream>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    printf("Thread %d is working\n", *static_cast<int*>(lpParam));

    ExitThread(0);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        return 1;
    }

    int threadQuantity = 5;

    try {
        threadQuantity = std::stoi(argv[1]);
    }
    catch (...) {
        std::cout << "Invalid argument" << std::endl;
        return 1;
    }


    HANDLE* handles = new HANDLE[threadQuantity];
    int* indexes = new int[threadQuantity];

    for (int i = 0; i < threadQuantity; i++)
    {
        indexes[i] = i;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &indexes[i], CREATE_SUSPENDED, NULL);
    }

    for (int i = 0; i < threadQuantity; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(threadQuantity, handles, true, INFINITE);
    
    return 0;
}
