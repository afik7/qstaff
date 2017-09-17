#ifndef TABLEIOMAPPER_H
#define TABLEIOMAPPER_H

#include "table.h"
#include <QObject>
#include <QMap>

class QDataWidgetMapper;
class QWidget;
class QComboBox;
class QListView;
class QAbstractItemView;
class QComboBox;

class TableIOMapper : public QObject
{
    Q_OBJECT
public:
    explicit TableIOMapper(Table * table);
    ~TableIOMapper();
    bool selectRow(int num);
    bool selectNextRow();
    bool selectPrevRow();
    void selectFirstRow();
    void selectLastRow();
    int rowsCount();
    int columnsCount();
    int currentRow();
    void addDataWidget(QWidget * widget, int column);
    void addDataWidget(QWidget * widget, const QString &columnName);
    void addRelationalComboBox(QComboBox * comboBox, int column);
    void addRelationalComboBox(QComboBox * comboBox, const QString &columnName);
    void reloadRelation(QComboBox * comboBox);
    void reloadRelations();
    void clearDataWidgets();
    void setView(QAbstractItemView * view);
    void setColumnName(int column, const QString &name);
public slots:
    void newRow();
    void save();
    void cancelChanges();
    void refresh();
    void deleteRow(int num);
protected:
    QDataWidgetMapper * mapper;
    Table * m_table;
};

#endif // TABLEIOMAPPER_H
