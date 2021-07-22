#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#define COLNUM_COUNT 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addWidget(QWidget *pWidget);

private:
    int  m_widgetCount;

private slots:
    void on_pushButton_clicked();

    void on_QMLSet_clicked();

    void on_codeTest_clicked();

private:
    Ui::MainWindow *ui;
    QGridLayout *m_GLayout;
};

#endif // MAINWINDOW_H
