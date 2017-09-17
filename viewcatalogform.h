#ifndef VIEWCATALOGFORM_H
#define VIEWCATALOGFORM_H

#include <QDialog>
#include <QPointer>

namespace Ui {
    class ViewCatalogForm;
}

class TableIOMapper;

typedef QPointer<TableIOMapper> MapperPtr;

class ViewCatalogForm : public QDialog
{
    Q_OBJECT

public:
    explicit ViewCatalogForm(QWidget *parent = 0);
    ~ViewCatalogForm();
    void setMapper(TableIOMapper * mapper);
private slots:
    void on_toolButtonAdd_clicked();
    void deleteItem();
    void save();
private:
    Ui::ViewCatalogForm *ui;
    MapperPtr mapper;
};

#endif // VIEWCATALOGFORM_H
