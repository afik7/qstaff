#include "report.h"
//#include "ui_staffdialog.h"
#include <QSqlQuery>
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QAxObject>
#include<QFileDialog>
#include<QMessageBox>
#include <QSqlRecord>

Report::Report(QObject *parent) :
    QObject(parent)
{
    cols=9;
    strl<<"№"<<"Прізвище, і'мя, по батькові"<<"Дата народження"<<"Стать"<<"Вік"<<"Посада"<<"Робітники"<<"Рівень ВНЗ"<<"Пенсія";
    ppers=new QPerson;
    personList.clear();
    model = new QSqlQueryModel;
excelUtil();
    //setPersonList();
}
void Report::setPersonList(QString dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(dbName);
    if(!db.isOpen())
        db.open();
    QSqlQuery query;
QString ssql="SELECT personal.id_person, personal.surname, personal.name, personal.patronomic, personal.DOB, gender.type_gender AS gender, (CASE when(vnz.type_vnz ='-') then ' ' else vnz.type_vnz end) AS vnz, position.name_position AS position, cast(CASE when(position.worker = 1) then 'Р' else ' ' end as char) AS worker,        (CASE when(pention.type_pention = 'немає') then ' ' else pention.type_pention end) AS pention FROM personal         INNER JOIN pention ON (personal.id_pention = pention.id_pention) INNER JOIN gender ON (personal.id_gender = gender.id_gender) INNER JOIN position ON (personal.id_position = position.id_position) INNER JOIN vnz ON (personal.id_vnz = vnz.id_vnz)";
//id_person(0),surname(1),name(2),patronomic(3),DOB(4),gender(5),vnz(6),position(7), worker(8),pention(9)

query.exec(ssql );
QSqlRecord record = query.record();
int fieldcount = record.count();
qDebug()<<QString::number(fieldcount)<<"IIIIIIIIIIIIII";

while (query.next())
{

ppers->setId(query.value(0).toInt());
ppers->setSurname(query.value(1).toString());
ppers->setName(query.value(2).toString());
ppers->setPatronomic(query.value(3).toString());
ppers->setDOB(query.value(4).toDate());
ppers->setGender(query.value(5).toString());
ppers->setVnz(query.value(6).toString());
ppers->setPosition(query.value(7).toString());
ppers->setWorker(query.value(8).toString());
ppers->setPention(query.value(9).toString());
ppers->setAge(ppers->getAge(query.value(4).toDate(),QDate::currentDate()));
//personList.push_back(
personList.append(ppers);

}
}
QSqlQueryModel * Report::CreateReportModel()
{

QString ssql="SELECT personal.id_person, personal.surname||' '|| personal.name||' '||personal.patronomic AS SNP, personal.DOB, gender.type_gender AS gender,  cast(CASE when(strftime('%d', 'now') < strftime('%d', DOB)) then(((strftime('%Y', 'now') - strftime('%Y', DOB)) * 12 + (strftime('%m', 'now') - strftime('%m', DOB)) -1) %12) / 100.0 + ((strftime('%Y', 'now') - strftime('%Y', DOB)) * 12 + strftime('%m', 'now') - strftime('%m', DOB) -1) / 12 else((strftime('%Y', 'now') - strftime('%Y', DOB)) * 12 + strftime('%m', 'now') - strftime('%m', DOB)) %12/ 100.0 + ((strftime('%Y', 'now') - strftime('%Y', DOB)) * 12 + strftime('%m', 'now') - strftime('%m', DOB)) / 12 end as float) AS age, position.name_position AS position, cast(CASE when(position.worker = 1) then 'Р' else ' ' end as char) AS worker, (CASE when(vnz.type_vnz ='-') then ' ' else vnz.type_vnz end) AS vnz, (CASE when(pention.type_pention = 'немає') then ' ' else pention.type_pention end) AS pention FROM personal INNER JOIN pention ON (personal.id_pention = pention.id_pention) INNER JOIN gender ON (personal.id_gender = gender.id_gender) INNER JOIN position ON (personal.id_position = position.id_position) INNER JOIN vnz ON (personal.id_vnz = vnz.id_vnz)";
//id_person(0),SNP(1),DOB(2),gender(3),age(4),position(5), worker(6),vnz(7),pention(8)
    model->setQuery(ssql);
    int i=0;
    while(i<strl.count())
    {
        QString g=QString(strl.at(i));
    model->setHeaderData(i, Qt::Horizontal,tr( g.toUtf8().constData() ));
    i++;
    //model->setHeaderData(1, Qt::Horizontal, tr("Salary"));
}
return model;

}
void Report::excelUtil()
{
  QFile file ("excel.html");
QTextStream out(&file);
file.open(QIODevice::WriteOnly|QIODevice::Truncate);
QAxObject* excel= new QAxObject("Excel.Application");
QString d=excel->generateDocumentation();
out<<d;
file.close();
}
QString Report::choosePath()
{
    QString path = QFileDialog::getExistingDirectory(0,
                              tr("Вибери шлях щоб збергти таблицю"),
                                      "", QFileDialog::ShowDirsOnly);
        if (path == "" )
        {
            QMessageBox mbox;
            mbox.setText(tr("Не вибрано шлях для збереження таблиці. Відміна збереження"));
            mbox.exec();

        }

        return path;
}
void Report::createExcel()
{
    QString path=choosePath();
    if(!path.isEmpty())
    {
  QAxObject* excel= new QAxObject("Excel.Application",this);
 //excel->dynamicCall("SetVisible(bool)",true);
  QAxObject* workbooks=excel->querySubObject("Workbooks");
  QAxObject* workbook=workbooks->querySubObject("Add()");
  QAxObject* sheets=workbook->querySubObject("Sheets");
  QAxObject* sheet=sheets->querySubObject("Item(1)");
  sheet->dynamicCall("Select()");
  sheet->setProperty("SetStandardFontSize",9.);

  QAxObject *range=sheet->querySubObject("Range(const QVariant&)",QVariant(QString("A1:J200")));
  range->dynamicCall("Clear()");
  QAxObject* cell=sheet->querySubObject("Cells(int,int)",1,4);
  //cell->dynamicCall("Select()");
  cell->setProperty("Value",QVariant(tr("Пенсіонери")));
 // QAxObject *range12 = sheet1->querySubObject( "Range(const QString&, const QString&)");
 // range->setProperty("NumberFormat","@");// - устанавливает текстовый формат для диапазона ячеек
QAxObject *range2=sheet->querySubObject("Range(const QVariant&)",QVariant(QString("E1:E200")));
  range2->setProperty("NumberFormat","@");
  QAxObject *range3=sheet->querySubObject("Range(const QVariant&)",QVariant(QString("H1:H200")));
    range3->setProperty("NumberFormat","@");
  QString s;
  for(int r=0;r<model->rowCount();++r)
  {
      s=model->headerData(r,Qt::Vertical,Qt::DisplayRole).toString();
      if(s.size()>0)
      {
          QAxObject* cell=sheet->querySubObject("cells(int,int)",r+4,1);
          QAxObject* font=cell->querySubObject("Font");
          font->setProperty("Size",9);
          font->setProperty("Bold",true);
          cell->setProperty("HorizontalAlignment",-4152);
          cell->setProperty("Value",s);


      }
  }
  int cw[]={4,30,14,5,6,24,10,10,10};
  for(int c=0;c<model->columnCount();++c)
  {
      //cell=sheet->querySubObject("cells(int,int)",3,c+2);
      cell=sheet->querySubObject("cells(int,int)",3,c+1);
      s=model->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
       QAxObject* font=cell->querySubObject("Font");
      font->setProperty("Size",9);
      font->setProperty("Bold",true);
      cell->setProperty("HorizontalAlignment",-4108);
      cell->setProperty("VerticalAlignment",-4108);
      cell->setProperty("Value",s);
   //   QAxObject* row=sheet->querySubObject("Columns(int)",c+2);
      QAxObject* row=sheet->querySubObject("Columns(int)",c+1);
      row->setProperty("ColumnWidth",cw[c]);
      for(int r=0;r<model->rowCount();++r)
      {
          if(c==0)
              continue;
          else
          {
          s=model->data(model->index(r,c),Qt::DisplayRole).toString();
          if(c==0)
              continue;
          if(s.size()>0)
          {
              //QAxObject* cell=sheet->querySubObject("cells(int,int)",r+4,c+2);
              QAxObject* cell=sheet->querySubObject("cells(int,int)",r+4,c+1);
              cell->setProperty("Value",s);

          }
          }

      }
  }

  QAxObject* cell1=sheet->querySubObject("cells(int,int)",3,1);
//  QAxObject* cell2=sheet->querySubObject("cells(int,int)",model->rowCount()+3,model->columnCount()+1);
  QAxObject* cell2=sheet->querySubObject("cells(int,int)",model->rowCount()+3,model->columnCount());
  QAxObject* range1=sheet->querySubObject("Range(const QVariant&, const QVariant&)",QVariant(cell1->dynamicCall("Address()")),QVariant(cell2->dynamicCall("Address()")));
  QAxObject* borders=range1->querySubObject("Borders");
  borders->setProperty("LineStyle",1);


  QString cur=QDateTime::currentDateTime().toString("dd_MM_yyyy-ss");

    path = path + QString("\\Pensioners_ ") +  cur + ".xls" ;

//path+="\\pensioners.xls";
  QList<QVariant> lstParam;
  lstParam.append(path);
  lstParam.append(-4143);
  lstParam.append("");
  lstParam.append("");
  lstParam.append(false);
  lstParam.append(false);
  lstParam.append(1);
  lstParam.append(2);
  lstParam.append(false);
  lstParam.append(false);
  lstParam.append(false);
  lstParam.append(false);
qDebug()<<path;
  workbook->dynamicCall("SaveAs(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)", lstParam);



 // workbook->dynamicCall("Close(Boolean)", false);
  workbook->dynamicCall("Close()");
 //excel->dynamicCall("Quit()");
      excel->dynamicCall("Quit(void)");
      delete sheet;
      delete sheets;
      delete workbook;
       delete workbooks;
      delete excel;
}
}
