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
  void set(const double& re, const double& im);
  double real() const;
  double imag() const;
  void add(const Complex& val);
  void printCartesian() const;
  void printPolar();
};

void Complex::set(const double& re, const double& im)
{
	Re=re;
	Im=im;
}

double Complex::real() const
{ 
	return Re;
}

double Complex::imag() const
{
	return Im;
}

void Complex::add(const Complex& val)
{
	Re+=val.Re;
	Im+=val.Im;
}

void Complex::printCartesian() const
{
	std::cout << Re << ',' << Im << std::endl;
}

int main(int argc, char* argv[])
{
	// Klasese complex
	Complex a,b;
	a.printCartesian();
	a.set(1,2);
	a.printCartesian();
	b.set(3,4);
	a.add(b);
	a.printCartesian();


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

