//------------------------------------------------------------
//Benutzungsbeispiel:
//
//	#include "RandomGenerator.h"
//	double meineZufallszahl = RandomGenerator::uniform(0,1);
//
//Erzeugt mithilfe einer Gleichverteilung einer Zufallszahl
//aus dem Intervall [0,1[
//------------------------------------------------------------

#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <complex>
#include <QtCore/QObject>

namespace NeuroMath{
//Dies ist kein magischen Zahlen..
//Könnte auch irgendwas anderes negatives (keine 0!) sein.
//Dieser Wert dient der Initialisierung von RandomGenerator::m_defaultSeed.
#define DETERMINISTIC_SEED_IDUM -11524	//sollte negativ sein

// Diese Werte brauche Ich für den Zufalls-Algorithmus aus den "Numerical Recipes".
// Die wurden (genau wie der Algorithmus in funktion _shuffle(qint32)) aus diesem
// Buch kopiert.
#define IA 16807
#define IM 2147483647
#define AM (1.0)/IM
#define	IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#ifdef EPS
#undef EPS
#endif
#define EPS 1.2E-7
#define RNMX (1.0-EPS)

/**
* \author Nicolas Luck (nicolas.luck@udo.edu)
* \date 01.02.2006
* \brief Ein Singleton, dass PHOTOSS mit Zufallszahlen versorgt.
*
* Historie:
* Ich habe diese Klasse aus der GPC::rndGenerator gebaut. Die wesentlichen Algorithmen zur Berechnung der
* Zufallszahlen habe ich einfach übernommen. Zuvor wurde in jeder Komponente, die Zufallszahlen benötigt,
* ein GPC::rndGenerator erzeugt. Dieses Vorgehen macht aber Probleme, da so in jeder Komponente abgefragt
* werden muss, ob wir deterministische oder statistische Zufallswerte haben wollen (bei deterministischen
* ist die Zufallsfolge immer die Gleiche, so dass die Ergebnisse einer Simulation auch immer gleich sind.
* Dies wird benötigt, um Simulationsergebnisse vergleichbar zu machen). Erreicht wird dies, indem man den
* Zufallsgenerator mit konstanten Seed-Werten initialisiert. Also musste auch jede Komponente, abhängig von
* den SimulationParameters, den Zufallsgenerator mit festen Werten oder z.B. mit der Systemzeit initialisieren
* (wenn dies von dem Komponentenprogrammierer vergessen wurde, waren Zufallszahlen statistisch, auch wenn
* der Benutzer in den SimulationParameters deterministic ausgewählt hatte).
* Ein weiteres Problem war, dass Zufallsgeneratoren, die schnell hintereinader herzeugt wurden, die gleiche
* Folge von Zufallszahlen geliefert haben, da sie mit den gleichen Seed-Werten initialisiert wurden.
* Idee: besser wär es, wenn es  nur ein Objekt gäbe, das für die Erzeugung von Zufallszahlen zuständig ist.
* <br>
*
* Aktuell:
* RandomGenerator ist ein Singleton, d.h. es ist sichergestellt (durch private Konstruktoren), dass in einem
* Projekt nur ein Objekt dieser Klasse existieren kann. Die Klasse enthält statische, öffentliche Funktionen
* wie <tt>uniform(const double min, const double max)</tt>, die Zufallswerte zurückgeben. Diese statischen
* Funktionen bekommen über die Funktion <tt>getInstance()</tt> einen Zeiger auf das RandomGenerator Objekt
* und rufen seine _<Verteilung>(..) - Funktionen auf.
* Über die Funktionen <tt>setDeterministic()</tt> und <tt>setRandom()</tt> wird gesteuert, ob die
* Zufallswerte wirklich zufällig (naja, von der Systemzeit abh.) oder deterministisch sind.
* Diese Funktionen werden nur an einer Stelle in PHOTOSS aufgerufen. Kurz bevor die Simulation gestartet
* wird, in der <tt>GlobalSimFkt()</tt>, in der Datei PhotossDoc.cpp.
* 
* Alle Verteilungen (für die "public static" Funktionen existieren) habe ich am 10.02.2006 zusammen mit
* Martin Windmann getestet.
* (1Mio. Werte erzeugen und in eine Datei schreiben lassen. Anschliessend mit Matlab sichergestellt, dass
* die Zufallswerte unkorreliert sind und das Histogramm die jeweilige, für die Verteilung charakteristische,
* Form aufweist.)
*/
class  RandomGenerator : public QObject {
	Q_OBJECT
private:
		/**
		* Privater Konstruktor.
		* Niemand, außer uns, darf einen RandomGenerator erzeugen!
		*
		* Benutzt <tt>NewRandomSeed()</tt> um den seed mithilfe der Systemzeit zu initialisieren.
		* D.h. Default ist statistisch.
		*/
		RandomGenerator();

		/**
		* Privater Destruktor.
		* Niemand, außer uns, darf einen RandomGenerator zerstören!
		*/
		~RandomGenerator();
		
