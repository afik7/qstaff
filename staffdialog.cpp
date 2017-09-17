#include "staffdialog.h"
#include "ui_staffdialog.h"
#include "viewcatalogform.h"
#include "viewimportform.h"
#include "table.h"
#include "tableiomapper.h"
#include "dedelegate.h"
#include "qperson.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include<QDate>
#include<QStringList>
#include <QDebug>
#include <QStringListModel>
#include <QDateTime>
#include<QFile>
#include<QXmlStreamWriter>
#include<QSettings>
#include<QDesktopServices>
#include<QTextEdit>
#include<QWhatsThis>
StaffDialog::StaffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StaffDialog)
{
    ui->setupUi(this);


   ui->contentsWidget->setViewMode(QListView::IconMode);
    ui->contentsWidget->setIconSize(QSize(96, 84));
    ui->contentsWidget->setMovement(QListView::Static);
    ui->contentsWidget->setMaximumWidth(128);
    ui->contentsWidget->setSpacing(12);


    createIcons();
    ui->contentsWidget->setCurrentRow(0);



//////////////////
   positionForm = new ViewCatalogForm(this);
       positionForm->setWindowTitle(tr("Посади"));
       agepentionForm = new ViewCatalogForm(this);
       agepentionForm->setWindowTitle(tr("Каталог пенсій"));
    importForm=new ViewImportForm(this);
    importForm->setWindowTitle(tr("Опції імпорту"));
       ////////////////////////
   createActions();
createTrayIcon();
trayIcon->setIcon(QIcon(QString::fromUtf8(":/images/query.png")));
trayIcon->show();
connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
///////////////////////////////////////////////
db = QSqlDatabase::addDatabase("QSQLITE");
dbHost="localhost";
dbName="staff.db";
setWindowTitle(QString::fromLocal8Bit("Персонал - ") + dbName);
connectDB(dbName);
rep=new Report;


rep->setPersonList(dbName);
//rep.excelUtil();

showReport();
//rep.saveExcel();
ui->contentsWidget->setWindowTitle(tr("Штат Qstaff"));
//QString s = QDate::currentDate().toString();
QDate date= QDate::currentDate();
QString title=date.toString("dd.MM.yyyy");
ui->groupBoxPention->setTitle(QString("Пенсіонери на ")+title);

Utils(dbName);
                //  ui->groupBoxPention->setTitle(title);


createModel();
setupView();

         connect(ui->toolButtonSave, SIGNAL(clicked()), this, SLOT(save()));

         connect(ui->toolButtonOpen, SIGNAL(clicked()), this, SLOT(openDB()));

         connect(ui->toolButtonPosition, SIGNAL(clicked()), this, SLOT(showpositionForm()));

         connect(ui->toolButtonAgepention, SIGNAL(clicked()), this, SLOT(showagepentionForm()));

         connect(ui->toolButtonAdd, SIGNAL(clicked()), this, SLOT(addRecord()));

         connect(ui->toolButtonRemove, SIGNAL(clicked()), this, SLOT(deleteRecord()));

         connect(ui->toolButtonCancel, SIGNAL(clicked()), this, SLOT(cancelChanges()));

 connect(ui->toolButtonSaveExcel,SIGNAL(clicked()),rep,SLOT(createExcel()));
 connect(ui->toolButtonBackup,SIGNAL(clicked()),this,SLOT(backup()));
 connect(ui->toolButtonImport,SIGNAL(clicked()),this,SLOT(import()));
 connect(ui->toolButtonExportCSV,SIGNAL(clicked()),this,SLOT(saveToCSV()));
  connect(ui->toolButtonExportXML,SIGNAL(clicked()),this,SLOT(saveToXML()));
   connect(ui->toolButtonSaveModes,SIGNAL(clicked()),this,SLOT(setModes()));

}

StaffDialog::~StaffDialog()
{
    delete ui;
    deleteModel();
}

