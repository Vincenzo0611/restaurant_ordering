#include "Glowne_okno_host.h"
#define START_NR 1
#define KONIEC_NR 20
#define ID_PARAGONU 0

class MyDialog : public wxDialog
{
public:
    MyDialog(wxWindow* parent, const wxString& title)
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* message = new wxStaticText(this, wxID_ANY, "Czy zaplacone?", wxDefaultPosition, wxSize(200, 50));
        sizer->Add(message, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* yesButton = new wxButton(this, wxID_YES, "Tak", wxDefaultPosition, wxSize(300, 300));
        wxButton* noButton = new wxButton(this, wxID_NO, "Nie", wxDefaultPosition, wxSize(300, 300));

        buttonSizer->Add(yesButton, 0, wxALL, 5);
        buttonSizer->Add(noButton, 0, wxALL, 5);
        sizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);

        SetSizerAndFit(sizer);

        yesButton->Bind(wxEVT_BUTTON, &MyDialog::OnYesButtonClicked, this);
        noButton->Bind(wxEVT_BUTTON, &MyDialog::OnNoButtonClicked, this);

        wxSize screenSize = wxGetDisplaySize();
        SetSize(screenSize.GetWidth() / 2 - 100, screenSize.GetHeight() / 2);
    }

private:
    void OnYesButtonClicked(wxCommandEvent& event)
    {
        EndModal(wxID_YES);
    }

    void OnNoButtonClicked(wxCommandEvent& event)
    {
        EndModal(wxID_NO);
    }
};

bool comparePhrases(const std::string& str1, const std::string& phrase) {
    // SprawdŸ, czy fraza wystêpuje w obu ci¹gach
    return (str1.find(phrase) != std::string::npos);
}

