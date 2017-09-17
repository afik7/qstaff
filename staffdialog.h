
#ifndef STAFFDIALOG_H
#define STAFFDIALOG_H
#include "report.h"
#include <QSystemTrayIcon>
#include <QDialog>
#include <Qmenu>
#include <QPointer>
#include <QDate>
#include <QVector>
QT_BEGIN_NAMESPACE

struct Catalog{
  float age;
  QDate DOBbegin;
  QDate DOBend;
  int gender;
} ;
class QListWidgetItem;

QT_END_NAMESPACE

class Table;
class TableIOMapper;
class ViewCatalogForm;
class ViewImportForm;
typedef QPointer<Table> TablePtr;
typedef QPointer<TableIOMapper> TableMapperPtr;
namespace Ui {
class StaffDialog;

}

class StaffDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StaffDialog(QWidget *parent = 0);
    ~StaffDialog();
      void Utils(QString dbName);//Суммарные данные по пенсионерам на сегодняшний день
      void showReport();
     void backupMessage();
     void showimportForm();
    void setUnCheck();
      //float getAge( QDate birth, QDate current);//расчет возраста
private:
    Ui::StaffDialog *ui;

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();
 ///////////////////////

    QSystemTrayIcon *trayIcon;
QMenu *trayIconMenu;

QAction *minimizeAction;
QAction *maximizeAction;
QAction *restoreAction;
QAction *quitAction;

void createActions();


private slots:
void createTrayIcon();
void showMessage();
void iconActivated(QSystemTrayIcon::ActivationReason reason);




private:

  bool connectDB(const QString &dBName);
  bool createDB(const QString &dBName);
  bool closeDB( QSqlDatabase& db);

  void createModel();
  void deleteModel();
  void setupView();
  TablePtr position;
  TablePtr agepention;
  TablePtr compositions;
  TableMapperPtr positionMapper;
  TableMapperPtr agepentionMapper;
  TableMapperPtr compositionsMapper;
  ViewCatalogForm * positionForm;
  ViewCatalogForm * agepentionForm;
  ViewImportForm* importForm;
  QSqlDatabase db;
QString dbName;
  QString dbHost;
  Report* rep;
public slots:
  void newDB();
  void openDB();
  void showpositionForm();
  void showagepentionForm();
  void addRecord();
  void deleteRecord();
  void cancelChanges();
  void save();
void backup();
void import();
void saveToCSV();
void saveToXML();
void setModes();
};

#endif // STAFFDIALOG_H
