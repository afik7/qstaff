#ifndef REPORT_H
#define REPORT_H

#include "qperson.h"
#include<QStringList>
#include<QList>
#include <QObject>
#include <QSqlQueryModel>
#include <QAxObject>

class Report : public QObject
{
    Q_OBJECT
    int cols;
    QStringList strl;
    QPerson* ppers;
    QList<QPerson *> personList;
public:
    explicit Report(QObject *parent = 0);
    QSqlQueryModel * CreateReportModel();

   void setPersonList(QString dbName);
   QSqlQueryModel *model ;
   void excelUtil();
QString  choosePath();

signals:

public slots:
    void createExcel();

};
#endif // REPORT_H
