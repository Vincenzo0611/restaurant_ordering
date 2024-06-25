#pragma once
#include <vector>
#include "Produkt.h"

using namespace std;

class Paragon
{
public:
	Paragon(int n);
	int id;
	int numer;
	bool zaplacone;
	string czas;
	vector<Produkt> produkty;
};