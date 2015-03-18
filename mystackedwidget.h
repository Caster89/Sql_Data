#ifndef MYSTACKEDWIDGET_H
#define MYSTACKEDWIDGET_H

#include <QList>
#include <QWidget>
#include <QVBoxLayout>

class MyStackedWidget : public QWidget
    {
        Q_OBJECT

        public:
            MyStackedWidget(QWidget * = 0, Qt::WindowFlags = 0);

            void addWidget(QWidget *);
            int count();
            QWidget * currentWidget();

            void setAutoResize(bool yes)
                { auto_resize = yes; }

            QSize sizeHint();

        protected:
            void showCurrentWidget();

        private:
            bool auto_resize;
            int curr_index;
            QList<QWidget *> widgets;
            QVBoxLayout * layout = new QVBoxLayout(this);

        public slots:
            void setCurrentIndex(int);
    };

#endif // MYSTACKEDWIDGET_H
