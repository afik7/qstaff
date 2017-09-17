#include "table.h"
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QSqlRelationalDelegate>
#include <QStringList>
#include <QSqlRelation>
#include <QStyleOption>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

Table::Table(QSqlDatabase *db, const QString &TableName)
{
    m_model = new QSqlRelationalTableModel(0);
    m_model->setTable(TableName);
    names = new QStringList();
}

bool Table::selectRows(const QString &where)
{
    m_model->setFilter(where);
    m_model->select();
    return (m_model->rowCount() > 0);
}

void Table::addRelation(int column, const QString &ExtTable, const QString &ExtKey, const QString &ExtDisplay)
{
    QSqlRelation rel(ExtTable, ExtKey, ExtDisplay);
    m_model->setRelation(column, rel);
}

void Table::addRelation(const QString &column, const QString &ExtTable, const QString &ExtKey, const QString &ExtDisplay)
{
    addRelation(this->columnIndex(column), ExtTable, ExtKey, ExtDisplay);
}

bool Table::openTable()
{
     m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
     if (!m_model->select())
             return false;
     names->clear();
     for (int i =0; i < m_model->columnCount(); i++)
         names->append(m_model->headerData(i, Qt::Horizontal).toString());
     return true;
}

bool Table::newRow()
{
    m_model->submitAll();
    m_model->setFilter("");
    m_model->select();
    if (!m_model->insertRow(0)) {
        return false;
    }
    return true;
}

void Table::deleteRow(int row)
{
    if (!m_model->removeRow(row)) {
  //      QMessageBox::message("error", m_model->lastError().text());
        //QMessageBox::information(this,"error", m_model->lastError().text());
        QMessageBox msgBox(QMessageBox::Information,
                       "error",
                       m_model->lastError().text());
        msgBox.show();
    }
}

void Table::cancelChanges()
{
    m_model->revertAll();
}

bool Table::setTableValue(int row, const QString &FieldName, const QVariant &value)
{
        return setTableValue(row, m_model->fieldIndex(FieldName), value);
}

bool Table::setTableValue(int row, int column, const QVariant &value)
{
        return m_model->setData(m_model->index(row, column), value);
}


QVariant Table::getTableValue(int row, const QString &FieldName)
{
    return getTableValue(row, m_model->fieldIndex(FieldName));
}

QVariant Table::getTableValue(int row, int column)
{
        return m_model->data(m_model->index(row, column));
}

void Table::renameColumn(const QString &OldName, const QString &NewName)
{
    this->setTableColumnName(names->indexOf(OldName), NewName);
}

bool Table::save()
{
    if (!m_model->submitAll()) {
       // QMessageBox::message("Error", m_model->lastError().text());
        QMessageBox msgBox(QMessageBox::Information,
                       "error",
                       m_model->lastError().text());
        msgBox.show();
        return false;
    }
    return true;
}

void Table::setTableColumnName(int index, const QString &name)
{
    m_model->setHeaderData(index, Qt::Horizontal, name);
}

int Table::rowsCount()
{
    return m_model->rowCount();
}

Table::~Table()
{
    delete m_model;
    delete names;
}

QString Table::getColumnName(int column)
{
    return names->value(column);
}

int Table::columnIndex(const QString &column)
{
    return m_model->fieldIndex(column);
}

