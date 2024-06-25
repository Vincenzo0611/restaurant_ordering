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
using namespace rapidjson;

class Okno_ustawiania_menu : public wxFrame
{
public:
    Okno_ustawiania_menu();
    ~Okno_ustawiania_menu();
    void LoadDataFromFile(const wxString& filename, wxListBox* listBox);
    void SaveDataToFile(const wxString& filename, wxListBox* listBox);
    void OnAddProduct(wxCommandEvent& event);
    void OnRemoveProduct(wxCommandEvent& event);
private:
    wxString filename = "m.txt";
    wxListBox* listBox;
    wxTextCtrl* txtNewProduct;
    wxButton* btnAdd;
    wxButton* btnRemove;
    wxComboBox* cmbPosition;
};


//BEGIN_EVENT_TABLE(Okno_klient, wxFrame)
//EVT_COMMAND (NUMBER_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, Okno_klient::onNumberUpdate)
//END_EVENT_TABLE()