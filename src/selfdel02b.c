#include <windows.h>

void DeleteMyself()
{
    char    buf[MAX_PATH];
    HMODULE module;
    
    module = GetModuleHandle(0);
    GetModuleFileName(module, buf, MAX_PATH);

    if(0x80000000 & GetVersion())
    {
        __asm 
        {
          lea     eax, buf
          push    0
          push    0
          push    eax
          push    ExitProcess
          push    module
          push    DeleteFile
          push    FreeLibrary
          ret
        }
    }
    // do for WinNT kernel
    else    
    {
        CloseHandle((HANDLE)4);

        __asm 
        {
          lea     eax, buf
          push    0
          push    0
          push    eax
          push    ExitProcess
          push    module
          push    DeleteFile
          push    UnmapViewOfFile
          ret
        }
    }
}

int main(int argc, char *argv[])
{
   DeleteMyself();
   return 0;
} 
