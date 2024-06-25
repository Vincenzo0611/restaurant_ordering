#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <vector>
#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include <wx/textfile.h>

#pragma comment(lib, "ws2_32.lib") // Dodatkowa biblioteka dla Windows


using namespace std;

class Ilosc_bulek : public wxFrame
{
public:
    Ilosc_bulek();
    ~Ilosc_bulek();
private:
    wxString filename = "bulki.txt";
    wxTextCtrl* txtDuzeBulki;
    wxTextCtrl* txtMaleBulki;
};


//BEGIN_EVENT_TABLE(Okno_klient, wxFrame)
//EVT_COMMAND (NUMBER_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::onNumberUpdate)
//END_EVENT_TABLE()