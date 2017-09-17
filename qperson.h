#ifndef QPERSON_H
#define QPERSON_H
#include <QDate>
#include <QObject>

class QPerson : public QObject
{
    Q_OBJECT
public:
    explicit QPerson(QObject *parent = 0);
    QPerson(int _id, QString _surname,QString _name,QString _patronomic,
            QDate _DOB, QString _gender, QString _vnz, QString _position,
            QString _pention, QString _worker);
    float getAge( QDate birth, QDate current);//расчет возраста
    void setName(QString _name);
    void setSurname(QString _surname);
    void setPatronomic(QString _patronomic);
    void setId(int _id);
    void setDOB(QDate _DOB);
    void setPosition(QString _position);
    void setPention(QString _pention);
    void setGender(QString _gender);
    void setVnz(QString _vnz);
    void setWorker(QString _worker);
    void setAge(float _age);

private:
    int id_person;
    QString name;
    QString surname;
    QString patronomic;
    QDate DOB;
    QString gender;//Ч або Ж
    QString vnz;//1-2,  3-4
    QString position;
    QString pention;//ПВ - за віком ПІ - за інвалідність ПІ3 - за інвалідність 3 групи
    QString worker;//P
    float age;// точність до місяця
signals:

public slots:
};

#endif // QPERSON_H
