#pragma once
#include <wx/wx.h>
#include <string>

using namespace std;

class Blok_paragonu
{
public:
	Blok_paragonu();

	string nazwa;
	int ilosc;
	wxPanel* panel;
	wxButton* plus;
	wxButton* minus;
	wxButton* usun;
	wxStaticText* name;
	wxTextCtrl* notatka;
	wxTextCtrl* ilosc_teskt;
};