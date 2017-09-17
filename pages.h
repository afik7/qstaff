#ifndef PAGES_H
#define PAGES_H





#include <QWidget>

class ConfigurationPage : public QWidget
{
public:
/*!*/

    ConfigurationPage(QWidget *parent = 0);

private slots:
    void on_toolButtonOpen_clicked();

private:

};

class QueryPage : public QWidget
{
public:
    QueryPage(QWidget *parent = 0);
private:

};

class PersonalPage : public QWidget
{
public:
    PersonalPage(QWidget *parent = 0);
private:

};



#endif // PAGES_H
