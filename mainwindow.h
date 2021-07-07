#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_qqSetUp_clicked();

    void on_LayoutTest_clicked();

    void on_sysSet_clicked();

    void on_QMLSet_clicked();

    void on_WinCMD_clicked();

    void on_Socket_clicked();

    void on_layoutForm_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