Okno_host::Okno_host(int szerokosc, int wysokosc, vector<Produkt> dzis_p) : wxFrame(nullptr, wxID_ANY, "Host", wxDefaultPosition, wxSize(szerokosc/2, wysokosc))
{
    this->menu = Menu_wszystko();
    
    for (int i = 0; i < dzis_p.size(); i++)
    {
        if (i == dzis_p.size() - 1)
            break;
        if (comparePhrases(dzis_p[i].Get_nazwa(), "Wolowy") && comparePhrases(dzis_p[i+1].Get_nazwa(), "Wolowy"))
        {
            dzis_p[i + 1].Set_ilosc_do_sprzedazy(dzis_p[i].Get_ilosc_do_sprzedazy());
        }
        else if (comparePhrases(dzis_p[i].Get_nazwa(), "Maly wolowy") && comparePhrases(dzis_p[i + 1].Get_nazwa(), "Maly wolowy"))
        {
            dzis_p[i + 1].Set_ilosc_do_sprzedazy(dzis_p[i].Get_ilosc_do_sprzedazy());
        }
        else if (comparePhrases(dzis_p[i].Get_nazwa(), "Jagniecy") && comparePhrases(dzis_p[i + 1].Get_nazwa(), "Jagniecy"))
        {
            dzis_p[i + 1].Set_ilosc_do_sprzedazy(dzis_p[i].Get_ilosc_do_sprzedazy());
        }
        else if (comparePhrases(dzis_p[i].Get_nazwa(), "Kozlecy") && comparePhrases(dzis_p[i + 1].Get_nazwa(), "Kozlecy"))
        {
            dzis_p[i + 1].Set_ilosc_do_sprzedazy(dzis_p[i].Get_ilosc_do_sprzedazy());
        }
    }

    this->szerokosc = szerokosc;
    this->wysoksoc = wysokosc;
    this->numer_paragonu = START_NR;
    this->id_paragonu = ID_PARAGONU;
    this->aktualny = nullptr;

    this->dzis_p = dzis_p;


    // Tworzenie ekranow
    dodawanie = new wxFrame(this, wxID_ANY, "Dodaj", wxDefaultPosition, wxSize(szerokosc/2, wysokosc));
    ekran = new wxFrame(this, wxID_ANY, "Ekran", wxDefaultPosition, wxDefaultSize);


    panel_dod = new wxPanel(dodawanie, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    akceptuj = new wxPanel(panel_dod, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    przyciski_dodawania = new wxPanel(panel_dod, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    aktualny_paragon = new wxScrolled<wxPanel>(dodawanie, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    przyciski_dodawania->SetBackgroundColour(wxColour(30, 30, 30));
    aktualny_paragon->SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* panel_dod_Sizer = new wxBoxSizer(wxVERTICAL);
    
    panel_dod_Sizer->Add(akceptuj, 15, wxEXPAND);
    panel_dod_Sizer->Add(przyciski_dodawania, 85, wxEXPAND);

    panel_paragony = new wxScrolled<wxPanel>(this, wxID_ANY);
    panel_paragony_ekran = new wxScrolled<wxPanel>(ekran, wxID_ANY);


    aktualny_paragon->SetScrollRate(0, FromDIP(10));
    panel_paragony->SetScrollRate(0, FromDIP(10));
    panel_paragony_ekran->SetScrollRate(0, FromDIP(10));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* ekran_Sizer = new wxBoxSizer(wxVERTICAL);
    aktualny_Sizer = new wxBoxSizer(wxVERTICAL);
    paragony_panel_Sizer = new wxBoxSizer(wxVERTICAL);

    int rows = dzis_p.size() / 3;

    if (dzis_p.size() % 3 != 0)
        rows++;

    wxGridSizer* przyciski_Sizer = new wxGridSizer(rows, 3, 5, 5);
    wxGridSizer* akceptuj_Sizer = new wxGridSizer(1, 3, 5, 5);
    paragony_Sizer = new wxGridSizer(3, 3, 5, 5);
    paragony_ekran_Sizer = new wxGridSizer(6, 6, 5, 5);

    for (int i = 0; i < dzis_p.size(); i++)
    {
        string n = dzis_p[i].Get_nazwa() + "\n" + to_string(dzis_p[i].Get_ilosc_do_sprzedazy());
        if (dzis_p[i].Get_ilosc_do_sprzedazy() <= -100)
        {
            n = dzis_p[i].Get_nazwa();
        }
        wxButton* p = new wxButton(przyciski_dodawania, dzis_p[i].Get_ID(), n);
        p->Bind(wxEVT_BUTTON, &Okno_host::Produkt_button, this, p->GetId());
        przyciski_dodawanie_vector.push_back(p);
        przyciski_Sizer->Add(p, 0, wxEXPAND);
    }

    wxPanel* h = new wxPanel(akceptuj, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    wxBoxSizer* h_sizer = new wxBoxSizer(wxVERTICAL);

    numer = new wxTextCtrl(h, wxID_ANY, to_string(this->numer_paragonu));

    h_sizer->Add(numer, 0, wxEXPAND);

    wxButton* historia = new wxButton(h, wxID_ANY, "Historia");
    historia->Bind(wxEVT_BUTTON, &Okno_host::Historia, this);

    h_sizer->Add(historia, 0, wxEXPAND);
    wxButton* placenie = new wxButton(h, wxID_ANY, "Do placenia");
    placenie->Bind(wxEVT_BUTTON, &Okno_host::Do_zaplacenia, this);

    h_sizer->Add(placenie, 0, wxEXPAND);
    
    wxButton* reset = new wxButton(h, wxID_ANY, "Reset ekranu");
    reset->Bind(wxEVT_BUTTON, &Okno_host::reset_klienta, this);

    h_sizer->Add(reset, 0, wxEXPAND); 
    

    wxButton* k = new wxButton(h, wxID_ANY, "Dodaj klienta");
    k->Bind(wxEVT_BUTTON, &Okno_host::dodaj_klienta, this);

    h_sizer->Add(k, 0, wxEXPAND);

    oczekiwanie = new wxStaticText(h, wxID_ANY, "   0 min");
    h_sizer->Add(oczekiwanie, 0, wxEXPAND);

    h->SetSizer(h_sizer);

    akceptuj_Sizer->Add(h, 0, wxEXPAND);

    wxButton* dodaj = new wxButton(akceptuj, wxID_ANY, "DODAJ");
    dodaj->Bind(wxEVT_BUTTON, &Okno_host::OnDodajClicked, this);
    dodaj->SetBackgroundColour(wxColor(0, 255, 0));
    
    akceptuj_Sizer->Add(dodaj, 0, wxEXPAND);

    wxButton* cancel = new wxButton(akceptuj, wxID_ANY, "ANULUJ");
    cancel->Bind(wxEVT_BUTTON, &Okno_host::OnCancelClicked, this);
    cancel->SetBackgroundColour(wxColor(255, 0, 0));

    akceptuj_Sizer->Add(cancel, 0, wxEXPAND);

    akceptuj->SetSizer(akceptuj_Sizer);
    panel_paragony->SetSizer(paragony_Sizer);
    panel_paragony_ekran->SetSizer(paragony_ekran_Sizer);
    aktualny_paragon->SetSizer(aktualny_Sizer);
    przyciski_dodawania->SetSizer(przyciski_Sizer);
    panel_dod->SetSizer(panel_dod_Sizer);

    mainSizer->Add(panel_dod, 1, wxEXPAND);  // Panel 1 zajmuje 50% dostêpnej przestrzeni
    mainSizer->Add(aktualny_paragon, 1, wxEXPAND);

    ekran_Sizer->Add(panel_paragony_ekran, 1, wxEXPAND);

    dodawanie->SetSizer(mainSizer);
    ekran->SetSizer(ekran_Sizer);

	Polaczenie* tmp_p = new Polaczenie();
    p.push_back(tmp_p);

    dodawanie->Show();
    ekran->Show();
}

void Okno_host::OnCancelClicked(wxCommandEvent& event)
{
    bloki.clear();
    if(aktualny != nullptr)
        aktualny->produkty.clear();
    aktualny_paragon->DestroyChildren();
    aktualny_Sizer->Clear();
    aktualny_Sizer->Layout();
    aktualny_paragon->Refresh();
}

void Okno_host::Produkt_button(wxCommandEvent& event)
{
    int buttonID = event.GetId();
    
    if (aktualny == nullptr)
        aktualny = new Paragon(this->numer_paragonu);
    
    aktualny->produkty.push_back(menu.produkty[buttonID]);
    Blok_paragonu blok;
    blok.panel= new wxPanel(aktualny_paragon, bloki.size(), wxDefaultPosition, wxSize(-1, 200), wxBORDER_SIMPLE);
    blok.ilosc = 1;
    blok.nazwa = aktualny->produkty[aktualny->produkty.size() - 1].Get_nazwa();

    blok.name = new wxStaticText(blok.panel, bloki.size(), blok.nazwa, wxPoint(10 ,10));

    blok.notatka = new wxTextCtrl(blok.panel, bloki.size(), "", wxPoint(10, 40), wxSize(300, 30));
    blok.ilosc_teskt = new wxTextCtrl(blok.panel, bloki.size(), "1", wxPoint(250, 10), wxSize(40, 30));
    blok.plus = new wxButton(blok.panel, bloki.size(), "+", wxPoint(310, 10), wxSize(30, 30));
    blok.minus = new wxButton(blok.panel, bloki.size(), "-", wxPoint(210, 10), wxSize(30, 30));
    blok.usun = new wxButton(blok.panel, bloki.size(), "USUN", wxPoint(350, 40));
    blok.plus->Bind(wxEVT_BUTTON, &Okno_host::Plus, this, blok.plus->GetId());
    blok.minus->Bind(wxEVT_BUTTON, &Okno_host::Minus, this, blok.minus->GetId());
    blok.usun->Bind(wxEVT_BUTTON, &Okno_host::Usun_z_aktualnego, this, blok.usun->GetId());
    
    bloki.push_back(blok);

    aktualny_Sizer->Add(blok.panel, 1, wxEXPAND | wxALL, 5);
    aktualny_Sizer->FitInside(aktualny_paragon);
}

void Okno_host::OnDodajClicked(wxCommandEvent& event)
{

    std::time_t currentTime = std::time(nullptr);

    // Konwertowanie czasu na strukturê tm
    std::tm* localTime = std::localtime(&currentTime);

    // Formatowanie czasu jako string
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M", localTime);

    // Konwersja do std::string
    std::string currentTimeString(buffer);

    if (aktualny != nullptr)
    {
        numer_paragonu = wxAtoi(numer->GetValue());

        numery.insert(numer_paragonu);

        aktualny->numer = numer_paragonu;
        aktualny->czas = currentTimeString;

        wxInitAllImageHandlers();
        MyDialog dialog(nullptr, "Pytanie");
        int result = dialog.ShowModal();
        if (result == wxID_YES)
        {
            aktualny->zaplacone = true;
        }
        else if (result == wxID_NO)
        {
            aktualny->zaplacone = false;
        }
        else
        {
            return;
        }

        aktualny->id = id_paragonu;
        id_paragonu += 1;


        for (int i = 0; i < aktualny->produkty.size(); i++)
        {
            if (aktualny->produkty[i].Get_usuniety() == false)
            {
                aktualny->produkty[i].Set_notatka(string(bloki[i].notatka->GetValue().mb_str()));
                if (comparePhrases(aktualny->produkty[i].Get_nazwa(), "Wolowy"))
                {
                    for (int m = 0; m < dzis_p.size(); m++)
                    {
                        if (comparePhrases(dzis_p[m].Get_nazwa(), "Wolowy") || dzis_p[m].Get_ID() == menu.id_DB)
                        {
                            dzis_p[m].Set_ilosc_do_sprzedazy(dzis_p[m].Get_ilosc_do_sprzedazy() - aktualny->produkty[i].Get_ilosc());
                            string n = dzis_p[m].Get_nazwa()+ "\n" + to_string(dzis_p[m].Get_ilosc_do_sprzedazy());
                            if (dzis_p[m].Get_ilosc_do_sprzedazy() <= -100)
                            {
                                n = dzis_p[m].Get_nazwa();
                            }
                            przyciski_dodawanie_vector[m]->SetLabel(n);
                        }
                    }
                }
                else if (comparePhrases(aktualny->produkty[i].Get_nazwa(), "Maly wolowy"))
                {
                    for (int m = 0; m < dzis_p.size(); m++)
                    {
                        if (comparePhrases(dzis_p[m].Get_nazwa(), "Maly wolowy") || dzis_p[m].Get_ID() == menu.id_MB)
                        {
                            dzis_p[m].Set_ilosc_do_sprzedazy(dzis_p[m].Get_ilosc_do_sprzedazy() - aktualny->produkty[i].Get_ilosc());
                            string n = dzis_p[m].Get_nazwa() + "\n" + to_string(dzis_p[m].Get_ilosc_do_sprzedazy());
                            if (dzis_p[m].Get_ilosc_do_sprzedazy() <= -100)
                            {
                                n = dzis_p[m].Get_nazwa();
                            }
                            przyciski_dodawanie_vector[m]->SetLabel(n);
                        }
                    }
                }
                else if (comparePhrases(aktualny->produkty[i].Get_nazwa(), "Jagniecy"))
                {
                    for (int m = 0; m < dzis_p.size(); m++)
                    {
                        if (comparePhrases(dzis_p[m].Get_nazwa(), "Jagniecy") || dzis_p[m].Get_ID() == menu.id_DB)
                        {
                            dzis_p[m].Set_ilosc_do_sprzedazy(dzis_p[m].Get_ilosc_do_sprzedazy() - aktualny->produkty[i].Get_ilosc());
                            string n = dzis_p[m].Get_nazwa() + "\n" + to_string(dzis_p[m].Get_ilosc_do_sprzedazy());
                            if (dzis_p[m].Get_ilosc_do_sprzedazy() <= -100)
                            {
                                n = dzis_p[m].Get_nazwa();
                            }
                            przyciski_dodawanie_vector[m]->SetLabel(n);
                        }
                    }
                }
                else if (comparePhrases(aktualny->produkty[i].Get_nazwa(), "Kozlecy"))
                {
                    for (int m = 0; m < dzis_p.size(); m++)
                    {
                        if (comparePhrases(dzis_p[m].Get_nazwa(), "Kozlecy") || dzis_p[m].Get_ID() == menu.id_DB)
                        {
                            dzis_p[m].Set_ilosc_do_sprzedazy(dzis_p[m].Get_ilosc_do_sprzedazy() - aktualny->produkty[i].Get_ilosc());
                            string n = dzis_p[m].Get_nazwa() + "\n" + to_string(dzis_p[m].Get_ilosc_do_sprzedazy());
                            if (dzis_p[m].Get_ilosc_do_sprzedazy() <= -100)
                            {
                                n = dzis_p[m].Get_nazwa();
                            }
                            przyciski_dodawanie_vector[m]->SetLabel(n);
                        }
                    }
                }
                else
                {
                    for (int m = 0; m < dzis_p.size(); m++)
                    {
                        if (aktualny->produkty[i].Get_ID() == dzis_p[m].Get_ID())
                        {
                            dzis_p[m].Set_ilosc_do_sprzedazy(dzis_p[m].Get_ilosc_do_sprzedazy() - aktualny->produkty[i].Get_ilosc());
                            string n = dzis_p[m].Get_nazwa() + "\n" + to_string(dzis_p[m].Get_ilosc_do_sprzedazy());
                            if (dzis_p[m].Get_ilosc_do_sprzedazy() <= -100)
                            {
                                n = dzis_p[m].Get_nazwa();
                            }
                            przyciski_dodawanie_vector[m]->SetLabel(n);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < aktualny->produkty.size(); i++)
        {

            if (aktualny->produkty[i].Get_usuniety())
            {
                aktualny->produkty.erase(aktualny->produkty.begin() + i);
                i--;
            }
        }

        //sort(aktualny->produkty.begin(), aktualny->produkty.end(), Compare);
        sort(aktualny->produkty.begin(), aktualny->produkty.end(), [](Produkt& a, Produkt& b) {
            return a.Get_ID() < b.Get_ID();
            });

        paragony.push_back(*aktualny);
        aktualne.push_back(*aktualny);


        //Wyslij tu
        // Tworzenie struktury JSON
        Document data;
        data.SetObject();

        Document::AllocatorType& allocator = data.GetAllocator();


        data.AddMember("typ", 1, allocator);
        data.AddMember("numer", aktualny->numer, allocator);
        data.AddMember("ilosc_produktow", aktualny->produkty.size(), allocator);

        Value czasV;
        czasV.SetString(currentTimeString.c_str(), static_cast<SizeType>(currentTimeString.length()), allocator);

        data.AddMember("czas", czasV, allocator);

        Value produkty(kArrayType);


        for (int i = 0; i < aktualny->produkty.size(); i++)
        {
            Value produkt1(kObjectType);

            Value nazwa;
            nazwa.SetString(aktualny->produkty[i].Get_nazwa().c_str(), static_cast<SizeType>(aktualny->produkty[i].Get_nazwa().length()), allocator);

            produkt1.AddMember("nazwa", nazwa, allocator);
            produkt1.AddMember("ilosc", aktualny->produkty[i].Get_ilosc(), allocator);

            Value notatka;
            notatka.SetString(aktualny->produkty[i].Get_notatka().c_str(), static_cast<SizeType>(aktualny->produkty[i].Get_notatka().length()), allocator);

            produkt1.AddMember("notatka", notatka, allocator);
            produkty.PushBack(produkt1, allocator);
        }

        data.AddMember("produkty", produkty, allocator);

        // Konwersja do ci¹gu znaków
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        data.Accept(writer);

        string jsonString = buffer.GetString();

        // Przygotowanie bufora dla przesy³anych danych
        char bufferToSend[1024];
        std::memset(bufferToSend, 0, sizeof(bufferToSend));
        strncpy(bufferToSend, jsonString.c_str(), sizeof(bufferToSend) - 1);

        // Przesy³anie danych przez gniazdo
        // Przyk³adowa implementacja send()
        for (int p_i = 0; p_i < p.size(); p_i++)
        {
            send(p[p_i]->clientSocket, bufferToSend, strlen(bufferToSend), 0);
        }

        delete aktualny;

        aktualny = nullptr;

        bloki.clear();
        aktualny_paragon->DestroyChildren();
        aktualny_Sizer->Clear();
        aktualny_Sizer->Layout();
        aktualny_paragon->Refresh();
        /*
        while (numery.count(numer_paragonu))
        {
            if (numery.count(numer_paragonu + 10))
            {
                numer_paragonu++;
            }
            else
            {
                numer_paragonu += 10;
            }
        }
        */
        numer_paragonu++;
        if (numer_paragonu > KONIEC_NR)
        {
            numer_paragonu = START_NR;
        }
        while (numery.count(numer_paragonu))
        {
            numer_paragonu++;
        }

        numer->SetValue(to_string(numer_paragonu));
        wxPanel* para = new wxPanel(panel_paragony, paragony[paragony.size() - 1].numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

        wxBoxSizer* paraSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* numer = new wxStaticText(para, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].numer), wxPoint(10, 0), wxDefaultSize);
        wxFont font = numer->GetFont();
        font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
        numer->SetFont(font);

        wxStaticText* czas = new wxStaticText(para, paragony[paragony.size() - 1].numer, currentTimeString, wxPoint(150, 0), wxDefaultSize);

        paraSizer->Add(numer, 0, wxEXPAND);
        paraSizer->Add(czas, 0, wxEXPAND);


        for (int i = 0; i < paragony[paragony.size() - 1].produkty.size(); i++)
        {
            wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

            string n = paragony[paragony.size() - 1].produkty[i].Get_nazwa() + " x" + to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc());

            wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
            wxFont fontn = name->GetFont();
            fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
            name->SetFont(fontn);
            wxStaticText* notatka = new wxStaticText(prod, i, paragony[paragony.size() - 1].produkty[i].Get_notatka(), wxPoint(10, 50));
            //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
            //wxFont font = ilosc_teskt->GetFont();
            //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
            //ilosc_teskt->SetFont(font);

            int panelID = paragony[paragony.size() - 1].numer;
            int produktID = i;

            int id = panelID * 1000 + produktID;

            wxButton* wydaj = new wxButton(prod, id, "WYDAJ", wxPoint(240, 13));

            wydaj->Bind(wxEVT_BUTTON, &Okno_host::wydaj_produkt, this, wydaj->GetId());
            paraSizer->Add(prod, 0, wxEXPAND);
        }

        wxButton* usun = new wxButton(para, paragony[paragony.size() - 1].numer, "WYDANE");
        usun->Bind(wxEVT_BUTTON, &Okno_host::usun_paragon, this, usun->GetId());
        paraSizer->Add(usun, 0, wxEXPAND);

        if (!paragony[paragony.size() - 1].zaplacone)
        {
            wxButton* zaplac = new wxButton(para, paragony[paragony.size() - 1].numer, "ZAPLAC");
            zaplac->Bind(wxEVT_BUTTON, &Okno_host::zaplac_paragon, this, zaplac->GetId());
            paraSizer->Add(zaplac, 0, wxEXPAND);
        }

        para->SetSizer(paraSizer);
        paragony_Sizer->Add(para, 0, wxEXPAND);
        paragony_Sizer->FitInside(panel_paragony);
        kupony.push_back(para);

        //Drugi ekran
        para = new wxPanel(panel_paragony_ekran, paragony[paragony.size() - 1].numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

        paraSizer = new wxBoxSizer(wxVERTICAL);

        numer = new wxStaticText(para, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].numer), wxPoint(10, 0), wxDefaultSize);
        font = numer->GetFont();
        font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
        numer->SetFont(font);

        czas = new wxStaticText(para, paragony[paragony.size() - 1].numer, currentTimeString, wxPoint(150, 0), wxDefaultSize);

        paraSizer->Add(numer, 0, wxEXPAND);
        paraSizer->Add(czas, 0, wxEXPAND);


        for (int i = 0; i < paragony[paragony.size() - 1].produkty.size(); i++)
        {
            wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

            string n = paragony[paragony.size() - 1].produkty[i].Get_nazwa() + " x" + to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc());

            wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
            wxFont fontn = name->GetFont();
            fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
            name->SetFont(fontn);
            wxStaticText* notatka = new wxStaticText(prod, i, paragony[paragony.size() - 1].produkty[i].Get_notatka(), wxPoint(10, 50));
            //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
            //wxFont font = ilosc_teskt->GetFont();
            //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
            //ilosc_teskt->SetFont(font);

            int panelID = paragony[paragony.size() - 1].numer;
            int produktID = i;

            int id = panelID * 1000 + produktID;

            paraSizer->Add(prod, 0, wxEXPAND);
        }

        para->SetSizer(paraSizer);
        paragony_ekran_Sizer->Add(para, 0, wxEXPAND);
        paragony_ekran_Sizer->FitInside(panel_paragony_ekran);
        kupony_ekran.push_back(para);

        int oczeki = Policz_czas_oczekiwania();
        string l = "   " + to_string(oczeki) + " min";


        oczekiwanie->SetLabel(l);
    }
}


void Okno_host::dodaj_produkt(Produkt p)
{
    this->dzis_p.push_back(p);
}

Okno_host::~Okno_host()
{
    wxTextFile file("bulki.txt");

    wxString DB;
    wxString MB;

    DB = wxString::Format(wxT("%d"), dzis_p[dzis_p.size() - 2].Get_ilosc_do_sprzedazy());
    MB = wxString::Format(wxT("%d"), dzis_p[dzis_p.size() -1].Get_ilosc_do_sprzedazy());

    if (file.Open()) {
        file.Clear();
        file.AddLine(DB);
        file.AddLine(MB);
        file.Write();
        file.Close();
    }
    const char* nazwaFolderu = "historia";
    if (mkdir(nazwaFolderu) != 0) {
        // Jeœli nie uda³o siê utworzyæ folderu, sprawdŸ czy istnieje b³¹d z powodu ju¿ istniej¹cego folderu
        if (errno != EEXIST) {
            cerr << "B³¹d tworzenia folderu." << endl;
            return;
        }
    }

    // Pobranie bie¿¹cej daty i godziny
    time_t czasTeraz = time(nullptr);
    struct tm czasInfo = *localtime(&czasTeraz);

    // Tworzenie nazwy pliku na podstawie daty i godziny
    char nazwaPliku[80];
    strftime(nazwaPliku, sizeof(nazwaPliku), "%d-%m-%Y-%H-%M-%S.txt", &czasInfo);

    // Tworzenie pe³nej œcie¿ki do pliku
    string pelnaSciezka = string(nazwaFolderu) + "/" + string(nazwaPliku);

    ofstream plik(pelnaSciezka);



    if (plik.is_open()) {
        for (const Paragon& paragon : paragony) {
            plik << "Numer paragonu: " << paragon.numer << endl;
            plik << "Czas zakupu: " << paragon.czas << endl;
            plik << "Produkty:" << endl;
            for (Produkt produkt : paragon.produkty) {
                plik << produkt.Get_nazwa() << ", ";
                plik << "Ilosc: " << produkt.Get_ilosc() << " \n";
                plik << "Notatka: " << produkt.Get_notatka() << " \n--  \n";
            }

            plik << "-------------------------------\n";
        }
        plik.close();
    }
    else {

    }
}

void Okno_host::Minus(wxCommandEvent& event)
{
    int id = event.GetId();

    if (bloki[id].ilosc > 1)
    {
        bloki[id].ilosc--;
        aktualny->produkty[id].Set_ilosc(bloki[id].ilosc);
        bloki[id].ilosc_teskt->SetValue(to_string(bloki[id].ilosc));
    }
}
void Okno_host::Plus(wxCommandEvent& event)
{
    int id = event.GetId();
    bloki[id].ilosc++;
    aktualny->produkty[id].Set_ilosc(bloki[id].ilosc);
    bloki[id].ilosc_teskt->SetValue(to_string(bloki[id].ilosc));
}

void Okno_host::Usun_z_aktualnego(wxCommandEvent& event)
{
    int id = event.GetId();

    bloki[id].panel->Destroy();

    aktualny->produkty[id].Set_usuniety(true);

    //bloki.erase(bloki.begin() + id);

    aktualny_Sizer->Layout();

}

void Okno_host::usun_paragon(wxCommandEvent& event)
{
    int panelID = event.GetId(); // Pobierz ID panelu

    Document data;
    data.SetObject();

    Document::AllocatorType& allocator = data.GetAllocator();


    data.AddMember("typ", 0, allocator);
    data.AddMember("numer", panelID, allocator);

    // Konwersja do ci¹gu znaków
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    data.Accept(writer);

    string jsonString = buffer.GetString();

    // Przygotowanie bufora dla przesy³anych danych
    char bufferToSend[1024];
    std::memset(bufferToSend, 0, sizeof(bufferToSend));
    strncpy(bufferToSend, jsonString.c_str(), sizeof(bufferToSend) - 1);

    // Przesy³anie danych przez gniazdo
    // Przyk³adowa implementacja send()
    for (int p_i = 0; p_i < p.size(); p_i++)
    {
        send(p[p_i]->clientSocket, bufferToSend, strlen(bufferToSend), 0);
    }


    // Usuñ panel o podanym ID
    wxPanel* panelToRemove = nullptr;

    for (int i = 0; i < kupony.size(); i++)
    {
        if (kupony[i]->GetId() == panelID)
        {
            panelToRemove = kupony[i];
            kupony.erase(kupony.begin() + i);
            aktualne.erase(aktualne.begin() + i);
            break;
        }
    }

    if (panelToRemove)
    {
        wxWindow* firstChild = panelToRemove->GetChildren().GetFirst()->GetData();

        // SprawdŸ, czy pierwszy element potomny jest wxStaticText
        if (wxStaticText* staticText = wxDynamicCast(firstChild, wxStaticText)) {
            // Jeœli pierwszy element potomny jest wxStaticText, pobierz jego wartoœæ tekstu
            wxString labelText = staticText->GetLabel();
            int numer_do_usuniecia;
            labelText.ToInt(&numer_do_usuniecia);
            numery.erase(numer_do_usuniecia);
            // Teraz mo¿esz u¿yæ wartoœci tekstu labelText
        }

        panelToRemove->Destroy();
        paragony_Sizer->Layout();
        paragony_Sizer->FitInside(panel_paragony);
    }

    //Drugi ekran

    panelToRemove = nullptr;

    for (int i = 0; i < kupony_ekran.size(); i++)
    {
        if (kupony_ekran[i]->GetId() == panelID)
        {
            panelToRemove = kupony_ekran[i];
            kupony_ekran.erase(kupony_ekran.begin() + i);
            break;
        }
    }

    if (panelToRemove)
    {
        wxWindow* firstChild = panelToRemove->GetChildren().GetFirst()->GetData();

        // SprawdŸ, czy pierwszy element potomny jest wxStaticText
        if (wxStaticText* staticText = wxDynamicCast(firstChild, wxStaticText)) {
            // Jeœli pierwszy element potomny jest wxStaticText, pobierz jego wartoœæ tekstu
            wxString labelText = staticText->GetLabel();
            int numer_do_usuniecia;
            labelText.ToInt(&numer_do_usuniecia);
            numery.erase(numer_do_usuniecia);
            // Teraz mo¿esz u¿yæ wartoœci tekstu labelText
        }

        panelToRemove->Destroy();
        paragony_ekran_Sizer->Layout();
        paragony_ekran_Sizer->FitInside(panel_paragony_ekran);
    }

    int oczeki = Policz_czas_oczekiwania();
    string l = "   " + to_string(oczeki) + " min";


    oczekiwanie->SetLabel(l);
}

void Okno_host::wydaj_produkt(wxCommandEvent& event)
{
    int ID = event.GetId(); // Pobierz ID panelu

    int produktID = ID % 1000;
    int panelID = ID / 1000;

    wxPanel* panelToRemove = nullptr;

    for (int i = 0; i < kupony.size(); i++)
    {
        if (kupony[i]->GetId() == panelID)
        {
            panelToRemove = kupony[i];
            break;
        }
    }
    wxPanel* produkt = nullptr;
    wxWindowList childPanels = panelToRemove->GetChildren();
    for (wxWindow* child : childPanels)
    {
        wxPanel* panel = wxDynamicCast(child, wxPanel);
        if (panel)
        {
            if (panel->GetId() == produktID)
            {
                produkt = panel;
                break;
            }
        }
    }
    if(produkt != nullptr)
    {
        wxWindowList children = produkt->GetChildren();
        for (wxWindow* child : children)
        {
            if (child->IsKindOf(wxCLASSINFO(wxStaticText)))
            {
                wxStaticText* staticText = dynamic_cast<wxStaticText*>(child);
                wxFont font = staticText->GetFont();
                font.SetStrikethrough(true);
                staticText->SetFont(font);

            }
            else if (child->IsKindOf(wxCLASSINFO(wxButton)))
            {
                wxButton* przycisk = dynamic_cast<wxButton*>(child);
                przycisk->Destroy();

            }
        }
    }
    //Drugi ekran
    panelToRemove = nullptr;

    for (int i = 0; i < kupony_ekran.size(); i++)
    {
        if (kupony_ekran[i]->GetId() == panelID)
        {
            panelToRemove = kupony_ekran[i];
            break;
        }
    }
    produkt = nullptr;
    childPanels = panelToRemove->GetChildren();
    for (wxWindow* child : childPanels)
    {
        wxPanel* panel = wxDynamicCast(child, wxPanel);
        if (panel)
        {
            if (panel->GetId() == produktID)
            {
                produkt = panel;
                break;
            }
        }
    }
    if (produkt != nullptr)
    {
        wxWindowList children = produkt->GetChildren();
        for (wxWindow* child : children)
        {
            if (child->IsKindOf(wxCLASSINFO(wxStaticText)))
            {
                wxStaticText* staticText = dynamic_cast<wxStaticText*>(child);
                wxFont font = staticText->GetFont();
                font.SetStrikethrough(true);
                staticText->SetFont(font);

            }
            else if (child->IsKindOf(wxCLASSINFO(wxButton)))
            {
                wxButton* przycisk = dynamic_cast<wxButton*>(child);
                przycisk->Destroy();

            }
        }
    }
    Document data;
    data.SetObject();

    Document::AllocatorType& allocator = data.GetAllocator();


    data.AddMember("typ", 2, allocator);
    data.AddMember("numer", ID, allocator);

    // Konwersja do ci¹gu znaków
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    data.Accept(writer);

    string jsonString = buffer.GetString();

    // Przygotowanie bufora dla przesy³anych danych
    char bufferToSend[1024];
    std::memset(bufferToSend, 0, sizeof(bufferToSend));
    strncpy(bufferToSend, jsonString.c_str(), sizeof(bufferToSend) - 1);

    // Przesy³anie danych przez gniazdo
    // Przyk³adowa implementacja send()
    for (int p_i = 0; p_i < p.size(); p_i++)
    {
        send(p[p_i]->clientSocket, bufferToSend, strlen(bufferToSend), 0);
    }

}

void Okno_host::Historia(wxCommandEvent& event)
{
    wxFrame* okno = new wxFrame(this, wxID_ANY, "Historia");

    //wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(okno, wxID_ANY);

    panel->SetScrollRate(0, FromDIP(10));

    wxGridSizer* panel_sizer = new wxGridSizer(3, 3, 5, 5);

    for (int j = 0; j < paragony.size(); j++)
    {
        wxPanel* para = new wxPanel(panel, paragony[j].numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

        wxBoxSizer* paraSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* numer = new wxStaticText(para, paragony[j].numer, to_string(paragony[j].numer), wxPoint(10, 0), wxDefaultSize);
        wxFont font = numer->GetFont();
        font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
        numer->SetFont(font);

        wxStaticText* czas = new wxStaticText(para, paragony[j].numer, paragony[j].czas, wxPoint(150, 0), wxDefaultSize);

        paraSizer->Add(numer, 0, wxEXPAND);
        paraSizer->Add(czas, 0, wxEXPAND);


        for (int i = 0; i < paragony[j].produkty.size(); i++)
        {
            wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

            string n = paragony[j].produkty[i].Get_nazwa() + " x" + to_string(paragony[j].produkty[i].Get_ilosc());

            wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
            wxFont fontn = name->GetFont();
            fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
            name->SetFont(fontn);
            wxStaticText* notatka = new wxStaticText(prod, i, paragony[j].produkty[i].Get_notatka(), wxPoint(10, 50));
            //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
            //wxFont font = ilosc_teskt->GetFont();
            //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
            //ilosc_teskt->SetFont(font);

            int panelID = paragony[j].numer;
            int produktID = i;

            int id = panelID * 1000 + produktID;

            paraSizer->Add(prod, 0, wxEXPAND);
        }

        wxButton* cofnij = new wxButton(para, paragony[j].numer, "COFNIJ");
        cofnij->Bind(wxEVT_BUTTON, &Okno_host::cofnij_paragon, this, cofnij->GetId());

        paraSizer->Add(cofnij, 0, wxEXPAND);

        para->SetSizer(paraSizer);
        panel_sizer->Add(para, 0, wxEXPAND);
    }

    panel->SetSizer(panel_sizer);

    okno->Show();
    okno->Maximize();
}

void Okno_host::Do_zaplacenia(wxCommandEvent& event)
{
    wxFrame* okno = new wxFrame(this, wxID_ANY, "Historia");

    //wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(okno, wxID_ANY);

    panel->SetScrollRate(0, FromDIP(10));

    wxGridSizer* panel_sizer = new wxGridSizer(3, 3, 5, 5);

    if (!paragony_do_zaplacenia.empty())
    {
        paragony_do_zaplacenia.clear();
    }

    for (int j = 0; j < paragony.size(); j++)
    {
        if (paragony[j].zaplacone == false)
        {
            wxPanel* para = new wxPanel(panel, paragony[j].id, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

            wxBoxSizer* paraSizer = new wxBoxSizer(wxVERTICAL);

            wxStaticText* numer = new wxStaticText(para, paragony[j].numer, to_string(paragony[j].numer), wxPoint(10, 0), wxDefaultSize);
            wxFont font = numer->GetFont();
            font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
            numer->SetFont(font);

            wxStaticText* czas = new wxStaticText(para, paragony[j].numer, paragony[j].czas, wxPoint(150, 0), wxDefaultSize);

            paraSizer->Add(numer, 0, wxEXPAND);
            paraSizer->Add(czas, 0, wxEXPAND);


            for (int i = 0; i < paragony[j].produkty.size(); i++)
            {
                wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

                string n = paragony[j].produkty[i].Get_nazwa() + " x" + to_string(paragony[j].produkty[i].Get_ilosc());

                wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
                wxFont fontn = name->GetFont();
                fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
                name->SetFont(fontn);
                wxStaticText* notatka = new wxStaticText(prod, i, paragony[j].produkty[i].Get_notatka(), wxPoint(10, 50));
                //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragony[paragony.size() - 1].numer, to_string(paragony[paragony.size() - 1].produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
                //wxFont font = ilosc_teskt->GetFont();
                //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
                //ilosc_teskt->SetFont(font);

                int panelID = paragony[j].numer;
                int produktID = i;

                int id = panelID * 1000 + produktID;

                paraSizer->Add(prod, 0, wxEXPAND);
            }

            wxButton* zaplac = new wxButton(para, paragony[j].id, "ZAPLAC");
            zaplac->Bind(wxEVT_BUTTON, &Okno_host::zaplac_paragon_okno, this, zaplac->GetId());

            paraSizer->Add(zaplac, 0, wxEXPAND);

            para->SetSizer(paraSizer);
            panel_sizer->Add(para, 0, wxEXPAND);
            paragony_do_zaplacenia.push_back(para);
        }
    }

    panel->SetSizer(panel_sizer);

    okno->Show();
    okno->Maximize();
}

void Okno_host::cofnij_paragon(wxCommandEvent& event)
{
    int panelID = event.GetId(); // Pobierz ID panelu

    // Usuñ panel o podanym ID
    Paragon* paragonToAdd = nullptr;

    bool dodaj = true;

    for (int i = 0; i < kupony.size(); i++)
    {
        if (kupony[i]->GetId() == panelID)
        {
            wxMessageBox("Ten paragon juz istnieje", "Error", wxOK, this);
            dodaj = false;
            break;
        }
    }



    for (int i = 0; i < paragony.size(); i++)
    {
        if (paragony[i].numer == panelID)
        {
            paragonToAdd = &paragony[i];
            break;
        }
    }

    if (paragonToAdd && dodaj)
    {
        Document data;
        data.SetObject();

        Document::AllocatorType& allocator = data.GetAllocator();


        data.AddMember("typ", 1, allocator);
        data.AddMember("numer", paragonToAdd->numer, allocator);
        data.AddMember("ilosc_produktow", paragonToAdd->produkty.size(), allocator);

        Value czasV;
        czasV.SetString(paragonToAdd->czas.c_str(), static_cast<SizeType>(paragonToAdd->czas.length()), allocator);

        data.AddMember("czas", czasV, allocator);

        Value produkty(kArrayType);


        for (int i = 0; i < paragonToAdd->produkty.size(); i++)
        {
            Value produkt1(kObjectType);

            Value nazwa;
            nazwa.SetString(paragonToAdd->produkty[i].Get_nazwa().c_str(), static_cast<SizeType>(paragonToAdd->produkty[i].Get_nazwa().length()), allocator);

            produkt1.AddMember("nazwa", nazwa, allocator);
            produkt1.AddMember("ilosc", paragonToAdd->produkty[i].Get_ilosc(), allocator);

            Value notatka;
            notatka.SetString(paragonToAdd->produkty[i].Get_notatka().c_str(), static_cast<SizeType>(paragonToAdd->produkty[i].Get_notatka().length()), allocator);

            produkt1.AddMember("notatka", notatka, allocator);
            produkty.PushBack(produkt1, allocator);
        }

        data.AddMember("produkty", produkty, allocator);

        // Konwersja do ci¹gu znaków
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        data.Accept(writer);

        string jsonString = buffer.GetString();

        // Przygotowanie bufora dla przesy³anych danych
        char bufferToSend[1024];
        std::memset(bufferToSend, 0, sizeof(bufferToSend));
        strncpy(bufferToSend, jsonString.c_str(), sizeof(bufferToSend) - 1);

        // Przesy³anie danych przez gniazdo
        // Przyk³adowa implementacja send()
        for (int p_i = 0; p_i < p.size(); p_i++)
        {
            send(p[p_i]->clientSocket, bufferToSend, strlen(bufferToSend), 0);
        }


        wxPanel* para = new wxPanel(panel_paragony, paragonToAdd->numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

        wxBoxSizer* paraSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* numer = new wxStaticText(para, paragonToAdd->numer, to_string(paragonToAdd->numer), wxPoint(10, 0), wxDefaultSize);
        wxFont font = numer->GetFont();
        font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
        numer->SetFont(font);

        wxStaticText* czas = new wxStaticText(para, paragonToAdd->numer, paragonToAdd->czas, wxPoint(150, 0), wxDefaultSize);

        paraSizer->Add(numer, 0, wxEXPAND);
        paraSizer->Add(czas, 0, wxEXPAND);


        for (int i = 0; i < paragonToAdd->produkty.size(); i++)
        {
            wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

            string n = paragonToAdd->produkty[i].Get_nazwa() + " x" + to_string(paragonToAdd->produkty[i].Get_ilosc());

            wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
            wxFont fontn = name->GetFont();
            fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
            name->SetFont(fontn);
            wxStaticText* notatka = new wxStaticText(prod, i, paragonToAdd->produkty[i].Get_notatka(), wxPoint(10, 50));
            //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragonToAdd.numer, to_string(paragonToAdd.produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
            //wxFont font = ilosc_teskt->GetFont();
            //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
            //ilosc_teskt->SetFont(font);

            int panelID = paragonToAdd->numer;
            int produktID = i;

            int id = panelID * 1000 + produktID;

            wxButton* wydaj = new wxButton(prod, id, "WYDAJ", wxPoint(240, 13));

            wydaj->Bind(wxEVT_BUTTON, &Okno_host::wydaj_produkt, this, wydaj->GetId());

            paraSizer->Add(prod, 0, wxEXPAND);
        }

        wxButton* usun = new wxButton(para, paragonToAdd->numer, "WYDANE");
        usun->Bind(wxEVT_BUTTON, &Okno_host::usun_paragon, this, usun->GetId());

        paraSizer->Add(usun, 0, wxEXPAND);

        para->SetSizer(paraSizer);
        paragony_Sizer->Add(para, 0, wxEXPAND);
        paragony_Sizer->FitInside(panel_paragony);
        kupony.push_back(para);
        aktualne.push_back(*paragonToAdd);

        //Drugi ekran

        para = new wxPanel(panel_paragony_ekran, paragonToAdd->numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

        paraSizer = new wxBoxSizer(wxVERTICAL);

        numer = new wxStaticText(para, paragonToAdd->numer, to_string(paragonToAdd->numer), wxPoint(10, 0), wxDefaultSize);
        font = numer->GetFont();
        font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
        numer->SetFont(font);

        czas = new wxStaticText(para, paragonToAdd->numer, paragonToAdd->czas, wxPoint(150, 0), wxDefaultSize);

        paraSizer->Add(numer, 0, wxEXPAND);
        paraSizer->Add(czas, 0, wxEXPAND);


        for (int i = 0; i < paragonToAdd->produkty.size(); i++)
        {
            wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

            string n = paragonToAdd->produkty[i].Get_nazwa() + " x" + to_string(paragonToAdd->produkty[i].Get_ilosc());

            wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
            wxFont fontn = name->GetFont();
            fontn.SetPointSize(14);  // Ustawienie wiêkszego rozmiaru czcionki
            name->SetFont(fontn);
            wxStaticText* notatka = new wxStaticText(prod, i, paragonToAdd->produkty[i].Get_notatka(), wxPoint(10, 50));
            //wxStaticText* ilosc_teskt = new wxStaticText(prod, paragonToAdd.numer, to_string(paragonToAdd.produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
            //wxFont font = ilosc_teskt->GetFont();
            //font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
            //ilosc_teskt->SetFont(font);

            int panelID = paragonToAdd->numer;
            int produktID = i;

            int id = panelID * 1000 + produktID;

            paraSizer->Add(prod, 0, wxEXPAND);
        }

        para->SetSizer(paraSizer);
        paragony_ekran_Sizer->Add(para, 0, wxEXPAND);
        paragony_ekran_Sizer->FitInside(panel_paragony_ekran);
        kupony_ekran.push_back(para);

        wxMessageBox("Przywrócono pragon!", "Success", wxOK, this);

    }

}

void Okno_host::dodaj_klienta(wxCommandEvent& evt)
{
    Polaczenie* tmp = new Polaczenie();
    p.push_back(tmp);
}
void Okno_host::reset_klienta(wxCommandEvent& evt)
{

    panel_paragony->Layout();
    panel_paragony_ekran->Layout();

    Document data;
    data.SetObject();

    Document::AllocatorType& allocator = data.GetAllocator();


    data.AddMember("typ", 4, allocator);

    // Konwersja do ci¹gu znaków
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    data.Accept(writer);

    string jsonString = buffer.GetString();

    // Przygotowanie bufora dla przesy³anych danych
    char bufferToSend[1024];
    std::memset(bufferToSend, 0, sizeof(bufferToSend));
    strncpy(bufferToSend, jsonString.c_str(), sizeof(bufferToSend) - 1);

    // Przesy³anie danych przez gniazdo
    // Przyk³adowa implementacja send()
    for (int p_i = 0; p_i < p.size(); p_i++)
    {
        send(p[p_i]->clientSocket, bufferToSend, strlen(bufferToSend), 0);
    }
}

int Okno_host::Policz_czas_oczekiwania()
{
    int result = 0;
    int i_bureger = 0;
    int i_steki = 0;
    int i_comber = 0;
    for (const Paragon& paragon : aktualne) {
        for (Produkt produkt : paragon.produkty) {
            if (comparePhrases(produkt.Get_nazwa(), "Stek"))
            {
                i_steki += produkt.Get_ilosc();
            }
            else if (comparePhrases(produkt.Get_nazwa(), "Comber"))
            {
                i_comber += produkt.Get_ilosc();
            }
            else if (comparePhrases(produkt.Get_nazwa(), "Wolowy") || comparePhrases(produkt.Get_nazwa(), "Maly wolowy") || comparePhrases(produkt.Get_nazwa(), "Jagniecy") || comparePhrases(produkt.Get_nazwa(), "Kozlecy"))
            {
                i_bureger += produkt.Get_ilosc();
            }
        }
    }

    result += (i_bureger / 4 * 6) + (i_steki * 8) + (i_comber/3 * 10);
    if (i_bureger % 4 != 0)
    {
        result += 6;
    }

    return result;

}

void Okno_host::zaplac_paragon(wxCommandEvent& event)
{
    int panelID = event.GetId(); // Pobierz ID panelu
    // Usuñ panel o podanym ID
    wxPanel* panelToRemove = nullptr;

    for (int i = 0; i < kupony.size(); i++)
    {
        if (kupony[i]->GetId() == panelID)
        {
            panelToRemove = kupony[i];
            aktualne[i].zaplacone = true;
            paragony[aktualne[i].id].zaplacone = true;
            break;
        }
    }
    wxWindowList& children = panelToRemove->GetChildren();
    if (!children.empty())
    {
        // ZnajdŸ ostatnie dziecko
        wxWindowListNode* lastNode = children.GetLast();
        wxWindow* lastChild = lastNode->GetData();

        // Zniszcz ostatnie dziecko
        lastChild->Destroy();
    }
}

void Okno_host::zaplac_paragon_okno(wxCommandEvent& event)
{
    int panelID = event.GetId(); // Pobierz ID panelu
    // Usuñ panel o podanym ID
    wxPanel* panelToRemove = nullptr;

    paragony[panelID].zaplacone = true;

    for (int i = 0; i < paragony_do_zaplacenia.size(); i++)
    {
        if (paragony_do_zaplacenia[i]->GetId() == panelID)
        {
            panelToRemove = paragony_do_zaplacenia[i];
            paragony_do_zaplacenia.erase(paragony_do_zaplacenia.begin() + i);
            break;
        }
    }
    if (panelToRemove)
    {
        panelToRemove->Destroy();
        //paragony_ekran_Sizer->Layout();
        //paragony_ekran_Sizer->FitInside(panel_paragony_ekran);
    }


}
