#pragma once
#include <string>
#include <QMutex>
using namespace std;

class VisualizerData
{
public:
	VisualizerData(void);
	VisualizerData(const VisualizerData& c);
	virtual ~VisualizerData(void);

	//! Welche Linien-Arten kennen wir? Durchgezogen, Kreuze, gepunktet, etc.
	enum Style{LINE, CROSS, DASHED_LINE, DASHED_DOTTED_LINE, SQUARE};

	//! Setze den Namen, den dieser Dateneintrag im Visualizer (In der GUI "Objects"-Liste)haben soll.
	void setName(string);

	//! Lese den Namen des Dateneintrags aus.
	string getName();

	//! Setze die Beschreibung des Dateneintrags
	void setDescription(string);

	//! Lese die Beschreibung aus
	string getDescription();

	//! Setze die Einheit (der Y-Achse) für den Dateneintrag
	void setUnit(string);

	//! Lese die Einheit (der Y-Achse) für den Dateneintrag aus
	string getUnit();

	//! Setze die RGB-Farbe, den die Datenlinie (nicht das Koordinatensystem!) hat
	void setColor(float red, float green, float blue);

	//! Dito, aber über Integer Werte
	void setColorInt(unsigned int red, unsigned int green, unsigned int blue);

	//! Gib mir den Rot-Wert zurück!
	float getColorRed();
	//! Gib mir den Grün-Wert zurück!
	float getColorGreen();
	//! Gib mir den Blau-Wert zurück!
	float getColorBlue();

	//! Setzt den Linien-Style der Datenlinie (z.B. Linie, Kreuze, gepunktet, etc.)
	void setStyle(Style);

	//! Lese den Linien-Style der Datenlinie aus
	Style getStyle();

	//! Gibt zurück, ob die Linie standardmäßig sichtbar ist.
	bool isVisible();

	//! Setzt, ob die Linie standardmäßig sichtbar ist.
	void setVisible(bool);

	//! ???
	bool isTransparent();

	//! ???
	void setTransparent(bool);

protected:
	QMutex m_mutex;

private:
	string m_name;
	string m_unit;
	string m_description;
	float m_color[3];
	Style m_style;
	bool m_visible;
	bool m_transparent;
};

