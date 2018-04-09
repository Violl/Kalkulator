// POP 2015-01-20 projekt 2 Rucinska Malgorzata EIT 7 160718
// Dev-C++ 5.11 TDM-GCC 4.9.2 64-bit Release
#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <string.h>
using namespace std;

void Czyszczenie_Wiersza(string pobrany_wiersz, char *czysty_wiersz);
bool Czy_Poprawny_Wiersz(char *czysty_wiersz);
void InfixNaONP(char *czysty_wiersz, char *ONP);
float ObliczONP(char *ONP);

int main()
{
	fstream plik, plik_wynik;
	float wynik;
	
	cout << "Witaj w programie!\n" << endl;
	
	plik.open("plik.txt");
	plik_wynik.open("pomocniczy.txt", fstream::out | fstream::trunc);
	if (plik.good())																		//sprawdzanie czy plik, ktory chcemy otworzyc istnieje
	{			
		if (plik_wynik.good())
		{
			string pobrany_wiersz;															//wiersz ktory bedzie kolejna linia z pliku
			bool poprawny_wiersz = false;
			
			cout<<"Plik zostal prawidlowo odczytany\nOto poprawione i rozwiazane zadania:\n"<< endl;
			while (!plik.eof())																// przeszukiwanie calego pliku
			{
				getline(plik, pobrany_wiersz);
				char czysty_wiersz[512] = {0} ;									//wiersz bez spacji, wszystkie przecinki beda zamienione na kropki, i wszystkie znaki poprzedzajace # beda usuniete
				char ONP[512] = {0};

				Czyszczenie_Wiersza(pobrany_wiersz, czysty_wiersz);				//czyscimy wczytany wiersz ze wszystkich znakow do # wlacznie, ze zbednych spacji, oraz zamieniamy przecinki na kropki
				poprawny_wiersz = Czy_Poprawny_Wiersz(czysty_wiersz);
				if (poprawny_wiersz)
				{
					InfixNaONP(czysty_wiersz, ONP);											//zamieniamy wczytany, wyczyszczony wiersz na wersje odwroconej notacji polskiej
					wynik = ObliczONP(ONP);													//obliczamy wartosc wyrazenia odwroconej notacji polskiej
					plik_wynik << pobrany_wiersz << " " << wynik << endl;
					cout << czysty_wiersz << " " << wynik << endl << endl;
				}

				else
				{
					plik_wynik << pobrany_wiersz << " err" << endl;
					cout << pobrany_wiersz << " err\n"<< endl;
				}
			}
		}
	}
	else cout << "Plik nie istnieje.\nUtworz plik tekstowy o nazwie plik i umiesc w nim przyklady do rozwiazania,\nzaczynajac kazde zadanie od znaku # i konczac na znaku =. \n";
	plik.close();
	plik_wynik.close();
	remove("plik.txt");
	rename("pomocniczy.txt","plik.txt");
	return 0;
}

void Czyszczenie_Wiersza(string pobrany_wiersz, char *czysty_wiersz)
{
	bool wystapil_hash = false;
	int j = 0;
	for (int i = 0; i < pobrany_wiersz.length(); i++)
	{
		if (pobrany_wiersz[i] == '#')
		{
			wystapil_hash = true;
			continue;
		}
		if (wystapil_hash)
		{
			if (pobrany_wiersz[i] == ' ')
				continue;
			czysty_wiersz[j] = pobrany_wiersz[i];
			if (pobrany_wiersz[i] == ',')
				czysty_wiersz[j] = '.';
			j++;
		}
	}
	czysty_wiersz[j] = 0;
	wystapil_hash = false;
}

