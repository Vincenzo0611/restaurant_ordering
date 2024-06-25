#include "Okno_ustawiania_menu.h"

Okno_ustawiania_menu::Okno_ustawiania_menu() : wxFrame(nullptr, wxID_ANY, "Menu_ustawienia")
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Sizer dla pierwszej linii (txtNewProduct i cmbPosition)
    wxBoxSizer* firstLineSizer = new wxBoxSizer(wxHORIZONTAL);

    // Pole tekstowe do wprowadzania nowych produkt�w
    txtNewProduct = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    firstLineSizer->Add(txtNewProduct, 1, wxEXPAND | wxALL, 5);

    // Kombiwidget do wyboru pozycji
    cmbPosition = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN);
    cmbPosition->Append(wxT("Na pocz�tku"));
    cmbPosition->Append(wxT("Na ko�cu"));
    cmbPosition->Append(wxT("Przed wybran� pozycj�"));
    firstLineSizer->Add(cmbPosition, 1, wxEXPAND | wxALL, 5);

    // Dodaj pierwszy sizer do g��wnego sizer
    mainSizer->Add(firstLineSizer, 0, wxEXPAND | wxALL, 5);

    // Sizer dla drugiej linii (btnAdd i btnRemove)
    wxBoxSizer* secondLineSizer = new wxBoxSizer(wxHORIZONTAL);

    // Przycisk "Dodaj" do dodawania nowych produkt�w
    btnAdd = new wxButton(this, wxID_ANY, wxT("Dodaj"), wxDefaultPosition, wxDefaultSize, 0);
    secondLineSizer->Add(btnAdd, 0, wxALIGN_CENTER | wxALL, 5);

    // Przycisk "Usu�" do usuwania wybranych produkt�w
    btnRemove = new wxButton(this, wxID_ANY, wxT("Usu�"), wxDefaultPosition, wxDefaultSize, 0);
    secondLineSizer->Add(btnRemove, 0, wxALIGN_CENTER | wxALL, 5);

    // Dodaj drugi sizer do g��wnego sizer
    mainSizer->Add(secondLineSizer, 0, wxEXPAND | wxALL, 5);

    listBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB);
    LoadDataFromFile(filename, listBox);
    mainSizer->Add(listBox, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(mainSizer);
    this->Layout();

    // Pod��czamy zdarzenie dla przycisku "Dodaj"
    btnAdd->Bind(wxEVT_BUTTON, &Okno_ustawiania_menu::OnAddProduct, this);
    btnRemove->Bind(wxEVT_BUTTON, &Okno_ustawiania_menu::OnRemoveProduct, this);
}

Okno_ustawiania_menu::~Okno_ustawiania_menu()
{
    this->SaveDataToFile(this->filename, this->listBox);
}

void Okno_ustawiania_menu::LoadDataFromFile(const wxString& filename, wxListBox* listBox) {
    wxString line;
    wxTextFile file(filename);

    if (file.Open()) {
        for (line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
            listBox->Append(line);
        }
        file.Close();
    }
}

// Zapisywanie danych do pliku txt
void Okno_ustawiania_menu::SaveDataToFile(const wxString & filename, wxListBox * listBox) {
    wxTextFile file(filename);

    if (file.Open()) {
        file.Clear();
        for (unsigned int i = 0; i < listBox->GetCount(); ++i) {
            file.AddLine(listBox->GetString(i));
        }
        file.Write();
        file.Close();
    }
}

void Okno_ustawiania_menu::OnAddProduct(wxCommandEvent& event) {
    wxString newProduct = txtNewProduct->GetValue();
    wxString selectedPosition = cmbPosition->GetStringSelection(); // Pobierz wybran� pozycj�

    if (!newProduct.IsEmpty()) {
        if (selectedPosition == "Na pocz�tku") {
            listBox->Insert(newProduct, 0); // Dodaj na pocz�tku listy
        }
        else if (selectedPosition == "Na ko�cu") {
            listBox->Append(newProduct); // Dodaj na ko�cu listy
        }
        else {
            int index = listBox->GetSelection(); // Pobierz indeks wybranej pozycji
            if (index != wxNOT_FOUND) {
                listBox->Insert(newProduct, index); // Dodaj przed wybran� pozycj�
            }
            else {
                wxMessageBox("Prosz� wybra� pozycj� na li�cie.", "Uwaga", wxOK | wxICON_WARNING, this);
            }
        }
        txtNewProduct->Clear(); // Wyczy�� pole tekstowe
    }
}

void Okno_ustawiania_menu::OnRemoveProduct(wxCommandEvent& event) {
    int selectedIndex = listBox->GetSelection();
    if (selectedIndex != wxNOT_FOUND) {
        listBox->Delete(selectedIndex);
    }
}