void StaffDialog::Utils(QString dbName)
{
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   // db.setHostName(dbHost);
    db.setDatabaseName(dbName);
    if(!db.isOpen())
        db.open();

    QVector<Catalog> vCatalog;//=new QVector();

QDate current= QDate::currentDate();
QDate c=current.addDays(10);

   // qDebug()<<QString::number(getAge(QDate::fromString("14-05-2004", "d-MM-yyyy"),c));

    int arN[4]={0};
     QString s;
     QStringList* strlistQueries= new   QStringList();
      QSqlQuery query_pentioner,query_catalog,query_total;
QStringList* today_pentioners= new QStringList();// для ФИО на сегодня
QStringList* today10_pentioners= new QStringList();// для ФИО на 10 дней вперед
     strlistQueries->append("Select count(*) FROM personal where id_pention=2");
// возможные пенсионеры на сегодня
     strlistQueries->append("Select  get_age3.surname as surname, get_age3.name as name, get_age3.patronomic as                             patronomic, get_age3.id_gender as id_gender, cast(get_age3.age as real) as age, DOB from get_age3,agepention_catalog where  strftime('%s', DOB) between strftime('%s', agepention_catalog.DOBbegin) and strftime('%s', agepention_catalog.DOBend) and id_pention!=2 and get_age3.age >=agepention_catalog.age and  agepention_catalog.id_gender=get_age3.id_gender order by id_gender");
//каталог пенсий
strlistQueries->append("Select * FROM agepention_catalog ");


query_pentioner.exec(strlistQueries->at(1) );
//QSqlRecord record_pentioner = query_pentioner.record();
// int fieldcount_pentioners = record_pentioner.count();
 query_catalog.exec(strlistQueries->at(2));
// QSqlRecord record_catalog = query_catalog.record();
//  int fieldcount_catalog = record_catalog.count();
//  QSqlRecord record_total=query_total.record();
  query_total.exec(strlistQueries->at(0));
  query_total.next();
   arN[0]=query_total.value(0).toInt();
// qDebug()<<QString::number(fieldcount_pentioners)<<"||"<<QString::number(fieldcount_catalog);
//surname (0),name(1), patronomic(2) , id_gender(3),age(4), DOB(5)
  Catalog cur;
while (query_catalog.next())
{
//vCatalog.append( query_catalog.value(1).toFloat()
cur.age=query_catalog.value(1).toFloat();
cur.DOBbegin=query_catalog.value(2).toDate();
cur.DOBend=query_catalog.value(3).toDate();
cur.gender=query_catalog.value(4).toInt();
vCatalog.push_back(cur); ;
}
int i=0;
QPerson pers;
 while ( query_pentioner.next() )
 {
     i=0;
     while(i<vCatalog.count())
     {
         cur=vCatalog.at(i);
qDebug()<<QString::number(query_pentioner.value(3).toInt());
         if(cur.gender==query_pentioner.value(3).toInt()&&
                 query_pentioner.value(5).toDate()>= cur.DOBbegin &&
                query_pentioner.value(5).toDate()<= cur.DOBend )
         {
             float age=pers.getAge(query_pentioner.value(5).toDate(),c);//****************
              qDebug()<<QString::number(age);
             if(query_pentioner.value(4).toFloat()>=cur.age)
             {
               s.clear();
                s=query_pentioner.value(0).toString()+" "+query_pentioner.value(1).toString()+" "+query_pentioner.value(2).toString();
                today_pentioners->append(s);
             arN[2]++;
             }
             else if(age>=cur.age)
             {

                 s.clear();
                 s=query_pentioner.value(0).toString()+" "+query_pentioner.value(1).toString()+" "+query_pentioner.value(2).toString();
                 today10_pentioners->append(s);
              arN[1]++;
             }

         }


         i++;
 }
}
qDebug()<<QString::number( arN[0])<<QString::number( arN[2])<<QString::number( arN[1]);
i=0;
while(i<today_pentioners->count())
{
qDebug()<<today_pentioners->at(i);
i++;
}
s.clear();
    QStringList* strlistPentioners= new   QStringList();
            strlistPentioners->append("Всього у базі "+s.setNum(arN[0])+" пенсіонерів за віком");

                  strlistPentioners->append("Стануть пенсіонерами за віком через 10 днів  "+s.setNum(arN[1])+" пенсіонерів");

                    strlistPentioners->append("На сьогоді треба додати   "+s.setNum(arN[2])+" пенсіонерів за віком");
                    ui->labelTotalAgePentioners->setText( strlistPentioners->at(0));
                    ui->labeAgePentioners10FromNow->setText( strlistPentioners->at(1));
                    QPalette palette = ui->labelAgePentionersNow->palette();

                     palette.setColor(ui->labelAgePentionersNow->foregroundRole(), Qt::red);
                     ui->labelAgePentionersNow->setPalette(palette);
                     ui->labelAgePentionersNow->setText( strlistPentioners->at(2));
//arN[2]=0;
if(arN[2]==0)
    ui->listPentionersNow->hide();
else{

   QStringListModel* listModel = new QStringListModel(*today_pentioners,NULL);//(*today_pentioners, NULL);//*
   ui->listPentionersNow->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->listPentionersNow->setModel(listModel);
    //ui->listPentionersNow->
}

}
void StaffDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(ui->contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Конфігурація"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(ui->contentsWidget);
    updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Персонал"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(ui->contentsWidget);
    queryButton->setIcon(QIcon(":/images/query.png"));
    queryButton->setText(tr("Звіти"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(ui->contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}
void StaffDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
}
//////////////////////////////////////

void StaffDialog::createActions()
{
minimizeAction = new QAction(tr("Зг&орнути"), this);
connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

restoreAction = new QAction(tr("&Розгорнути"), this);
connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

quitAction = new QAction(tr("&Вихід"), this);
connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void StaffDialog::createTrayIcon()
{
trayIconMenu = new QMenu(this);
trayIconMenu->addAction(minimizeAction);
trayIconMenu->addAction(restoreAction);
trayIconMenu->addSeparator();
trayIconMenu->addAction(quitAction);

trayIcon = new QSystemTrayIcon(this);
trayIcon->setToolTip(tr("Персонал"));
trayIcon->setContextMenu(trayIconMenu);

}


void StaffDialog::showMessage()
{
QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
trayIcon->showMessage(tr("Персонал"), tr("Пенсіонери"), icon, 1000000);
}

void StaffDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
//showMessage();
switch (reason)
{
case QSystemTrayIcon::Trigger:
show();

break;
case QSystemTrayIcon::DoubleClick:
show();
break;
case QSystemTrayIcon::MiddleClick:
showMessage();
break;
default:
break;
}
}


bool StaffDialog::connectDB(const QString &dBName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(dBName);
    if (!db.open()) {
        QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), db.lastError().text());
        return false;
    }
    return true;
}

