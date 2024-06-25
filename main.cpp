#include <wx/wx.h>
#include "Glowne_okno_klient.h"
#include "Okno_menu.h"
#include "Tlo.h"
#include "Okno_ustawiania_menu.h"
#include "Ilosc_bulek.h"

class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, "Start") {}

    void OnButton1Clicked(wxCommandEvent& event)
    {
        // Zamknięcie bieżącego okna
        Close();

        // Otwarcie nowego okna
        Okno_menu* menu = new Okno_menu();
        menu->Maximize();
        menu->Show();
    }
    void OnButton3Clicked(wxCommandEvent& event)
    {
        // Otwarcie nowego okna
        Okno_ustawiania_menu* ustawienia = new Okno_ustawiania_menu();
        ustawienia->Maximize();
        ustawienia->Show();
    }    
    
    void OnButton4Clicked(wxCommandEvent& event)
    {
        // Otwarcie nowego okna
        Ilosc_bulek* ustawienia = new Ilosc_bulek();
        ustawienia->Maximize();
        ustawienia->Show();
    }


    void OnButton2Clicked(wxCommandEvent& event)
    {
        // Zamknięcie bieżącego okna
        Close();
        this->Close();
        wxString userInput = input->GetValue();  // Get the user's input as wxString
        std::string inputString = userInput.ToStdString();

        Okno_klient* klient = new Okno_klient(inputString);
        //klient->Maximize();
        klient->Show();
        //klient->startListeningInBackground();
        BackgroundThread* t = new BackgroundThread(klient, &klient->paragony, &klient->sock);
        
        wxThreadError err = t->Create();

        if (err != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Couldn't create thread!"));
        }

        err = t->Run();

        if (err != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Couldn't run thread!"));
        }
    }

    wxTextCtrl* input;
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_BUTTON(0, MyFrame::OnButton1Clicked)
EVT_BUTTON(1, MyFrame::OnButton2Clicked)
EVT_BUTTON(2, MyFrame::OnButton3Clicked)
EVT_BUTTON(3, MyFrame::OnButton4Clicked)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Okno_klient, wxFrame)
EVT_COMMAND(NUMBER_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::onNumberUpdate)
EVT_COMMAND(NUMBER_USUN_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::Usun_message)
EVT_COMMAND(NUMBER_WYDAJ_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::wydaj_message)
EVT_COMMAND(RESET_PARAGONOW, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::reset_paragonow)
wxEND_EVENT_TABLE()

class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame* frame = new MyFrame(NULL);
        frame->CreateStatusBar();
        button1 = new wxButton(frame, 0, "HOST", wxPoint(10, 70));

        frame->input = new wxTextCtrl(frame, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
        button2 = new wxButton(frame, 1, "KLIENT", wxPoint(10, 20));

        button3 = new wxButton(frame, 2, "Ustawienia menu", wxPoint(10, 100));
        button4 = new wxButton(frame, 3, "Ilosc bulek", wxPoint(10, 140));
      
        frame->Show();

        return true;
    }

private:
    wxButton* button1;
    wxButton* button2;
    wxButton* button3;
    wxButton* button4;
};

wxIMPLEMENT_APP(MyApp);
