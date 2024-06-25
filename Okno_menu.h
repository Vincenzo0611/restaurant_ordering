#pragma once
#include <wx/wx.h>
#include <vector>
#include "Menu_wszystko.h"
#include "Glowne_okno_host.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wx/textfile.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Okno_menu : public wxFrame
{
public:
    Okno_menu();

    void OnButtonClicked(wxCommandEvent& event);

private:
    vector<wxCheckBox*> checkboxs;
    vector<wxTextCtrl*> text;
    wxButton* button;
    Menu_wszystko menu;
    wxDECLARE_EVENT_TABLE();
};