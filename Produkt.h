#pragma once
#include <string>
#include <vector>

using namespace std;


class Produkt
{
public:
	Produkt(string n, int i);
	Produkt(string n, int i, string notaktka);
	Produkt(string n, int i, bool c);
	int Get_ID();
	int Get_ilosc();
	int Get_ilosc_do_sprzedazy();
	void Set_ilosc(int i);
	void Set_ilosc_do_sprzedazy(int i);
	string Get_nazwa();
	void Set_notatka(string nowa);
	string Get_notatka();
	bool Get_usuniety();
	void Set_usuniety(bool us);

	bool Get_check();
	void Set_check(bool c);

private:
	string nazwa;
	int id;
	int ilosc;
	string notatka;
	bool usuniety;
	int ilosc_do_sprzedazy;
	vector<int>id_do_usuniecia;
	bool check;
};