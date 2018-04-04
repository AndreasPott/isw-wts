// Rechenbeispiel zur Vorlesung: "Entwickung von wissenschaftlich technischer Software"
// Stand: 15.03.2017
//
// Copyright (c) 2017-2018, Andreas Pott
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

struct Base {
	virtual string getName()=0;
};

struct A : public Base {
	string getName() { return "Ich bin A"; }
};

struct B : public Base {
	string getName() { return "Ich bin B"; }
};

struct Customname : public Base {
	string myName;
	string getName() { return myName; }
};

void virtuellerTyp()
{
	list<Base*> aList;
	A einA;
	B einB;
	Customname CN;
	CN.myName = "Ich bin Foo";

	aList.push_back(&einA);
	aList.push_back(&einB);
	aList.push_back(&CN);
	for (auto itor=aList.begin(); itor!=aList.end(); itor++)
		cout << (*itor)->getName() << endl;
};

int main(int argc, char* argv[])
{
	virtuellerTyp();
	return 0;
}