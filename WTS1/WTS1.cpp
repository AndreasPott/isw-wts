// Rechenbeispiel zur Vorlesung: "Entwickung von wissenschaftlich technischer Software"
// Stand: 15.03.2017
//
// Copyright (c) 2017, Andreas Pott
//
// Hiermit wird unentgeltlich jeder Person, die eine Kopie der Software und der 
// zugeh�rigen Dokumentationen (die "Software") erh�lt, die Erlaubnis erteilt, 
// sie uneingeschr�nkt zu nutzen, inklusive und ohne Ausnahme mit dem Recht, sie 
// zu verwenden, zu kopieren, zu ver�ndern, zusammenzuf�gen, zu ver�ffentlichen, 
// zu verbreiten, zu unterlizenzieren und/oder zu verkaufen, und Personen, denen 
// diese Software �berlassen wird, diese Rechte zu verschaffen, unter den 
// folgenden Bedingungen:
//
// Der obige Urheberrechtsvermerk und dieser Erlaubnisvermerk sind in allen 
// Kopien oder Teilkopien der Software beizulegen.
//
// DIE SOFTWARE WIRD OHNE JEDE AUSDR�CKLICHE ODER IMPLIZIERTE GARANTIE 
// BEREITGESTELLT, EINSCHLIESSLICH DER GARANTIE ZUR BENUTZUNG F�R DEN VORGESEHENEN 
// ODER EINEM BESTIMMTEN ZWECK SOWIE JEGLICHER RECHTSVERLETZUNG, JEDOCH NICHT 
// DARAUF BESCHR�NKT. IN KEINEM FALL SIND DIE AUTOREN ODER COPYRIGHTINHABER F�R 
// JEGLICHEN SCHADEN ODER SONSTIGE ANSPR�CHE HAFTBAR ZU MACHEN, OB INFOLGE DER 
// ERF�LLUNG EINES VERTRAGES, EINES DELIKTES ODER ANDERS IM ZUSAMMENHANG MIT DER 
// SOFTWARE ODER SONSTIGER VERWENDUNG DER SOFTWARE ENTSTANDEN.
//

#include <iostream>
#include <string>
#include <list>

using namespace std;

class Complex {
private:
  // Daten
  double Re,Im;  
public:
  // Konstruktor
  Complex() : Re(0.0), Im(0.0) {}
  // Methoden
  void add(Complex& val);
  void printCartesian();
  void printPolar();
};

int main(int argc, char* argv[])
{
	// unser vector
	double vector[9]={0};

	// ohne DRY
	vector[1]=1;
	vector[3]=1;
	vector[4]=1;
	vector[6]=1;

	// alternative nach DRY
	int indices[]={1,3,4,6};
	for (int i=0; i<sizeof(indices)/sizeof(indices[0]); i++)
	  vector[indices[i]] = 1;

	std::cout << sizeof(indices)/sizeof(indices[0]) << std::endl;

	return 0;
}

