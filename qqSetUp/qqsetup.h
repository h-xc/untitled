#ifndef QQSETUP_H
#define QQSETUP_H

#include <QWidget>

namespace Ui {
class qqSetUp;
}

class qqSetUp : public QWidget
{
    Q_OBJECT

public:
    explicit qqSetUp(QWidget *parent = 0);
    ~qqSetUp();

private:
    Ui::qqSetUp *ui;
};

#endif // QQSETUP_H
