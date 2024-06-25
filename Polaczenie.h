#pragma once
#include <wx/wx.h>
#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Dodatkowa biblioteka dla Windows

class Polaczenie
{
public:
    Polaczenie();
    ~Polaczenie();
    SOCKET clientSocket;
};
