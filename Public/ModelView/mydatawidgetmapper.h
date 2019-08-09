#ifndef MYDATAWIDGETMAPPER_H
#define MYDATAWIDGETMAPPER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QPersistentModelIndex>
class QDataWidgetMapperPrivate;
 enum SubmitPolicy { AutoSubmit, ManualSubmit };

class mydataWidgetMapper : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(SubmitPolicy submitPolicy READ submitPolicy WRITE setSubmitPolicy)

public:
    explicit mydataWidgetMapper(QObject *parent = 0);
    ~mydataWidgetMapper();

    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model() const;

    void setItemDelegate(QAbstractItemDelegate *delegate);
    QAbstractItemDelegate *itemDelegate() const;

    void setRootIndex(const QModelIndex &index);
    QModelIndex rootIndex() const;

    void setOrientation(Qt::Orientation aOrientation);
    Qt::Orientation orientation() const;


   // Q_ENUM(SubmitPolicy)

    void setSubmitPolicy(SubmitPolicy policy);
    SubmitPolicy submitPolicy() const;

    void addMapping(QWidget *widget,int index, int section);
    //void addMapping(QWidget *widget, int section, const QByteArray &propertyName);
    void addMapping(QWidget *widget,int index, int section, const QByteArray &propertyName);

    void removeMapping(QWidget *widget);
    int mappedSection(QWidget *widget) const;
    QByteArray mappedPropertyName(QWidget *widget) const;
    QWidget *mappedWidgetAt(int section) const;
    void clearMapping();

   // int currentIndex() const;

public Q_SLOTS:
    void revert();
    bool submit();

    //void toFirst();
    //void toLast();
    //void toNext();
    //void toPrevious();
    //virtual void setCurrentIndex(int index);
    //void setCurrentModelIndex(const QModelIndex &index);

Q_SIGNALS:
    void currentIndexChanged(int index);

private:
   // Q_DECLARE_PRIVATE(mydataWidgetMapper)
    //Q_DISABLE_COPY(mydataWidgetMapper)
    QDataWidgetMapperPrivate  *d;

private Q_SLOTS:
    void _q_dataChanged(const QModelIndex &, const QModelIndex &);
    void _q_commitData(QWidget *);
    void _q_closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint);
    void _q_modelDestroyed();
};

#endif // MYDATAWIDGETMAPPER_H
