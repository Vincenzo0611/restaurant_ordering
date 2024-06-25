#include "Ilosc_bulek.h"

Ilosc_bulek::Ilosc_bulek() : wxFrame(nullptr, wxID_ANY, "Bulki")
{
    wxString line;
    wxTextFile file(filename);

    if (file.Open()) {
        line = file.GetFirstLine();
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxT("Duze bulki:"), wxPoint(10,10));
        txtDuzeBulki = new wxTextCtrl(this, wxID_ANY, line, wxPoint(10, 30), wxDefaultSize);
        line = file.GetNextLine();
        wxStaticText* label2 = new wxStaticText(this, wxID_ANY, wxT("Male bulki:"), wxPoint(10, 50));
        txtMaleBulki = new wxTextCtrl(this, wxID_ANY, line, wxPoint(10, 70), wxDefaultSize);
        file.Close();
    }

}

Ilosc_bulek::~Ilosc_bulek()
{
    wxTextFile file(filename);

    if (file.Open()) {
        file.Clear();
        file.AddLine(txtDuzeBulki->GetValue());
        file.AddLine(txtMaleBulki->GetValue());
        file.Write();
        file.Close();
    }
}