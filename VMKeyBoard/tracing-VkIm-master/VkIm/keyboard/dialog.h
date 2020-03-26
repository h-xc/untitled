#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QGridLayout;
class QLineEdit;
class QPushButton;
class QRegExpValidator;
class QDBusVariant;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public:
    void initWidgets();

public slots:
    void clickButton(int index);
    void showKeyboard(QPoint pt, QRect focusWidget);
    void hideKeyboard();
    bool isVisible() const;
    void pressKey(int key);

signals:
    void commit(QString str);

private:
    QPushButton *numBt[10];
    QPushButton *dotBt;
    QPushButton *minusBt;
    QPushButton *backspaceBt;
    QPushButton *enterBt;
    QLineEdit *inputEdit;
};

#endif // DIALOG_H
