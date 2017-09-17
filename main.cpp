#include "staffdialog.h"
#include "ui_staffdialog.h"
#include <QtGui>
#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{

     //  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
       QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    Q_INIT_RESOURCE(Qstaff);
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
            QMessageBox::critical(0, QObject::tr("Сістемий трей"),
                                  QObject::tr("Не можу знайти системний трей на цій ОС "));
            return 1;
        }
        QApplication::setQuitOnLastWindowClosed(false);

    StaffDialog* w=new StaffDialog;

 w->setWhatsThis("<img src=\":/images/profesion.png\">"
 "&nbsp;База даних відділу кадрів включає  "
 "3 основні таблиці:"
 "<ul>"
 "<li><b>Персонал </b> Зберігає данні по працюючим"
 "<li><b>Пенсії </b> Каталог пенсійного віку "

 "<li><b>Посади </b> Список посад коледджу "

 "</ul>");


       QStringList args = a.arguments();
       if (args.count() == 2 && args[1]=="-m")
       {

         w->hide();
       }

else
       {

 w->setUnCheck() ;
  w->show();
       }
    return a.exec();
}
