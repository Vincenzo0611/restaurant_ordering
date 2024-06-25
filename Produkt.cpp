#include "Produkt.h"

Produkt::Produkt(string n, int i)
{
	this->nazwa = n;
	this->id = i;
	this->ilosc = 1;
	this->notatka = "";
	this->usuniety = false;
}
Produkt::Produkt(string n, int i, string notatka)
{
	this->nazwa = n;
	this->id = i;
	this->ilosc = 1;
	this->notatka = notatka;
}

Produkt::Produkt(string n, int i, bool c)
{
	this->nazwa = n;
	this->id = i;
	this->ilosc = 1;
	this->notatka = "";
	this->usuniety = false;
	this->check = c;
}

int Produkt::Get_ID()
{
	return this->id;
}

string Produkt::Get_nazwa()
{
	return this->nazwa;
}

int Produkt::Get_ilosc()
{
	return this->ilosc;
}

int Produkt::Get_ilosc_do_sprzedazy()
{
	return this->ilosc_do_sprzedazy;
}
void Produkt::Set_ilosc(int i)
{
	this->ilosc = i;
}

void Produkt::Set_ilosc_do_sprzedazy(int i)
{
	this->ilosc_do_sprzedazy = i;
}
void Produkt::Set_notatka(string nowa)
{
	this->notatka = nowa;
}
string Produkt::Get_notatka()
{
	return this->notatka;
}

bool Produkt::Get_usuniety()
{
	return this->usuniety;
}
void Produkt::Set_usuniety(bool us)
{
	this->usuniety = us;
}

bool Produkt::Get_check()
{
	return this->check;
}
void Produkt::Set_check(bool c)
{
	this->check = c;
}