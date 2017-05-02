// Rechenbeispiel zur Vorlesung: "Entwickung von wissenschaftlich technischer Software"
// Stand: 15.03.2017
//
// Copyright (c) 2017, Andreas Pott
//
// Hiermit wird unentgeltlich jeder Person, die eine Kopie der Software und der 
// zugehörigen Dokumentationen (die "Software") erhält, die Erlaubnis erteilt, 
// sie uneingeschränkt zu nutzen, inklusive und ohne Ausnahme mit dem Recht, sie 
// zu verwenden, zu kopieren, zu verändern, zusammenzufügen, zu veröffentlichen, 
// zu verbreiten, zu unterlizenzieren und/oder zu verkaufen, und Personen, denen 
// diese Software überlassen wird, diese Rechte zu verschaffen, unter den 
// folgenden Bedingungen:
//
// Der obige Urheberrechtsvermerk und dieser Erlaubnisvermerk sind in allen 
// Kopien oder Teilkopien der Software beizulegen.
//
// DIE SOFTWARE WIRD OHNE JEDE AUSDRÜCKLICHE ODER IMPLIZIERTE GARANTIE 
// BEREITGESTELLT, EINSCHLIESSLICH DER GARANTIE ZUR BENUTZUNG FÜR DEN VORGESEHENEN 
// ODER EINEM BESTIMMTEN ZWECK SOWIE JEGLICHER RECHTSVERLETZUNG, JEDOCH NICHT 
// DARAUF BESCHRÄNKT. IN KEINEM FALL SIND DIE AUTOREN ODER COPYRIGHTINHABER FÜR 
// JEGLICHEN SCHADEN ODER SONSTIGE ANSPRÜCHE HAFTBAR ZU MACHEN, OB INFOLGE DER 
// ERFÜLLUNG EINES VERTRAGES, EINES DELIKTES ODER ANDERS IM ZUSAMMENHANG MIT DER 
// SOFTWARE ODER SONSTIGER VERWENDUNG DER SOFTWARE ENTSTANDEN.
//

#include <math.h>
#include <list>
#include <iostream>

// Rechenbeispiel zur Vorlesung: "Entwickung von wissenschaftlich technischer Software"
// Folgende Teilaspekte der Vorlesung sollen in diesem Programmierbeispiel durch Übungen vorgestellt und vertieft werden
// * Programmdesign
// * Konkrete Typen für Matrizen und Vektoren
// * Vererbung und Polymorphismus für Gelenke und Starrkörper (Basisklasse mit simplen abgeleiteten Klassen für P, R, rigid)
// * Hierarchien (sowohl kind-of als part-of) lassen sich an dem Vererbungsbeispiel erklären
//		Kind-of für die (potenziellen) mechanischen Komponenten des Roboters
//		Part-of für die Komposition des Roboters
// * Design Pattern
//		Container und Iteratoren für das Speichern und Traversieren von gleichen Elementen
//		Fassade für die API
//		Kompositum als (map-chain)
//		Decorator z.B. für Bewegungsmessung am End-Effektor
//		Einzelstück für einen Interpreter
//		Kommando ggf. eine triviale Bewegungsplanung
// Kapselung und Invarianten anhand einer Parametrisierung der Rotationsmatrix (auch im ebenen Fall möglich)
// Dokumentation mit doxygen
// Analyse der Qualität mit cpplint, cppcheck
// 
// Geplante bzw. mögliche Erweiterungen
// * Scripting mit Python für elementare Funktionen des Roboters (simple API)
// * Arbeit mit einem Versionierungssystem
// * einfache Toolchain (Interaktion zwischen Python und C++), Automatisierung des eigenen Codes
// * Codegenerierung für die DK eines komplexeren Teilsystems welches z.B. in Python oder Maple symbolisch erstellt wurde
// * Integration des Rechenmodells in ein ereignisorientiertes Programmgerüst


// Mathematische Grundobjekte
///////////////////////////////////////////////////////////////////////////////

