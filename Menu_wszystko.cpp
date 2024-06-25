#include "Menu_wszystko.h"
#include <fstream>

bool comparePhrases1(const std::string& str1, const std::string& phrase) {
    // SprawdŸ, czy fraza wystêpuje w obu ci¹gach
    return (str1.find(phrase) != std::string::npos);
}

Menu_wszystko::Menu_wszystko()
{
    ifstream plik("m.txt"); // Otwieramy plik z danymi


    string linia;

    int id = 0; // Inicjujemy ID

    // Wczytujemy nazwy produktów z pliku i tworzymy produkty na ich podstawie
    while (std::getline(plik, linia)) 
    {
        // Tworzymy nowy produkt na podstawie wczytanej linii
        Produkt* p = new Produkt(linia, id);

        if (comparePhrases1(linia, "Wolowy") || comparePhrases1(linia, "Maly wolowy") || comparePhrases1(linia, "Jagniecy") || comparePhrases1(linia, "Kozlecy") || comparePhrases1(linia, "Stek") || comparePhrases1(linia, "Comber") || comparePhrases1(linia, "Ziemniaki") || comparePhrases1(linia, "Dodatki") || comparePhrases1(linia, "Ciasto") || comparePhrases1(linia, "Kompot") || comparePhrases1(linia, "Herbata") || comparePhrases1(linia, "Kawa") || comparePhrases1(linia, "Bulki duze") || comparePhrases1(linia, "Bulki male") || comparePhrases1(linia, "Talerz wege") )
        {
            p->Set_check(true);
        }
        produkty.push_back(*p);
        id++; // Zwiêkszamy ID dla nastêpnego produktu
    }

    id_DB = id;
    Produkt* p = new Produkt("Duze bulki", id++);
    p->Set_check(true);
    produkty.push_back(*p);
    id_MB = id;
    p = new Produkt("Male bulki", id++);
    p->Set_check(true);
    produkty.push_back(*p);

    plik.close(); // Zamykamy plik
}