void StaffDialog::deleteModel()
{
    delete positionMapper;
    delete agepentionMapper;
    delete compositionsMapper;
    delete position;
    delete agepention;
    delete compositions;
}

void StaffDialog::createModel()
{
    deleteModel();
    position = new Table(0, "position");
    position->openTable();
    positionMapper = new TableIOMapper(position);
    agepention = new Table(0, "agepention_catalog");
    agepention->addRelation("id_gender", "gender", "id_gender", "type_gender");
    agepention->openTable();
    agepentionMapper = new TableIOMapper(agepention);

    compositions = new Table(0, "personal");
    compositions->addRelation("id_position", "position", "id_position", "name_position");
    compositions->addRelation("id_pention", "pention", "id_pention", "type_pention");
    compositions->addRelation("id_gender", "gender", "id_gender", "type_gender");
    compositions->addRelation("id_vnz", "vnz", "id_vnz", "type_vnz");
    compositionsMapper = new TableIOMapper(compositions);
}

void StaffDialog::openDB()
{
    QFileDialog fd;
    fd.setDefaultSuffix("db");
    fd.setWindowTitle(QString::fromLocal8Bit("Открыть базу данных"));

  //  fd.setMode(QFileDialog::AnyFile);
    fd.setNameFilter(QString::fromLocal8Bit("Базы данных (*.db)"));
    if (fd.exec() == QDialog::Accepted) {
        if (fd.selectedFiles().count() > 0) {
            dbName = fd.selectedFiles().at(0);
            setWindowTitle(QString::fromLocal8Bit("Персонал - ") + dbName);
            connectDB(dbName);
            createModel();
            //createModel();
            setupView();
           // setupView();
            Utils(dbName);

            rep->setPersonList(dbName);
          //  setupView();
            showReport();
        }
    }

}