bool Czy_Poprawny_Wiersz(char *czysty_wiersz)
{
	int dlugosc_wiersza = strlen(czysty_wiersz);
	for (int i = 0; i < dlugosc_wiersza; i++)
	{
		if (czysty_wiersz[i] == 's' || czysty_wiersz[i] == 'S')			//szukanie funkcji sin() lub sqrt()
		{
			if (czysty_wiersz[i + 1] == 'i' || czysty_wiersz[i + 1] == 'I')
				if (czysty_wiersz[i + 2] == 'n' || czysty_wiersz[i + 2] == 'N')
					if (czysty_wiersz[i + 3] == '(')
					{
						i += 3;								//dodajemy do i tyle pozycji ile sprawdzilismy i kontynuujemy sprawdzanie poprawnosci rownania
						continue;
					}
			if (czysty_wiersz[i + 1] == 'q' || czysty_wiersz[i + 1] == 'Q')
				if (czysty_wiersz[i + 2] == 'r' || czysty_wiersz[i + 2] == 'R')
					if (czysty_wiersz[i + 3] == 't' || czysty_wiersz[i + 3] == 'T')
						if (czysty_wiersz[i + 4] == '(')
						{
							i += 4;
							continue;
						}
		}
		if (czysty_wiersz[i] == 'c' || czysty_wiersz[i] == 'C')			//szukanie funkcji cos()
			if (czysty_wiersz[i + 1] == 'o' || czysty_wiersz[i + 1] == 'O')
				if (czysty_wiersz[i + 2] == 's' || czysty_wiersz[i + 2] == 'S')
					if (czysty_wiersz[i + 3] == '(')
					{
						i += 3;
						continue;
					}
		if ((czysty_wiersz[i]<'(' || czysty_wiersz[i]>'9' )&& czysty_wiersz[i] != '='&&czysty_wiersz[i] != '^')		//sprawdzanie czy w czystym wierszu nie ma jakis nieporzadanych znakow
			return false;
		if (czysty_wiersz[i] == '.')																				//sprawdzanie czy po kropce jest jakas inna liczba
			if (czysty_wiersz[i + 1]<'0' || czysty_wiersz[i + 1]>'9')
				return false;
	}
	return true;
}

void InfixNaONP(char *czysty_wiersz, char *ONP)
{
	int licznik = 0;
	char stos[500];
	int indeks_ONP = 0;
		for (int i = 0; i < strlen(czysty_wiersz); i++)
		{
			char znak = czysty_wiersz[i];
			if ((znak >= '0' && znak <= '9') || znak == '.')
				if ((czysty_wiersz[i + 1] < '0' || czysty_wiersz[i + 1] > '9') && czysty_wiersz[i + 1] != '.')
				{
					ONP[indeks_ONP++] = znak;
					ONP[indeks_ONP++] = ' ';
				}
				else
					ONP[indeks_ONP++] = znak;
			else
			{
				if(znak == '(')
					stos[licznik++] = znak;

				if (znak == '-' || znak == '+')			//jesli jest dodawanie lub odejmowanie to zrzucamy ze stosu wszystkie operatory o wiekszym lub tym samym priorytecie 
				{
					while (stos[licznik - 1] == '+' || stos[licznik - 1] == '-' || stos[licznik - 1] == '*' || stos[licznik - 1] == '/' || stos[licznik - 1] == '^')
					{
						ONP[indeks_ONP++] = stos[--licznik];
					}
					stos[licznik++] = znak;
				}

				if (znak == '*'|| znak == '/')			//jesli jest mnozenie lub dzielenie to zrzucamy ze stosu wszystkie operatory o wiekszym lub tym samym priorytecie 
				{
					while (stos[licznik - 1] == '*' || stos[licznik - 1] == '/' || stos[licznik - 1] == '^')
					{
						ONP[indeks_ONP++] = stos[--licznik];
					}
					stos[licznik++] = znak;
				}
				if (znak == '^')
					stos[licznik++] = znak;
				
				if (znak == 's' || znak == 'S')
				{
					if (czysty_wiersz[i + 1] == 'i' || czysty_wiersz[i + 1] == 'I')
					{
						stos[licznik++] = 's';
						i += 3;
					}
					if (czysty_wiersz[i + 1] == 'q' || czysty_wiersz[i + 1] == 'Q')
					{
						stos[licznik++] = 'q';
						i += 4;
					}
				}
				if (znak == 'c' || znak == 'C')
				{
					stos[licznik++] = 'c';
					i += 3;
				}
				
				if (znak == ')')			//jesli jest prawy nawias to sciagamy ze stosu operatory az do rozpoczecia nawiasu albo jakiejs funkcji, i jesli to jest funkcja to ja tez zrzucamy na wyscie
				{
					while (stos[licznik - 1] != '(' && stos[licznik - 1] != 'c' && stos[licznik - 1] != 'q' && stos[licznik - 1] != 's')	
						ONP[indeks_ONP++] = stos[--licznik];
					if (stos[licznik - 1] == 'c' || stos[licznik - 1] == 'q' || stos[licznik - 1] == 's')
						ONP[indeks_ONP++] = stos[--licznik];
				}
			}
		}
		licznik--;
		while (licznik >= 0)
		{
			if (stos[licznik] != ')'&&stos[licznik] != '(')
				ONP[indeks_ONP++] = stos[licznik];
			licznik--;
		}
		ONP[indeks_ONP] = 0;
}