// Diese Klasse repräsentiert einen konkreten Typ eine Vektors im zwei-dimensionalen Vektorraum
// Die beiden Variablen x,y stellen dabei die Koordianten des Vektors in Euklidischer 
// Parametrisierung dar. 
class Vector2 
{
public:
	double x,y;	// die Koordianten des Vektors
public:
	// der Standard-Konstruktor erzeugt einen Nullvektor
	Vector2() : x(0.0), y(0.0) {}
	// der Parametrische Konstruktor erlaubt auch das direkte Setzen der Koordinaten
	Vector2(double X, double Y) : x(X), y(Y) {}
	// die Operation add addiert zu den aktuellen Vektor den Parameter value dazu und gibt das ergebnis zurück
	Vector2 add(const Vector2& value)
	{
		Vector2 result;
		result.x = x + value.x;
		result.y = x + value.y;
		return result;
	}

	// die komfortablere Variante der Funktion "add" mit Operatorüberladung. 
	Vector2& operator+=(const Vector2& value)
	{
		x+=value.x;
		y+=value.y;
		return *this;
	}
};


// Die Klasse RotationMatrix2 ist ebenfalls ein konkreter Typ und repräsentiert eine Rotations- 
// bzw. Transformationsmatrix in der Ebene.
// Da die Rotation als Matrix anschaulich gesehen vier Elementen aber nur eine Invariante 
// (nämlich den Winkel phi) hat,
// wird die Matrix nur durch ihre Invariante phi parametrisiert. Den Benutzer der Klasse muss es 
// nicht stören, das die Klasse intern nur den Winkel speichert.
class RotationMatrix2
{
private:
	double phi;	// der Winkel der Rotation, gleichzeitig die invariante des Matrix, 
public:
	RotationMatrix2() : phi(0.0) {}
	// den Rotationswinkel einstellen
	void set(double angle) { phi=angle; }
	// den Drehwinkel ermitteln
	double getAngle() { return phi; }
	// eine Rotationsmatrix mit einem Winkel weiterdrehen 
	void rotate(double angle) { phi+=angle; }
	// eine Rotationsmatrix mit einer anderen Multiplizieren
	void rotate(RotationMatrix2 R) { phi+=R.phi; }
	// einen Vektor transformieren (drehen) und das Ergebnis zurückgeben; 
	// hier wird intern die Matrixschreibweise sichtbar
	Vector2 rotate(Vector2 v) 
	{
		return Vector2(
			 cos(phi)*v.x - sin(phi)*v.y,
			 sin(phi)*v.x + cos(phi)*v.y);
	}
};


// Mechanische Elemente des Roboters
///////////////////////////////////////////////////////////////////////////////

// Das MechanicalElement ist ein Abstrakter Typ, der sich nicht instanziieren lässt.
// MechanicalElement definiert das Interface in Form seiner virtuellen Funktionen sowie die
// von allen abgeleiteten Klassen zu nutzenden Daten (position, orientierung, eingang).
// das Paar (r,R) repräsentiert dabei ein bewegliches Koordinatensystem in der Ebene
class MechanicalElement
{
protected:
	// private Daten der Klasse
	Vector2 r;	// Position
	RotationMatrix2 R;  // Orientierung
	MechanicalElement *input; // Eingang der kinematischen Kette
public:
	MechanicalElement() : input(0) {}
	
	// Konkrete Dienste der Klasse
	// Position des Koordiantensystems 
	Vector2 getPosition() { return r; }
	// Orientierung des Koordiantensystems ausgeben
	RotationMatrix2 getOrientation() { return R; }

	// Interface der Klasse, rein virtuell (Abstraktion)
	// berechne die Vorwärtskinematik des Elements aus den aktuellen Zuständen
	virtual void doKinematics()=0;
};

