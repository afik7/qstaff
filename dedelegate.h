#ifndef DEDELEGATE_H
#define DEDELEGATE_H

#include <QStyledItemDelegate>


class DeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    DeDelegate(bool calpopup = true,
                 QObject *parent = 0);
    QWidget *createEditor(
                QWidget *parent, 
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    void setEditorData(QWidget *editor, 
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor, 
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option, 
            const QModelIndex &index) const;
    QString displayText(const QVariant &value, const QLocale &locale) const;
private:
    bool m_calpopup;
};


#endif // DEDELEGATE_H
