#include "viewcatalogform.h"
#include "ui_viewcatalogform.h"
#include "tableiomapper.h"
#include "dedelegate.h"
ViewCatalogForm::ViewCatalogForm(QWidget *parent):
    QDialog(parent),
    ui(new Ui::ViewCatalogForm)
{
    ui->setupUi(this);
    connect(ui->toolButtonRemove, SIGNAL(clicked()), this, SLOT(deleteItem()));
    connect(ui->toolButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->toolButtonSave, SIGNAL(clicked()), this, SLOT(save()));
 }

ViewCatalogForm::~ViewCatalogForm()
{
    delete ui;

}

void ViewCatalogForm::deleteItem()
{
    if (!mapper.isNull())
        mapper->deleteRow(ui->tableView->currentIndex().row());
}

void ViewCatalogForm::setMapper(TableIOMapper *mapper)
{
    this->mapper = mapper;
    int columns=mapper->columnsCount();
    mapper->setView(ui->tableView);
    mapper->setColumnName(0, windowTitle());

   // ui->tableView->setColumnWidth(0, 270);
 // ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );

   // ui->tableView->setColumnWidth(QHeaderView::ResizeToContents);
    ui->tableView->hideColumn(0);
    if(columns<=3)
    {
     mapper->setColumnName(1, "Посада");
      mapper->setColumnName(2, "Робоча");
       ui->tableView->setColumnWidth(1,250);
 ui->tableView->setColumnWidth(2, 50);

    }
    else
    {
     mapper-> setColumnName(1,QString::fromLocal8Bit("Вік"));
     mapper-> setColumnName(2,QString::fromLocal8Bit("Початок"));
    mapper-> setColumnName(3,QString::fromLocal8Bit("Кінець"));
        mapper-> setColumnName(4,QString::fromLocal8Bit("Стать"));
          ui->tableView->setColumnWidth(1, 50);
        ui->tableView->setColumnWidth(2, 100);
           ui->tableView->setColumnWidth(3, 100);
             ui->tableView->setColumnWidth(4, 50);
            ui->tableView->setItemDelegateForColumn(2,new DeDelegate(true,this));
                ui->tableView->setItemDelegateForColumn(3,new DeDelegate(true,this));

    }
}

void ViewCatalogForm::on_toolButtonAdd_clicked()
{
        if (!mapper.isNull())
            mapper->newRow();
}

void ViewCatalogForm::save()
{
        if (!mapper.isNull()) {
            ui->tableView->submit();
            mapper->save();
        }
}