// Die Klass WorldFrame repräsentiert das inertialsystem und tut im Grunde nicht viel außer dafür zu sorgen,
// dass die Daten auf null gesetzt werden. 
class WorldFrame : public MechanicalElement
{
public:
	// leerer Konstruktor
	WorldFrame() {}

	// die "Kinematik" berechnen, die darin besteht, das Koordinatensystem in den Urspung zu fixieren.
	virtual void doKinematics()
	{
		// die Funktion erzeugt immer das Null-Koordinatensystem
		r.x=0;
		r.y=0;
		R.set(0.0);
	}
};

// das erste "echte" Mechanische Element. Ein Drehgelenk dreht das 
// Ausgangskoordinatensystem gegenüber dem Eingang. Die Position bleibt 
// konstant. Der Winkel theta fungiert als weiterer Eingang des System 
// und definiert die Position des Drehgelenks
class RevoluteJoint : public MechanicalElement
{
public:
	double theta;		// der aktuelle Drehwinkel des Gelenks in lokalen Koordinaten
public:
	// der Konstruktor benötigt das Eingangselemt, an den das Gelenk geknüpft ist
	explicit RevoluteJoint(MechanicalElement& Input) : theta(0) 
	{
		input = &Input;
	}

	// die Kinematik berechnen das Ausgangskoordiantensystem (inform der Member r,R) aus dem Eingang und 
	// dem lokalen Zustand
	virtual void doKinematics()
	{
		r = input->getPosition();
		R.rotate(input->getOrientation());
		R.rotate(theta);
	}
};

// ein weiteres Mechanische Element: Ein starrer Körper repräsentiert die konstante Verschiebung (ein Arm-Segment)
// zwischen Eingang und Ausgang. Die Orientierung bleibt Konstant während die Position verschoben wird.
// der wert translation entspricht dabei der geometrischen Größe des modellierten Körpers
class RigidBody : public MechanicalElement
{
public:
	Vector2 translation; // die Länge des starren Körpers in lokalen Koordinaten
public:
	// der Konstruktor benötigt das Eingangselemt, an den das Gelenk geknüpft ist
	explicit RigidBody(MechanicalElement& Input) 
	{
		input = &Input;
	}
	
	virtual void doKinematics()
	{
		R = input->getOrientation();

		Vector2 dr;
		dr = R.rotate(translation);
		r = input->getPosition();
		r = r.add(dr);
	}
};

// Die Klasse KinematicChain repräsentiert eine kinematische Kette von mechanischen Übertragunsggliedern
// Die Klasse nutzt das Design Pattern Kompositum, da es eine Menge von Mechaischen Übertragungsgliedern 
// zusammenfasst und dem ergebnis wiederum die gleiche Schnittstelle des Basiselements gibt
// Das Kompositum stützt auch das DRY Paradigma, da die Zusammenstellung der kinematische Kette nur einmalig 
// erfolgt, und die Kette als ganze nun von der KinematicChain repräsentiert (abstrahiert) wird. 
class KinematicChain : public MechanicalElement
{
private:
	// ein Container (hier doppelt verkettete Liste) erlaubt beliebig viele Elemente zu sammeln
	std::list<MechanicalElement*> chain; 
public:
	KinematicChain() {}
	// füngt ein Element in die Kette ein
	KinematicChain& add(MechanicalElement& element)
	{
		// wir könnten an dieser Stelle die Implementierung robust machen, indem wir prüfen, ob das 
		// übergebene Element bereits teil der Kette ist

		// speichert einen Zeiger auf das Element in der Kette
		chain.push_back(&element);
		return *this;
	}
public:
	// berechnet die Kinematik. Für eine Kette besteht diese Funktion nur aus der Organisation 
	virtual void doKinematics()
	{
		// die Kinematik für alle Elemente berechnen; mit dem Iterator kann man bequem die
		// Liste traversieren. 
		for (auto itor=chain.begin(); itor!=chain.end(); itor++)
			// der Aufruf der virtuellen Funktion wird automatisch für den jeweils zuständen Typ ausgeführt
			(*itor)->doKinematics();
	}
};

