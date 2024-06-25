#include "Okno_menu.h"

Okno_menu::Okno_menu() : wxFrame(nullptr, wxID_ANY, "Menu") {
	
    menu = Menu_wszystko();

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    int rows = (menu.produkty.size()+1) / 6;

    if ((menu.produkty.size()+1) % 6 != 0)
        rows++;

    wxGridSizer* panel_sizer = new wxGridSizer(rows, 6, 5, 5);

    button = new wxButton(panel, 0, "Dalej", wxPoint(10, 10));

    panel_sizer->Add(button, 0, wxEXPAND);

    wxString i_DB;
    wxString i_MB;
    wxTextFile file("bulki.txt");

    if (file.Open()) {
        i_DB = file.GetFirstLine();
        i_MB = file.GetNextLine();
        file.Close();
    }


    for (int i = 0; i < menu.produkty.size(); i++)
    {
        wxPanel* produkt = new wxPanel(panel, wxID_ANY);

        wxBoxSizer* produkt_Sizer = new wxBoxSizer(wxVERTICAL);

        wxCheckBox* check = new wxCheckBox(produkt, menu.produkty[i].Get_ID(), menu.produkty[i].Get_nazwa());

        if (menu.produkty[i].Get_check())
        {
            check->SetValue(true);
        }

        produkt_Sizer->Add(check, 0, wxEXPAND);

        wxTextCtrl* t = new wxTextCtrl(produkt, menu.produkty[i].Get_ID());

        if (menu.produkty[i].Get_ID() == menu.id_DB)
        {
            t->SetValue(i_DB);
        }
        else if (menu.produkty[i].Get_ID() == menu.id_MB)
        {
            t->SetValue(i_MB);
        }

        produkt_Sizer->Add(t, 0, wxEXPAND);

        produkt->SetSizer(produkt_Sizer);

        panel_sizer->Add(produkt, 0, wxEXPAND);
        checkboxs.push_back(check);
        text.push_back(t);
    }

    panel->SetSizer(panel_sizer);

}

void Okno_menu::OnButtonClicked(wxCommandEvent& event)
{
    Close();
    
    string id_alert = "";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "B³¹d inicjalizacji biblioteki Winsock." << std::endl;
    }

    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) != 0) {
        std::cerr << "B³¹d pobierania nazwy hosta." << std::endl;
        WSACleanup();
    }

    struct addrinfo* result = nullptr;
    struct addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(hostName, nullptr, &hints, &result) != 0) {
        std::cerr << "B³¹d pobierania informacji o hoœcie." << std::endl;
        WSACleanup();
    }

    freeaddrinfo(result);
    

    int screenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

    vector<Produkt> dzis_p;

    for (int i = 0; i < checkboxs.size(); i++)
    {
        if (checkboxs[i]->GetValue())
        {
            if(empty(text[i]->GetValue()))
                menu.produkty[i].Set_ilosc_do_sprzedazy(-100);
            else
                menu.produkty[i].Set_ilosc_do_sprzedazy(atoi(text[i]->GetValue()));
            dzis_p.push_back(menu.produkty[i]);
        }
    }

    Okno_host* host = new Okno_host(screenWidth, screenHeight, dzis_p);

    //host->Maximize();
    host->Show();

}

wxBEGIN_EVENT_TABLE(Okno_menu, wxFrame)
EVT_BUTTON(0, Okno_menu::OnButtonClicked)
wxEND_EVENT_TABLE()