float ObliczONP(char *ONP)
{
	int dlugosc_wiersza = strlen(ONP);
	float liczby[20] = {0};
	char liczba_char[30] = {0};
	int indeks_tablicy_liczb=0;									//przechowuje indeks ostatniej dodanej liczby do tablicy float
	int indeks_liczby = 0;										//indeks liczby ktora zostanie z tekstu zamieniona na float
	for (int i = 0; i < dlugosc_wiersza; i++)
	{
		if ((ONP[i] >= '0'&&ONP[i] <= '9') || ONP[i] == '.')	//transformacja char na float i dodawanie liczb na stos
		{
			liczba_char[indeks_liczby++] = ONP[i];
			continue;
		}
		if (ONP[i] == ' ')								//jesli jest spacja to ma zakonczyc wpisywac znaki do tablicy liczba i przeksztalcic to co tam sie znajduje na float i dodac go do stosu liczb
		{
			liczba_char[indeks_liczby] = 0;
			liczby[indeks_tablicy_liczb++] = atof(liczba_char);
			indeks_liczby = 0;
			continue;
		}

		if(ONP[i]=='+')											//dodawanie dwoch elementow na szczycie stosu z wynikiem w drugim elemencie, oraz zmniejszenie wielkosci stosu o 1
			if (indeks_tablicy_liczb >= 2)
			{
				liczby[indeks_tablicy_liczb - 2]=liczby[indeks_tablicy_liczb - 2] + liczby[indeks_tablicy_liczb - 1];
				indeks_tablicy_liczb--;
				continue;
			}
		if (ONP[i] == '-')									//reszta operacji analogicznie oprocz potegowania
			if (indeks_tablicy_liczb >= 2)
			{
				liczby[indeks_tablicy_liczb - 2] = liczby[indeks_tablicy_liczb - 2] - liczby[indeks_tablicy_liczb - 1];
				indeks_tablicy_liczb--;
				continue;
			}
		if (ONP[i] == '*')
			if (indeks_tablicy_liczb >= 2)
			{
				liczby[indeks_tablicy_liczb - 2] = liczby[indeks_tablicy_liczb - 2] * liczby[indeks_tablicy_liczb - 1];
				indeks_tablicy_liczb--;
				continue;
			}
		if (ONP[i] == '/')
			if (indeks_tablicy_liczb >= 2)
			{
				liczby[indeks_tablicy_liczb - 2] = liczby[indeks_tablicy_liczb - 2] / liczby[indeks_tablicy_liczb - 1];
				indeks_tablicy_liczb--;
				continue;
			}
		if (ONP[i] == '^')																
			if (indeks_tablicy_liczb >= 2)	//pierwsza liczba na stosie jest wykladnikiem wiec ustalamy ja jako granice iterowania, i tyle razy mnozymy druga liczbe na stosie przez jej poczatkowa wartosc
			{
				float x = liczby[indeks_tablicy_liczb - 2];
				for (int j = 1; j < liczby[indeks_tablicy_liczb - 1]; j++)
				{
					liczby[indeks_tablicy_liczb - 2] = liczby[indeks_tablicy_liczb - 2] * x;
				}
				indeks_tablicy_liczb--;
				continue;
			}
		if (ONP[i] == 'c')					//kazda funkcja bierze pierwsza liczbe ze stosu liczb i korzystajac z gotowych funkcji z biblioteki math.h oblicza wynik i zapisuje go z powrotem na stos
			if (indeks_tablicy_liczb >= 1)
			{
				liczby[indeks_tablicy_liczb - 1] = cos(liczby[indeks_tablicy_liczb - 1]);
				continue;
			}
		if (ONP[i] == 's')
			if (indeks_tablicy_liczb >= 1)
			{
				liczby[indeks_tablicy_liczb - 1] = sin(liczby[indeks_tablicy_liczb - 1]);
				continue;
			}
		if (ONP[i] == 'q')
			if (indeks_tablicy_liczb >= 1)
			{
				liczby[indeks_tablicy_liczb - 1] = sqrt(liczby[indeks_tablicy_liczb - 1]);
				continue;
			}

	}
	return liczby[0];
}