bool StaffDialog::closeDB( QSqlDatabase& db)
{


    if (db.isOpen())
          db.close();

return true;


}
void StaffDialog::setupView()
{
    agepentionForm->setMapper(agepentionMapper);
//    agepentionMapper->setColumnName(1,QString::fromLocal8Bit("Вік"));
// agepentionMapper->setColumnName(2,QString::fromLocal8Bit("Початок"));
//  agepentionMapper->setColumnName(3,QString::fromLocal8Bit("Кінець"));
//   agepentionMapper->setColumnName(4,QString::fromLocal8Bit("Стать"));

    positionForm->setMapper(positionMapper);

    compositionsMapper->setView(ui->tableView);
    compositionsMapper->refresh();
    compositionsMapper->setColumnName(1, QString::fromLocal8Bit("Прізвище"));
    compositionsMapper->setColumnName(2, QString::fromLocal8Bit("Імя"));
    compositionsMapper->setColumnName(3, QString::fromLocal8Bit("По батькові"));
    compositionsMapper->setColumnName(4, QString::fromLocal8Bit("Народження"));
      compositionsMapper->setColumnName(5, QString::fromLocal8Bit("Стать"));
        compositionsMapper->setColumnName(6, QString::fromLocal8Bit("ВНЗ"));
          compositionsMapper->setColumnName(7, QString::fromLocal8Bit("Посада"));
            compositionsMapper->setColumnName(8, QString::fromLocal8Bit("Пенсія"));
    ui->tableView->hideColumn(0);
     ui->tableView->setColumnWidth(4, 100);
         ui->tableView->setColumnWidth(5, 50);
             ui->tableView->setColumnWidth(6, 50);
                 ui->tableView->setColumnWidth(7, 190);
                     ui->tableView->setColumnWidth(8, 60);
    ui->tableView->setItemDelegateForColumn(4,new DeDelegate(true,this));
}

bool StaffDialog::createDB(const QString &dBName)
{
    if (!connectDB(dBName))
        return false;
    QSqlQuery q;
    q.prepare("PRAGMA foreign_keys = ON;");
    if (!q.exec()) {
        QMessageBox::critical(this, QString::fromLocal8Bit("Помилка"), q.lastError().text());
        return false;
    }
    // Хотя у SQLite есть ROWID, у этих таблиц должны быть явно объявленные первичные ключи,
    // иначе реляционная модель не сработает.
    q.prepare("create table position(Name varchar(32) not NULL, id integer primary key)");
    if (!q.exec()) {
        QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), q.lastError().text());
        return false;
    }
    q.prepare("create table agepention(Name varchar(32) not NULL, id integer primary key)");
    if (!q.exec()) {
        QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), q.lastError().text());
        return false;
    }
    q.prepare("create table Compositions(Title varchar(64) not NULL, Artist integer not NULL, Genre integer not NULL, Year char(4), Id integer primary key, unique(Title, Artist), foreign key (Artist) references position(ROWID), foreign key (Genre) references agepention(ROWID))");
    if (!q.exec()) {
        QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), q.lastError().text());
        return false;
    }
    return true;
}

void StaffDialog::addRecord()
{
    if (!compositionsMapper.isNull())
        compositionsMapper->newRow();
}

void StaffDialog::deleteRecord()
{
    if (!compositionsMapper.isNull()) {
        int row = ui->tableView->currentIndex().row();
        if (row >= 0) {
            compositionsMapper->deleteRow(row);
        }
    }
}

