
#include "ui_ViewImportForm.h"
#include "viewimportform.h"
#include "staffdialog.h"
#include <QDebug>
#include<QLabel>
#include<QLineEdit>
#include<QMessageBox>
#include<QDir>
#include<QFileDialog>
#include<QSqlError>
#include<QSqlQuery>
ViewImportForm::ViewImportForm(QWidget *parent):QDialog(parent),ui(new Ui::ViewImportForm)
{
    ui->setupUi(this);
    cols=9;
     prange= new QStringList;
      wd= new QLineEdit;
 connect(ui->toolButtonImportBase, SIGNAL(clicked()), this, SLOT(importToBase()));
 }

ViewImportForm::~ViewImportForm()
{
    delete ui;

}
void ViewImportForm::setForm(bool sameCol)
{
  cols=9;
     QStringList stlist;
 stlist<<"Назва колонки<br>                "<<"Клітинка з назвою<br> колонки у форматі B6 "<<"Прізвище"<<"І'мя"<<"По батькові"<<"Посада"<<"Дата народження"<<"Стать"<<"Робітники"<<"Пенсія*"<<"Рівень ВНЗ";
    if(sameCol)
    {cols=7;
       stlist.clear();
       stlist<<"Назва колонки<br>                "<<"Клітинка з назвою<br> колонки у форматі B6 "<<"Прізвище, і'мя, по батькові"<<"Посада"<<"Дата народження"<<"Стать"<<"Робітники"<<"Пенсія*"<<"Рівень ВНЗ";

    }


 while(ui->gridLayoutImport->count() > 0) {
                QWidget* widget = ui->gridLayoutImport->itemAt(0)->widget();
                ui->gridLayoutImport->removeWidget(widget);
                delete widget;
        }
ui->gridLayoutImport->update();
    QLabel* pcaption1=new QLabel();
    pcaption1->setText(stlist.at(0));
ui->gridLayoutImport->addWidget(pcaption1,0,0);
QLabel* pcaption2=new QLabel();
pcaption2->setText(stlist.at(1));
ui->gridLayoutImport->addWidget(pcaption2,0,1);
 QList<QLabel*> plab;//=new QList;
 QList <QLineEdit*> plined;

    for(int i=0;i<cols;i++)
    {

        plab<<new QLabel();
        plab.at(i)->update();
        plab.at(i)->setText(stlist.at(i+2));

ui->gridLayoutImport->addWidget(plab.at(i),i+1,0);
plined<<new QLineEdit();
plined.at(i)->setFixedHeight(30);
plined.at(i)->setFixedWidth(100);
ui->gridLayoutImport->addWidget(plined.at(i),i+1,1);
    }
   plined.at(0)->setText("B6");
   if(cols==7)
   {
      plined.at(1)->setText("C6");
      plined.at(2)->setText("D6");
      plined.at(3)->setText("E6");
      plined.at(4)->setText("G6");
      plined.at(5)->setText("H6");
      plined.at(6)->setText("I6");
   }
   else
   {
       plined.at(1)->setText("C6");
       plined.at(2)->setText("D6");
       plined.at(3)->setText("E6");
        plined.at(4)->setText("F6");
         plined.at(5)->setText("G6");

       plined.at(6)->setText("I6");
       plined.at(7)->setText("J6");
       plined.at(8)->setText("K6");
   }
}
void ViewImportForm::importToBase()
{
if(checkCols())
{
    QAxObject* worksheet;

int numPage=ui->spinBoxNumPage->value();

if(worksheet=excelCheckWorkbook(numPage))
{
   // QString cp = "Range(B6)";

    int colRows=ui->spinBoxNumRows->value();
QStringList listrange;
for(int i=0;i<cols;i++ )
{
   listrange.append(stringToStr(prange->at(i))+QString::number(stringToNum(prange->at(i))+1));
   listrange.append(stringToStr(prange->at(i))+QString::number(stringToNum(prange->at(i))+colRows));
 }

QAxObject *range;
int colColumn=9;
int k=0, start=0;
QVector<QVector<QVariant> > all(colColumn);//=QVector<QVector>(cols);
 if(cols==7)
 {

    range=worksheet->querySubObject( "Range(const QString&, const QString&)", listrange.at(0), listrange.at(1));
    for(int i=0;i<3;i++)
    {  all[i].resize(colRows);

        for (int row=0; row < colRows; row++)
        {
        QString dataColumn=range->dynamicCall("Value()").toList().at(row).toList().at(0).toString();
        QStringList st=dataColumn.split(" ");
        all[i][row]=st.at(i).trimmed();

     }
    }
        k=2;
        start=3;
 }
for(int column=start; column < colColumn; column++)
{

  //  range=NULL;
 range=worksheet->querySubObject( "Range(const QString&, const QString&)", listrange.at(k), listrange.at(k+1));
    qDebug()<<listrange.at(k)<<listrange.at(k+1) ;

     k+=2;
     // QVariantList dataColumn;
     all[column].resize(colRows);
     for (int row=0; row < colRows; row++)
     {
      QVariant dataColumn=range->dynamicCall("Value()").toList().at(row).toList().at(0);
      all[column][row]=dataColumn;
     // qDebug()<<dataColumn.toString()<<row<<column ;

     }
}

for(int column=0; column < colColumn; column++)
{
    for (int row=0; row < colRows; row++)
    {
     qDebug()<<all[column][row].toString()<<column<<row ;

    }
}
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setHostName("localhost");
db.setDatabaseName("staff.db");

if(!db.isOpen())
   { db.open();
if (!db.isOpen()) {
    QMessageBox::critical(this, QString::fromLocal8Bit("Помилка зєднання з базою"), db.lastError().text());
    return  ;
}
}
//adding new data to position table
QSqlQuery query,query1;
QString position,st ;

int worker;
QDate DOB;
int id_gender,id_vnz,id_position,id_pention;
//position -column3, worker column6 for colColumn

 query.exec("DELETE from personal");
 query.finish();
 query.exec("VACUUM");
for(int row=0;row<colRows;row++)
{ position=all[3][row].toString().trimmed();
  if(all[6][row].toString().trimmed().toUpper()=="P"||all[6][row].toString().trimmed().toUpper()=="Р")
      worker=1;
 else worker=0;
st="SELECT id_position FROM position where name_position= '"+position+"'";
 query.exec(st);
if(!query.next())
{
       //insert name , worker
       st="INSERT INTO position (name_position, worker) VALUES ( '"+position+"', "+QString::number(worker)+")";
       query1.exec(st);
   qDebug()<<QString::number(query1.numRowsAffected())<<st;
   query1.finish();
 }
query.finish();

 st="Select id_position from position where name_position= '"+position+"' and worker!= "+QString::number(worker);
 query.exec(st);
 if(query.next())
 {
     //update worker
   st= "UPDATE  position SET worker="+QString::number(worker)+" WHERE name_position= '"+position+"'";
      query1.exec(st);
   qDebug()<<QString::number(query1.numRowsAffected())<<st;
   query1.finish();

 }

query.finish();
}
 query1.prepare("INSERT INTO personal (surname, name,patronomic,DOB,id_gender,id_vnz,id_position,id_pention) VALUES ( :surname,:name,:patronomic,:DOB,:id_gender,:id_vnz,:id_position,:id_pention )");
for(int row=0;row<colRows;row++)
{
//        st=;
  // query1.prepare(st);
   query1.bindValue(":surname",all[0][row].toString());

    query1.bindValue(":name",all[1][row].toString());

   query1.bindValue(":patronomic",all[2][row].toString());
DOB=all[4][row].toDate();
if(DOB.isNull())
{
    DOB=cleanDOB(all[4][row].toString());
}
    query1.bindValue(":DOB",DOB);
 if(all[5][row].toString().trimmed().toUpper()=="Ж")
     id_gender=2;
 else id_gender=1;
 query1.bindValue(":id_gender",id_gender);
 if(all[8][row].toString().trimmed()=="1-2")
     id_vnz=1;
 else if(all[8][row].toString().trimmed()=="3-4")
         id_vnz=2;
 else   id_vnz=3;
 query1.bindValue(":id_vnz",id_vnz);

 position=all[3][row].toString().trimmed();
    st="SELECT id_position FROM position WHERE name_position= '"+position+"'";
    query.exec(st);
 if(  query.next())
  {

         qDebug()<<query.value(0).toString()<<st<<"****";
        id_position=query.value(0).toInt();
 }
 query1.bindValue(":id_position",id_position);
 if(all[7][row].toString().trimmed().toUpper()=="П")
     id_pention=2;
 else
     id_pention=1;
  query1.bindValue(":id_pention",id_pention);
  qDebug()<<":surname,:name,:patronomic,:DOB,:id_gender,:id_vnz,:id_position,:id_pention"<<all[0][row].toString()<<all[1][row].toString()<<all[2][row].toString()<<all[4][row].toDate()<<id_gender<<"  "<<id_vnz<<"  "<<id_position<<"  "<<id_pention;
   query1.exec();

   if(query1.numRowsAffected()==-1)
   {
       QMessageBox::critical(this, QString::fromLocal8Bit("Запит INSERT до таблиці Персонал верув 0 рядків"), "Запит до БД");
       return;
   }
    query1.finish();
}
st="SELECT count(*) FROM personal";
        query.exec(st);
if(query.next())
QMessageBox::information(0, QString::fromLocal8Bit("Запит INSERT до таблиці Персонал"), "Додано "+query.value(0).toString()+" записів");
/*
QList<QVariantList> data;	//Data list from excel, each QVariantList is worksheet row

for (int row=0; row <= colRows; row++)
{
    QVariantList dataRow;
    k=0;
    for (int column=0; column <= cols; column++)
    {
        //QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row, column );
       QAxObject* range=worksheet->querySubObject( "Range(const QString&, const QString&)", listrange.at(k), listrange.at(k+1));
        k+=2;
        //QVariant value = cell->dynamicCall( "Value()" );
        QVariant value =range->dynamicCall("Value()").toList().at(column).toList().at(

                    );
        qDebug()<<value.toString();
       // if (!value.toString().isEmpty() && isEmpty)
        //    isEmpty = false;
        dataRow.append(value);
    }
  //  if (isEmpty) //criteria to get out of cycle
      //  break;
    data.append(dataRow);
}
*/


}
}
}
QDate ViewImportForm::cleanDOB(QString dob)
{

    QStringList r, sl=dob.split('.');
    QDate DOB;
    int letters,f,l=2;
    for(int i=0;i<sl.size();i++)
    {
    letters =sl.at(i).size();
    if(i==2)
        l=4;
    f=letters-l;
    r.append(sl.at(i).mid(f,l));

    }
    DOB.setDate(r.at(2).toInt(),r.at(1).toInt(),r.at(0).toInt());

return DOB;
}
QString ViewImportForm::stringToStr(QString cp)
{
    QString snum;
    for(int i=0;i<cp.size();i++)
        if(cp.at(i).isLetter())
        {
            snum.append(cp.at(i));
           // if(cp.at(i+1).)
          }
    else break;
                //int k= cp.split(" ")[0].toInt();
    return snum ;
}
int ViewImportForm::stringToNum(QString cp)
{
    QString snum;
    for(int i=0;i<cp.size();i++)
        if(cp.at(i).isNumber())
        {
            snum.append(cp.at(i));
           // if(cp.at(i+1).)
          }
                //int k= cp.split(" ")[0].toInt();
    return snum.toInt();
}
bool ViewImportForm::checkCols()
{
    bool full=true;

   // QStringList* prange= new QStringList;
   //  QWidget* wd= new QLineEdit;
for(int i=0;i<cols;i++)
{

         wd=  ui->gridLayoutImport->itemAtPosition(i+1,1)->widget();
    QLineEdit* le= qobject_cast<QLineEdit *>(wd);

   if(le->text().isEmpty())
        full=false;
    else
        *prange<<le->text();
}
if (!full)
{
    QMessageBox::critical(0, QString::fromLocal8Bit("Не всі поля заповнені"), "Назад");

}
return full;
}
QString ViewImportForm::fileOpen()
{
    QString dir=QDir::currentPath();
 QString fn = QFileDialog::getOpenFileName(0, tr("Відкрити Excel 2003-97 файл для імпорту"),dir, tr("xls файли (*.xls)"));

   return fn;
}

  QAxObject * ViewImportForm::excelCheckWorkbook(int numPage)
 {
//bool result=false;
      QAxObject* worksheet= new QAxObject;
      worksheet=NULL;
     QString path=fileOpen();

    if(!path.isEmpty())
    {
        QAxObject *excel=new QAxObject("Excel.Application");
        excel->setProperty("Visible",false);
        QAxObject *workbooks=excel->querySubObject("WorkBooks");
        workbooks->dynamicCall("Open (const QString&)",path); //QString("C:\\Book.xls"));
        QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
      QAxObject *worksheets=workbook->querySubObject("WorkSheets");
int intCount = worksheets->property("Count").toInt();
if(intCount<numPage)
{
    QString st="Немає листа з номером "+QString::number(numPage);
     QMessageBox::critical(0, QString::fromLocal8Bit("Введить дані знову" ), st);
     workbook->dynamicCall("Close()");
     excel->dynamicCall("Quit()");
   delete   worksheets ;
     delete workbook;
     delete workbooks;
     delete excel;

    // return false;
}
else
   // result=true;

    worksheet= workbook->querySubObject("Worksheets(int)", numPage);
    }
    return worksheet;
 }
