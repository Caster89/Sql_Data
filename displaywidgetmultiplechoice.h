#ifndef DISPLAYWIDGETMULTIPLECHOICE_H
#define DISPLAYWIDGETMULTIPLECHOICE_H
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QStringList>
#include "displaywidgetbase.h"


class DisplayWidgetMultipleChoice : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetMultipleChoice(MyField newField, bool editable=false, QWidget *parent=0);
    QVariant getValue();
    void setValue(QVariant newValue);
    ~DisplayWidgetMultipleChoice();
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

private:
    QComboBox *cmbValues = new QComboBox();
    QLabel *lblValue = new QLabel();
    QStringList *avlbChoices = new QStringList();

private slots:
    void selectionChanged(int newSelection);
};

#endif // DISPLAYWIDGETMULTIPLECHOICE_H
