#include "BoolEditor.h"

BoolEditor::BoolEditor(QWidget *widget) : QComboBox(widget)
{
	insertItem(0, "true");
	setItemData(0, true, Qt::DecorationRole);
	insertItem(0, "false");
	setItemData(0, false, Qt::DecorationRole);
}

bool BoolEditor::value() const
{
    return itemData(currentIndex(), Qt::DecorationRole).value<bool>();
}

void BoolEditor::setValue(bool b)
{
	setCurrentIndex(findData(b, int(Qt::DecorationRole)));
}
