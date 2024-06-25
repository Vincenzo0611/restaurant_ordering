#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <vector>
#include "Produkt.h"
#include "Paragon.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <rapidjson/document.h>

#pragma comment(lib, "ws2_32.lib") // Dodatkowa biblioteka dla Windows


using namespace std;
using namespace rapidjson;

class Okno_klient : public wxFrame
{
public:
    Okno_klient(string ipAddress);
    //void startListeningInBackground();
    //void backgroundThread();
    void dodaj_produkt(Paragon p);
	void onNumberUpdate(wxCommandEvent& evt);
	void Usun_message(wxCommandEvent& evt);
	void wydaj_message(wxCommandEvent& evt);
    void wydaj_produkt(int ID);
    void usun_paragon(int panelID);
    void reset_paragonow(wxCommandEvent& evt);
    ~Okno_klient();
    vector<Produkt> dzis_p;
    vector<Paragon> paragony;
    wxScrolled<wxPanel>* panel_paragony;
    wxGridSizer* paragony_Sizer;
    SOCKET sock;
    vector<wxPanel*> kupony;
    
    bool usuniete = false;

    wxDECLARE_EVENT_TABLE();
};


//BEGIN_EVENT_TABLE(Okno_klient, wxFrame)
//EVT_COMMAND (NUMBER_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::onNumberUpdate)
//END_EVENT_TABLE()