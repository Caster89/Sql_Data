#pragma once
#ifndef METATYPEDECLARATION_H
#define METATYPEDECLARATION_H
#include <myfield.h>
#include <QSqlRecord>
#include <QVector>
Q_DECLARE_METATYPE(QSqlRecord);
Q_DECLARE_METATYPE(QVector<QVector<QString> >);
Q_DECLARE_METATYPE(QVector<QSqlRecord>);


#endif // METATYPEDECLARATION_H
