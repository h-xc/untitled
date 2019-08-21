#ifndef LAYOUTTEST_H
#define LAYOUTTEST_H

#include <QWidget>

namespace Ui {
class LayoutTest;
}

class LayoutTest : public QWidget
{
    Q_OBJECT

public:
    explicit LayoutTest(QWidget *parent = NULL);
    ~LayoutTest();

private:
    Ui::LayoutTest *ui;
};

#endif // LAYOUTTEST_H
