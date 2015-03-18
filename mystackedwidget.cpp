#include "mystackedwidget.h"
#include <QDebug>
#include <QVBoxLayout>


MyStackedWidget::MyStackedWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      curr_index(0)
    {

    }

int MyStackedWidget::count()
    { return widgets.count(); }

void MyStackedWidget::addWidget(QWidget * w)
    {
        widgets.append(w);

        layout->addWidget(w);
        showCurrentWidget();
    }

QWidget * MyStackedWidget::currentWidget()
    { return widgets.at(curr_index); }

void MyStackedWidget::setCurrentIndex(int i)
{
    curr_index = i;
    showCurrentWidget();
}

void MyStackedWidget::showCurrentWidget()
    {
        if (widgets.count() > 0)
        {
            foreach (QWidget * widget, widgets)
                widget->hide();

            widgets.at(curr_index)->show();
            updateGeometry();
        }
    }

QSize MyStackedWidget::sizeHint()
    {
        if (auto_resize
         && count() > 0)
            return currentWidget()->minimumSize();
        else
            return QWidget::sizeHint();
    }