void StaffDialog::save()
{
    if (!compositionsMapper.isNull()) {
        ui->tableView->submit();
        compositionsMapper->save();
        Utils(dbName);
        rep->setPersonList(dbName);
        showReport();
    }

}

void StaffDialog::cancelChanges()
{
    if (!compositionsMapper.isNull())
        compositionsMapper->cancelChanges();
}

void StaffDialog::newDB()
{
    QFileDialog fd;
    fd.setDefaultSuffix("db");
    fd.setWindowTitle(QString::fromLocal8Bit("Новая база данных"));
  //  fd.setMode(QFileDialog::AnyFile);
    fd.setNameFilter(QString::fromLocal8Bit("Базы данных (*.db)"));
    if (fd.exec() == QDialog::Accepted) {
        if (fd.selectedFiles().count() > 0) {
            dbName = fd.selectedFiles().at(0);
            setWindowTitle(QString::fromLocal8Bit("Персонал- ") + dbName);
            createDB(dbName);
            createModel();
            setupView();
            Utils(dbName);
            rep->setPersonList(dbName);
        }
    }
    Utils(dbName);
}

void StaffDialog::showpositionForm()
{
    positionForm->setWhatsThis("<img src=\":/images/profesion.png\">"
    "&nbsp;База даних відділу кадрів. Список посад коледджу  "

  );
    positionForm->exec();
    if (!compositionsMapper.isNull()) {
        compositionsMapper->reloadRelations();
        compositionsMapper->refresh();
    }

}

void StaffDialog::showagepentionForm()
{
    agepentionForm->setWhatsThis("<img src=\":/images/profesion.png\">"
    "&nbsp;База даних відділу кадрів. Каталог пенсійного віку  "

  );
    agepentionForm->exec();
    if (!compositionsMapper.isNull()) {
        compositionsMapper->reloadRelations();
        compositionsMapper->refresh();
    }
}
void StaffDialog::showReport()
{

//connectDB(dbName);
//createModel();
//setupView();
     ui->viewRep->setModel(rep->CreateReportModel());
      ui->viewRep->hideColumn(0);
      ui->viewRep->setColumnWidth(1, 180);
       ui->viewRep->setColumnWidth(2, 100);
          ui->viewRep->setColumnWidth(3, 50);
           ui->viewRep->setColumnWidth(4, 50);
           ui->viewRep->setColumnWidth(5, 190);
              ui->viewRep->setColumnWidth(6, 60);
                  ui->viewRep->setColumnWidth(7, 70);
                      ui->viewRep->setColumnWidth(8, 60);


}
void StaffDialog::backup()
{
    if(closeDB(db))
    {
QMessageBox::information(this, QString::fromLocal8Bit("База закрита для резервування"), "Починаємо резервування...");
QString path = QFileDialog::getExistingDirectory(0,
                         "Виберіть шлях для резервного копіювання бази",
                                 "", QFileDialog::ShowDirsOnly);
// dir.absolutePath() << flush;
//QString path1=QDir::currentPath() ;//qDebug() << "App path : " << qApp->applicationDirPath();
QDir dir;
QString path1=dir.absolutePath() ;
   if (path == "" )
   {

       QMessageBox::critical(this,"Шлях для резервного копіювання не вибрано"," Відміна резервування.");


   }
   else
   {
       QString cur=QDateTime::currentDateTime().toString("dd_MM_yyyy-ss");



  path = path + QString("\\Backup\\staff_") + cur+ ".db" ;
  path1=path1+QString("\\staff.db");
  if( QFile::exists(path))
  {
      QFile::remove(path);
  }

  if(QFile::copy(path1, path))
     QMessageBox::information(this, QString::fromLocal8Bit("Рзервна копія збережена до діректорії Backup"), "Відбулося"); ;

//qDebug()<<path<<endl<<path1;
    }
   connectDB(dbName);
    }
    else
         QMessageBox::critical(this, QString::fromLocal8Bit("не можу закрити базу для резервування"), db.lastError().text());

}