// Diese Klasse repräsentiert einen 2R Roboter, d.h. eine Roboter bestehen aus zwei Drehgelenken mit
// zwei Arm Segmenten.
// Die Klasse folgt dem Design Pattern "Facade" da sie eine Schnittstelle für einen ganze Anzahl von 
// einzelelemente zur Verfügung stellt.
// Die Klasse stellt konzeptionell eine höhere Abstraktion dar, als die Element. Man erkannt dies auch 
// daran, dass kaum noch Mathematische Basisberechnunge ausgeführt werden.
class Robot2R
{
public:
	// Einen Roboter erstellen
	KinematicChain robot;

	// die Bausteine des Roboters
	WorldFrame K0;		// das Inertialsystem
	RevoluteJoint R1;	// das erste Drehgelenk
	RigidBody B1;		// das erste Armsegment
	RevoluteJoint R2;	// das zweite Drehgelenk
	RigidBody B2;		// das zweite Armsegment

public:
	// der Konstruktor, die Initialisierung unserer konkreten Bausteine mit ihren Vorgängern muss hier
	// erfolgten
	Robot2R() : R1(K0), B1(R1), R2(B1), B2(R2) 
	{
		// die Geometrie des Roboters definieren
		B1.translation.x=0.5;
		B2.translation.x=0.3;

		// den Roboter Zusammensetzen
		robot.add(K0);
		robot.add(R1);
		robot.add(B1);
		robot.add(R2);
		robot.add(B2);		
	}

	// berechnet die Kinematik des Roboters; die Parameter theta1, theta2 übergeben 
	// die Gelenkpositionen des Roboters. Das Ergebnis der Berechnung wird als Zustand
	// gespeichert. 
	void doKinematics(const double& theta1, const double& theta2)
	{
		R1.theta = theta1;
		R2.theta = theta2;
		robot.doKinematics();
	}
	
	// gibt den Zustand des Roboters auf dem Bildschrim aus
	void printTCP()
	{
		std::cout << "Position des End-Effektors: (" <<
			B2.getPosition().x << " ; " <<
			B2.getPosition().y << " ; " <<
			B2.getOrientation().getAngle() << " )\n";
	}
};


// das erste Use-Case Szenario
void Robot_Base_Test1(void)
{
	// Einen Roboter erstellen
	std::list<MechanicalElement*> robot;

	// seine Komponenten erstellen
	WorldFrame K0;
	RevoluteJoint R1(K0);
	RigidBody B1(R1);
	RevoluteJoint R2(B1);
	RigidBody B2(R2);

	// die Komponenten Konfigurieren
	B1.translation.x=0.5;
	B2.translation.x=0.3;
	R1.theta = 45.0/180.0*3.1415926;
	R2.theta = -45.0/180.0*3.1415926;

	// den Roboter Zusammensetzen
	robot.push_back(&K0);
	robot.push_back(&R1);
	robot.push_back(&B1);
	robot.push_back(&R2);
	robot.push_back(&B2);

	// die Kinematik für alle Elemente berechnen
	for (auto itor=robot.begin(); itor!=robot.end(); itor++)
		(*itor)->doKinematics();

	// berechnete Position ausgeben
	std::cout << "Position des End-Effektors: (" <<
		B2.getPosition().x << " ; " <<
		B2.getPosition().y << " )\n";

	// eine schleife mit einem Iterator über dem Container
	for (auto itor=robot.begin(); itor!=robot.end(); itor++)
		std::cout << "Position der Komponenten: (" <<
			(*itor)->getPosition().x << " ; " <<
			(*itor)->getPosition().y << " ; " <<
			(*itor)->getOrientation().getAngle() << 
			" )\n";
}

// das zweite Use-Case Szenario
void Robot_Base_Test2(void)
{
	Robot2R robot;
	robot.doKinematics(45.0/180.0*3.1415926, -45.0/180.0*3.1415926);
	robot.printTCP();
}
