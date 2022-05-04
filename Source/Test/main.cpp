#include <Windows.h>
#include <cstdio>

int GetValue()
{
    return 10;
}

int main()
{
    while (true)
    {
        if (GetAsyncKeyState(VK_RETURN) & 1)
            break;

        printf("Value : %d\n", GetValue());

        Sleep(500);
    }

    system("PAUSE");
    return 0;
}