		/**
		* Privater (leerer) Kopier-Konstruktor.
		* Niemand darf einen RandomGenerator kopieren!
		*/
                RandomGenerator(const RandomGenerator&):QObject() {};

		/**
		* Setzt einen festen Seed.
		* Wird von <tt>setDeterministic()</tt> benötigt.
		*/
		void seed(const qint32 idum);

		/**
		* Erzeugt einen zufälligen Seed mithilfe der Systemzeit.
		* Wird vom Konstruktor benutzt.
		*/
		void NewRandSeed();


		/**
		* Diese Funktion war vorher GPC::ran(long).
		* Ist aus den "Numerical Recipes" kopiert.
		* Idum wird mit dem seed initialisiert. Wird bei jedem Aufruf geändert. Siehe "Numerical Recipes" Kapitel 7.
		*
		* Liefert eine Zahl zwischen 0.0 und 1.0 (Grenzen exklusiv).
		* Benutzt "Bays-Durham shuffle", um unkorrelierte Zufallszahlen zu erzeugen.
		*/
		double _shuffle(qint32 *idum);

		/**
		* Aus gpc_random.h kopiert (und umbenannt).
		* Benutzt <tt>_shuffle(qint32)</tt>
		*/
		double _uniform(const double xmin, const double xmax);
		/**
		* Aus den "Numerical Recipes".
		* Benutzt <tt>_shuffle(qint32 *idum)</tt>.
		*/
		double _gauss(const double mean, const double var);
		/**
		* Von GPC::rndGenerator übernommen.
		* Benutzt <tt>_uniform(const double, const double)</tt>.
		*/
		double _poisson(const double lambda);
		/**
		* Von GPC::rndGenerator übernommen.
		* Benutzt <tt>_uniform(const double, const double)</tt> und <tt>_poisson(const double)</tt>
		*/
		double _fastpoisson(const double lambda);
		/**
		* Von GPC::rndGenerator übernommen.
		*/
		double _gammln(const double xx);
		/**
		* Von GPC::rndGenerator übernommen.
		* Benutzt <tt>_uniform(const double, const double)</tt>.
		*/
		double _pareto(double MinValue, double MeanValue);
		
		// Generation of negative exponentially distributed random numbers
		/**
		* Aus gpc_random.h kopiert (und umbenannt).
		* Benutzt <tt>_shuffle(qint32)</tt>
		*/
		double _negExp(double lambda);

		/**
		* Diese Variable repräsentiert den Zustand des Generators.
		* Seed setzen bedeutet, diese Variable zu setzen.
		* Eine neue Zufallszahl wird hieraus berechnet und auch hierein geschrieben,
		* so dass diese Variablen sich mit jedem aufruf von _shuffle() ändert.
		* (die anderen Verteilungen werden mithilfe einer Gleichverteilung emuliert, d.h.
		* es wird auch dann _shuffle() aufgerufen) 
		*
		* Ursprünglich war idum vom Typ long. Laut MSDN
		* (http://msdn.microsoft.com/en-us/library/s3f49ktz(VS.80).aspx)
		* ist long das gleiche wie int, nämlich ein Interger der Größe 4 Byte.
		* Daher wird hier nun unmissveständlich ein 32-Bit-Argument verwendet,
		* da QtScript Probleme mit long hat.

		*/
		qint32 idum;


		/**
		* Diese Variable enthält den Wert, auf den idum standardmäßig beim Aufruf von
		* #setDeterministic() ohne Argumente gesetzt wird.
		*
		* \sa #setDefaultSeed(), #getDefaultSeed()
		*/
		qint32 m_defaultIdum;

		/**
		* Der Algorithmus zum berechnen der gaussverteilten Zufallszahlen erzeugt immer
		* zwei Zahlen. Wir brauchen aber erstmal nur eine. Die zweite wird gespeichert und
		* dieses Flag wird auf true gesetzt damit beim nächsten aufruf von gauss() nicht
		* wieder eine Zufallszahl berechnet werden muss sondern die zuvor gespeicherte
		* zurückgeliefert wird.
		* In setDeterministic() muss dieses Flag auf false gesetzt werden, da es sonst seien
		* kann, dass die erste gauss-Zahl abweichen kann..
		*/
		bool gaussAlreadyComputed;

public:
	/**
	* So kommen unsere statischen Member-Funktionen an das RandomGenerator Objekt.
	*/
	static RandomGenerator*	getInstance();

public slots:


	/**
	* Liefert eine, zwischen den Grenzen gleichverteilte, zufällig Zahl.
	* Wobei das Intervall, in dem die Zufallszahl liegt, zu beiden Seiten offen ist:
	*	]xmin,xmax[		
	*
	* \param xmin Untere Schranke des Intervalls (exklusiv)
	* \param xmax Obere Schranke des Intervalls (exklusiv)
	* \return Zufällige Zahl aus ]xmin,xmax[
	*/
	static double			uniform(double xmin, double xmax);

	/**
	* Ruft uniform(double,double) mit den real-Anteilen der Komplexen-Parameter auf.
	*/
	static std::complex<double> uniform(std::complex<double> xmin, std::complex<double> xmax){
		return uniform(xmin.real(), xmax.real());
	}


