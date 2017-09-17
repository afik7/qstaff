#include "dedelegate.h"
#include <QtWidgets>
//DeDelegate::DeDelegate(QObject *parent) :
 //   QStyledItemDelegate(parent)
//{
//}
DeDelegate::DeDelegate(
                    bool calpopup,
                    QObject *parent)
            :  QStyledItemDelegate(parent),
              m_calpopup(calpopup) {
}

QWidget *DeDelegate::createEditor(
            QWidget *parent,
            const QStyleOptionViewItem& /* option */,
            const QModelIndex& /* index */) const {
    QDateEdit *editor = new QDateEdit(parent);
    editor->setCalendarPopup(m_calpopup);
    #if QT_VERSION >= 0x040400
    if (m_calpopup)
        editor->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
    #endif
    editor->installEventFilter(const_cast<DeDelegate*>(this));
    editor->setDisplayFormat("dd.MM.yyyy");
    return editor;
}

void DeDelegate::setEditorData(
                QWidget *editor,
                const QModelIndex &index) const {
    QDate value = index.model()->data(
            index, Qt::EditRole).toDate();
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->setDate(value);
}

void DeDelegate::setModelData(
            QWidget *editor, 
            QAbstractItemModel *model,
            const QModelIndex& index) const {
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->interpretText();
    QDate value = de->date();
    model->setData(index, value);
}

void DeDelegate::updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option, 
            const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}
QString DeDelegate::displayText(const QVariant &value, const QLocale &locale) const
{

    return locale.toString(value.toDate(), "dd.MM.yyyy");

}