void StaffDialog::backupMessage()
{
    QMessageBox msgBox(
                QMessageBox::Question,
                trUtf8("Резервне копіювання бази "),
               trUtf8("Ви зробили резервне копіювання бази?"),
                QMessageBox::Yes | QMessageBox::No);

    msgBox.setButtonText(QMessageBox::Yes, trUtf8("Зробити"));
    msgBox.setButtonText(QMessageBox::No, trUtf8("Відмовитись"));

    if (msgBox.exec() == QMessageBox::Yes)
        backup();
}
void  StaffDialog::import()
{

   backupMessage();
    showimportForm();
    Utils(dbName);
connectDB(dbName);
createModel();
setupView();
rep->setPersonList(dbName);
showReport();
}


void StaffDialog::showimportForm()
{
    importForm->setWhatsThis("<img src=\":/images/profesion.png\">"
    "&nbsp;База даних відділу кадрів. Опції для імпортування "

  );
    bool sameCol=ui->checkBoxSameCol->isChecked();
importForm->setForm(sameCol);
 importForm->exec();

}
void StaffDialog::saveToCSV()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                           tr("Збережіть CSV"), ".",
                                           tr("CSV файли (*.csv)"));
    QFile f(filename);
        if (!f.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "", tr("Помилка збереження до файлу.\n%1").arg(f.errorString()));
            return;
        }



            QTextStream ts( &f );  // #include <QtCore/QTextStream>
            QStringList strList;

            for( int r = 0; r < ui->tableView->model()->rowCount(); ++r ){
                strList.clear();
                for( int c = 0; c < ui->tableView->model()->columnCount(); ++c ){
                    //tableView->selectionModel()->select(tabelView->model()->index(row,colum), QItemSelectionModel::Select);
                  //  QModelIndex index = model->index(r, c, QModelIndex());
QString s=ui->tableView->model()->data(ui->tableView->model()->index(r,c)).toString();

                    strList << "\""+s+"\"";
                }
                ts << strList.join( ";" )+"\n";
            }
            if(f.exists())
                QMessageBox::information(this,"Експорт","Файл "+filename+" успішно створено");
            f.close();

}
void StaffDialog::saveToXML()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                           tr("Збережіть XML"), ".",
                                           tr("XML файли (*.xml)"));

    QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "", tr("Помилка збереження до файлу.\n%1").arg(file.errorString()));
            return;
        }
    QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement(tr("Персонал"));

        for (int i = 0; i < ui->tableView->model()->rowCount(); i++) {
            stream.writeStartElement(tr("Персона"));

            for (int j = 0; j < ui->tableView->model()->columnCount(); j++) {
                const QVariant& value = ui->tableView->model()->index(i, j).data(Qt::DisplayRole);
                if(j==0)
                     stream.writeTextElement("id", value.toString());
                else
                stream.writeTextElement(ui->tableView->model()->headerData(j, Qt::Horizontal, Qt::EditRole).toString(), value.toString());
            }
            stream.writeEndElement();
        }

        stream.writeEndElement();

        stream.writeEndDocument();


        if(file.exists())
            QMessageBox::information(this,"Експорт","Файл "+filename+" успішно створено");
        file.close();
}
void StaffDialog::setModes()
{

#ifdef Q_WS_WIN
    //HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
QSettings bootUpSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
//QString path1=QDir::currentPath() ;
//qDebug() << "App path : " << qApp->applicationDirPath();
QDir dir;
QString base_dir=dir.absolutePath() ;
qDebug() <<base_dir;
if (ui->checkBoxWindows->isChecked())
{
    bootUpSettings.setValue("QStaff","\""+base_dir+"\""+ (ui->checkBoxInTray->isChecked() ? " -m" : ""));
}
else
{
    bootUpSettings.remove("QStaff");
    ui->checkBoxWindows->setChecked(false);
}
#endif


}
void StaffDialog::setUnCheck()
{
    ui->checkBoxInTray->setChecked(false);
}

