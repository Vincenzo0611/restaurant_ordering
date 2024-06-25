#pragma once
#include <wx/wx.h>
#include <vector>
#include <set>
#include <ctime>
#include <fstream>
#include <sys/stat.h> 
#include "Produkt.h"
#include "Paragon.h"
#include "Polaczenie.h"
#include "Menu_wszystko.h"
#include "Blok_paragonu_aktualnego.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <wx/textfile.h>

#ifdef _WIN32
#include <direct.h> // Dla funkcji _mkdir
#define mkdir _mkdir
#endif

using namespace std;
using namespace rapidjson;


class Okno_host : public wxFrame
{
public:
    Okno_host(int szerokosc, int wysokosc, vector<Produkt> dzis_p);
    ~Okno_host();
    void Historia(wxCommandEvent& event);
    void Do_zaplacenia(wxCommandEvent& event);
    void OnCancelClicked(wxCommandEvent& event);
    void OnDodajClicked(wxCommandEvent& event);
    void Produkt_button(wxCommandEvent& event);
    void Plus(wxCommandEvent& event);
    void Minus(wxCommandEvent& event);
    void Usun_z_aktualnego(wxCommandEvent& event);
    void usun_paragon(wxCommandEvent& event);
    void zaplac_paragon(wxCommandEvent& event);
    void zaplac_paragon_okno(wxCommandEvent& event);
    void cofnij_paragon(wxCommandEvent& event);
    void wydaj_produkt(wxCommandEvent& event);
    void dodaj_produkt(Produkt p);
    void reset_klienta(wxCommandEvent& evt);
    void dodaj_klienta(wxCommandEvent& evt);
    int Policz_czas_oczekiwania();
    //bool Compare(Produkt& a, Produkt& b);
private:
    Menu_wszystko menu;
    vector<Produkt> dzis_p;
    vector<Paragon> paragony;
    vector<Paragon> aktualne;

    int id_paragonu;

    int numer_paragonu;
    int szerokosc;
    int wysoksoc;

    wxPanel* panel_dod;
    wxPanel* akceptuj;
    wxPanel* przyciski_dodawania;
    wxScrolled<wxPanel>* aktualny_paragon;
    wxScrolled<wxPanel>* panel_paragony;
    wxScrolled<wxPanel>* panel_paragony_ekran;

    vector<wxPanel*> kupony;
    vector<wxPanel*> kupony_ekran;
    
    
    vector<wxPanel*> paragony_do_zaplacenia;

    vector<wxButton*> przyciski_dodawanie_vector;

    wxBoxSizer* aktualny_Sizer;
    wxBoxSizer* paragony_panel_Sizer;
    wxGridSizer* paragony_Sizer;
    wxGridSizer* paragony_ekran_Sizer;

    vector<Polaczenie*> p;
    wxFrame* dodawanie;
    wxFrame* ekran;

    wxTextCtrl* numer;
    Paragon* aktualny;
    vector<Blok_paragonu> bloki;

    set<int>numery;

    wxStaticText* oczekiwanie;

};

