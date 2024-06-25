#pragma once
#include <wx/wx.h>
#include <vector>
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

const int NUMBER_UPDATE_ID = 100000;
const int NUMBER_USUN_ID = 900000;
const int NUMBER_WYDAJ_ID = 200000;
const int RESET_PARAGONOW = 300000;

using namespace std;
using namespace rapidjson;

class BackgroundThread : public wxThread
{
public:
	BackgroundThread(wxFrame* parent, vector<Paragon>* paragony, SOCKET* sock) : wxThread(wxTHREAD_DETACHED)
	{
		this->parent = parent;
		this->sock = sock;
		this->paragony = paragony;
	}
	virtual void* Entry();

private:
	wxFrame* parent;
	vector<Paragon>* paragony;
	SOCKET* sock;
};