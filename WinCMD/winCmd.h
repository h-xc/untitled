#ifndef WINCMD_H
#define WINCMD_H

#include <QWidget>
#include <QProcess>
#include <QByteArray>

namespace Ui {
class WinCMD;
}

class WinCMD : public QWidget
{
    Q_OBJECT

public:
    explicit WinCMD(QWidget *parent = 0);
    ~WinCMD();

private:
    Ui::WinCMD *ui;
    QProcess *m_pCmdProcess;
    QString showReadStr(QByteArray byte);

private slots:
    void	error(QProcess::ProcessError error);
    void	finished(int exitCode, QProcess::ExitStatus exitStatus);
    void	readyReadStandardError();
    void	readyReadStandardOutput();
    void	started();
    void	stateChanged(QProcess::ProcessState newState);
    void on_pushButton_cmd_clicked();
};

#endif // WINCMD_H
