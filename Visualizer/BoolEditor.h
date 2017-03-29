#pragma once
#include <QComboBox>

class BoolEditor : public QComboBox
{
	Q_OBJECT
		Q_PROPERTY(bool value READ value WRITE setValue USER true)

public:
	BoolEditor(QWidget *widget = 0);

public:
	bool value() const;
	void setValue(bool b);

};
