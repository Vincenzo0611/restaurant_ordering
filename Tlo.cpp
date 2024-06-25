#include "Tlo.h"

void* BackgroundThread::Entry()
{
	char buffer[4096];
	int bytesReceived = 0;

	do
	{
		std::memset(buffer, 0, sizeof(buffer));

		bytesReceived = recv(*sock, buffer, sizeof(buffer), 0);

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
				p->czas = parsedData["czas"].GetString();


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

				paragony->push_back(*p);
				//dodaj_produkt(p);
				wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, NUMBER_UPDATE_ID);
				parent->GetEventHandler()->AddPendingEvent(event);
			}
			else if (k == 0)
			{

				int id = parsedData["numer"].GetInt();

				wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, NUMBER_USUN_ID);
				event.SetInt(id);  // pass some data along the event, a number in this case
				parent->GetEventHandler()->AddPendingEvent(event);
			}
			else if (k == 2)
			{

				int id = parsedData["numer"].GetInt();

				wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, NUMBER_WYDAJ_ID);
				event.SetInt(id);  // pass some data along the event, a number in this case
				parent->GetEventHandler()->AddPendingEvent(event);
			}
			else if (k == 4)
			{
				wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, RESET_PARAGONOW);
				parent->GetEventHandler()->AddPendingEvent(event);
			}
		}

	} while (bytesReceived > 0);

	return NULL;
}