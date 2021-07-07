#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QMainWindow>

namespace Ui {
class toolbar;
}

class toolbar : public QMainWindow
{
    Q_OBJECT

public:
    explicit toolbar(QWidget *parent = 0);
    ~toolbar();

private:
    Ui::toolbar *ui;
};

#endif // TOOLBAR_H
