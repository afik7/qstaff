#ifndef VIEWIMPORTFORM_H
#define VIEWIMPORTFORM_H
#include "viewimportform.h"
//#include "staffdialog.h"
#include <QDialog>
#include <QLineEdit>
#include <QAxObject>
namespace Ui {
    class ViewImportForm;
}

class ViewImportForm : public QDialog
{
    Q_OBJECT

public:
    explicit ViewImportForm(QWidget *parent = 0);
    ~ViewImportForm();

   // StaffDialog* staffdi;
void setForm(bool sameCol);
bool checkCols();
QAxObject * excelCheckWorkbook(int numPage);
QString fileOpen();

private slots:

void importToBase();
private:
    Ui::ViewImportForm *ui;
    int cols;
    int rows;
    QStringList* prange ;
     QWidget* wd ;
    int stringToNum(QString cp);
    QString stringToStr(QString cp);
    QDate cleanDOB(QString dob);

};

#endif
