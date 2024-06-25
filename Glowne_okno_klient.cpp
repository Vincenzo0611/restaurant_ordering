#include "Glowne_okno_klient.h"

Okno_klient::Okno_klient(string ipAddress) : wxFrame(nullptr, wxID_ANY, "Klient")
{
	int port = 12345;						// Listening port # on the server


	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		wxMessageBox("Can't start Winsock, Err #");
		return;
	}

	// Create socket
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		wxMessageBox("Can't create socket, Err #");
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr) <= 0) {
		wxMessageBox("\nInvalid address/ Address not supported \n");
		WSACleanup();
		return;
	}

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		wxMessageBox("Can't connect to server, Err #");
		closesocket(sock);
		WSACleanup();
		return;
	}


	panel_paragony = new wxScrolled<wxPanel>(this, wxID_ANY);

	panel_paragony->SetScrollRate(0, FromDIP(25));

	paragony_Sizer = new wxGridSizer(3, 3, 5, 5);

	panel_paragony->SetSizer(paragony_Sizer);

	// Ustawienie fokusu na okno po jego utworzeniu
	panel_paragony->SetFocus();
}

/*
void Okno_klient::startListeningInBackground()
{
	wxThread* thread = new BackgroundThread(this, &paragony, &sock);
	//thread->SetDetach(true);
	thread->Run();
}

void Okno_klient::backgroundThread()
{
	char buffer[4096];
	int bytesReceived;

	do
	{
		std::memset(buffer, 0, sizeof(buffer));

		bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

		if (bytesReceived > 0)
		{

			Document parsedData;
			parsedData.Parse(buffer);

			int k = parsedData["typ"].GetInt();

			if (k == 1)
			{

				Paragon* p = new Paragon(0);
				// Przetwarzanie danych
				p->numer = parsedData["numer"].GetInt();
				int ilosc_produktow = parsedData["ilosc_produktow"].GetInt();


				const Value& produktyArray = parsedData["produkty"];
				for (SizeType i = 0; i < produktyArray.Size(); i++) {
					const Value& produkt = produktyArray[i];
					string nazwa = produkt["nazwa"].GetString();
					int ilosc = produkt["ilosc"].GetInt();
					string notatka = produkt["notatka"].GetString();

					Produkt* pro = new Produkt(nazwa, 0, notatka);

					pro->Set_ilosc(ilosc);

					p->produkty.push_back(*pro);

				}

				paragony.push_back(*p);
				//dodaj_produkt(*p);
			}
		}

	} while (bytesReceived > 0);
}
*/
void Okno_klient::dodaj_produkt(Paragon p)
{
	wxPanel* para = new wxPanel(panel_paragony, p.numer, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

	para->SetCanFocus(false);

	wxBoxSizer* paraSizer = new wxBoxSizer(wxVERTICAL);


	wxStaticText* numer = new wxStaticText(para, p.numer, to_string(p.numer), wxPoint(10, 0), wxDefaultSize);
	wxFont font = numer->GetFont();
	font.SetPointSize(20);  // Ustawienie wiêkszego rozmiaru czcionki
	numer->SetFont(font);
	wxStaticText* czas = new wxStaticText(para, paragony[paragony.size() - 1].numer, p.czas, wxPoint(150, 0), wxDefaultSize);

	paraSizer->Add(numer, 0, wxEXPAND);
	paraSizer->Add(czas, 0, wxEXPAND);

	for (int i = 0; i < p.produkty.size(); i++)
	{
		wxPanel* prod = new wxPanel(para, i, wxDefaultPosition, wxDefaultSize);

		string n = p.produkty[i].Get_nazwa() + " x" + to_string(p.produkty[i].Get_ilosc());

		wxStaticText* name = new wxStaticText(prod, i, n, wxPoint(10, 10));
		wxFont fontn = name->GetFont();
		fontn.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
		name->SetFont(fontn);
		wxStaticText* notatka = new wxStaticText(prod, i, p.produkty[i].Get_notatka(), wxPoint(10, 50));
		//wxStaticText* ilosc_teskt = new wxStaticText(prod, p.numer, to_string(p.produkty[i].Get_ilosc()), wxPoint(200, 10), wxSize(40, 30));
		//wxFont font = ilosc_teskt->GetFont();
		//font.SetPointSize(16);  // Ustawienie wiêkszego rozmiaru czcionki
		//ilosc_teskt->SetFont(font);
		paraSizer->Add(prod, 0, wxEXPAND);
	}

	para->SetSizer(paraSizer);
	paragony_Sizer->Add(para, 0, wxEXPAND);
	paragony_Sizer->FitInside(panel_paragony);
	kupony.push_back(para);
}


void Okno_klient::onNumberUpdate(wxCommandEvent& evt)
{
	this->dodaj_produkt(paragony[paragony.size() - 1]);
}

void Okno_klient::Usun_message(wxCommandEvent& evt)
{
	this->usun_paragon(evt.GetInt());
}
void Okno_klient::wydaj_message(wxCommandEvent& evt)
{
	this->wydaj_produkt(evt.GetInt());
}

void Okno_klient::usun_paragon(int panelID)
{
	wxPanel* panelToRemove = nullptr;

	for (int i = 0; i < kupony.size(); i++)
	{
		if (kupony[i]->GetId() == panelID)
		{
			panelToRemove = kupony[i];
			kupony.erase(kupony.begin() + i);
			break;
		}
	}

	if (panelToRemove)
	{
		panelToRemove->Destroy();
		paragony_Sizer->Layout();
		paragony_Sizer->FitInside(panel_paragony);
	}
}

void Okno_klient::wydaj_produkt(int ID)
{
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
		}
	}
}

void Okno_klient::reset_paragonow(wxCommandEvent& evt)
{	
	panel_paragony->Layout();
}

Okno_klient::~Okno_klient()
{


	closesocket(sock);
	WSACleanup();
}