        //                  1             /     (x - mean)²   \
	//	pfd(x) = ---------------- Exp(  - ---------------  )
	//           Wurzel(2 PI var)     \       2 var       /
	/**
	* Liefert eine normal (=gauss) verteilte, zufällige Zahl.
	* 
	* \param mean Mittelwert der Verteilung
	* \param var Varianz der Verteilung
	* Also die Parameter der Gaussverteilung
	*/
	static double			gauss(double mean, double var);

	/**
	* Ruft gauss(double,double) mit den real-Anteilen der Komplexen-Parameter auf.
	*/
	static std::complex<double> gauss(std::complex<double> mean, std::complex<double> var){
		return gauss(mean.real(), var.real());
	}

	/**
	* Liefert eine pareto verteilte, zufällige Zahl.
	* 
	* \param MinValue Minimum (=Keine Zufallszahl ist kleiner als diese Wert)
	* \param MeanValue "Mittelwert"..?
	*/
	static double			pareto(double MinValue, double MeanValue);

	/**
	* Ruft pareto(double,double) mit den real-Anteilen der Komplexen-Parameter auf.
	*/
	static std::complex<double> pareto(std::complex<double> MinValue, std::complex<double> MeanValue){
		return pareto(MinValue.real(), MeanValue.real());
	}

	/**
	* Liefert eine poisson verteilte, zufällig Zahl.
	* 
	* \param lambda Parameter der Poissonverteilung (=Erwartungswert=Varianz=3. zentriertes Moment)
	*/
	static double			poisson(double lambda);

	/**
	* Ruft poisson(double) mit dem real-Anteil des Komplexen-Parameters auf.
	*/
	static std::complex<double> poisson(std::complex<double> lambda){
		return poisson(lambda.real());
	}


	/**
	* Liefert eine poisson verteilte, zufällig Zahl.
	* Ist etwa doppelt so schnell, wie <tt>poisson(double)</tt>.
	*
	* \param lambda Parameter der Poissonverteilung (=Erwartungswert=Varianz=3. zentriertes Moment)
	*/
	static double			fastpoisson(double lambda);

	/**
	* Ruft fastpoisson(double) mit dem real-Anteil des Komplexen-Parameters auf.
	*/
	static std::complex<double> fastpoisson(std::complex<double> lambda){
		return fastpoisson(lambda.real());
	}

	
	/**
	* Liefert eine negativ exponential verteilte, zufällige Zahl.
	*
	* \param lambda Parameter der Exponentialverteilung
	*/
	static double			negExp(double lambda);

	/**
	* Ruft negExp(double) mit dem real-Anteil des Komplexen-Parameters auf.
	*/
	static std::complex<double> negExp(std::complex<double> lambda){
		return negExp(lambda.real());
	}


	/**
	* Setzt den Zufallsgenerator auf deterministisch.
	* D.h. es wird immer die gleiche Folge von Zufallswerten erzeugt.
	* 
	* Der default-seed bleibt von dieser Funktion unbeeinflusst.
	* 
	* Ursprünglich war das Argument vom Typ long. Laut der MSDN
	* (http://msdn.microsoft.com/en-us/library/s3f49ktz(VS.80).aspx)
	* ist long das gleiche wie int, nämlich ein Interger der Größe 4 Byte.
	* Daher wird hier nun unmissveständlich ein 32-Bit-Argument verwendet,
	* da QtScript Probleme mit long hat.
	*
	* Achtung! idum sollte negativ sein!
	*/
	static void setDeterministic(const qint32 idum);

	/**
	* Setzt den Zufallsgenerator auf deterministisch.
	* D.h. es wird immer die gleiche Folge von Zufallswerten erzeugt.
	* 
	* Der seed wird auf m_defaultIdum gesetzt. Dieses kann mit
	* #setDefaultSeed() vom Benutzer gesetzt werden.
	*/
	static void setDeterministic();


	/**
	* Diese Funktion setzt das Standard-Idum, das beim Aufruf von #setDeterministic()
	* ohne Argumente gesetzt wird. 
	* Wenn eine Simulation einen deterministischen RNG verwendet, ruft sie jedesmal
	* o.g. Funktion auf, so dass über #setDefaultSeed() der Seed für deterministische
	* Simulationen gesetzt werden kann.
	*
	* Der Zustand des RNG bleibt von dieser Funktion unbeeinflusst. Um den übergebenen
	* default-seed zu aktivieren, muss also noch die Funktion #setDeterministic()
	* aufgerufen werden!
	*/
	static void setDefaultSeed(const qint32 idum);
	static qint32 getDefaultSeed();


	/**
	* Setzt den Zufallsgenerator auf statistisch.
	* D.h. die Folge von Zufallswerten hängt von der Systemzeit ab.
	* (Default Verhalten)
	*/
	static void				setRandom();

	/**
	* Ruft abhängig von den übergebenen SimulationParameters
	* setDeterministic() oder setRandom() auf.
	*/
	static void				initFromSimulationParameters(bool isDeterministic,bool isTransientCalc);
};

};
#endif 
