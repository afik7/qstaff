#include "qperson.h"

QPerson::QPerson(QObject *parent):
    QObject(parent)
{

  id_person=0;
  name="xx";
  surname="xx";
  patronomic="xx";
  gender="Ч";
  vnz="";
  worker="";
  pention="";
  DOB=QDate::fromString("14-05-1999", "d-MM-yyyy");
  QDate current= QDate::currentDate();
  age=getAge(DOB,current);
}
QPerson::QPerson(int _id, QString _surname,QString _name,QString _patronomic,
        QDate _DOB, QString _gender, QString _vnz, QString _position,
        QString _pention, QString _worker)
{
    id_person=_id;
    name=_name;
    surname=_surname;
    patronomic=_patronomic;
    gender=_gender;
    vnz=_vnz;
    worker=_worker;
    pention=_pention;
    DOB=_DOB;
    QDate current= QDate::currentDate();
    age=getAge(DOB,current);


}
float QPerson::getAge( QDate birth, QDate current)
{

    const int MTHS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


       int yy, yyy, mm, mmm, dd, ddd;

     yy=birth.year();
      mm=birth.month();
       dd=birth.day();


      yyy=current.year();

     mmm=current.month();
        ddd=current.day();

       int y = yyy - yy;
       int d = y*365 + y/4;
       int m1 = 0;
       int m2 = 0;
       for(int i=0; i < mmm; i++) {
          m1 += MTHS[i];
          if(yyy % 4 == 0 && mmm > 2) {
             m1 += 1;
          }
       }
       for(int i=0; i < mm; i++) {
          m2 += MTHS[i];
          if(yy % 4 == 0 && mm > 2) {
             m2 += 1;
          }
       }
       int m = m1 - m2;
       d += m;
       d += ddd - dd;
/*
       if(d < 0) {
          qDebug() << "ERROR: current date is before birth date, please try again.";
         return 0.;
       }
*/
       y = d / 365;
       d = d % 365;
       d -= y/4;
       int i = 0;
       m = 0;
       while(d > MTHS[i]) {
          d -= MTHS[i];
          m++;
          i++;
       }
       //cout << endl << "You are " << y << " years,  " << m << " months, and " << d << " days old." << endl;
     //  qDebug()<<QString::number(y)<<QString::number(m);
       float age=m/100.+y;

       return age;


}
void QPerson::setName(QString _name){name=_name;}
void QPerson::setSurname(QString _surname){surname=_surname;}
void QPerson::setPatronomic(QString _patronomic){patronomic=_patronomic;}
void QPerson::setId(int _id){id_person=_id;}
void QPerson::setDOB(QDate _DOB){DOB=_DOB;}
void QPerson::setPosition(QString _position){position=_position;}
void QPerson::setPention(QString _pention){pention=_pention;}
void QPerson::setGender(QString _gender){gender=_gender;}
void QPerson::setVnz(QString _vnz){vnz=_vnz;}
void QPerson::setWorker(QString _worker){worker=_worker;}
 void QPerson::setAge(float _age){age=_age;}
