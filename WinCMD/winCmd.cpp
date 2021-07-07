#include "winCmd.h"
#include "ui_winCmd.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTextCodec>
#include <QRegExp>

WinCMD::WinCMD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinCMD),
    m_pCmdProcess(NULL)
{
    ui->setupUi(this);
    m_pCmdProcess = new QProcess(this);
    connect(m_pCmdProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(error(QProcess::ProcessError)));
    connect(m_pCmdProcess,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(finished(int, QProcess::ExitStatus)));
    connect(m_pCmdProcess,SIGNAL(readyReadStandardError()),this,SLOT(readyReadStandardError()));
    connect(m_pCmdProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(m_pCmdProcess,SIGNAL(started()),this,SLOT(started()));
    connect(m_pCmdProcess,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(stateChanged(QProcess::ProcessState )));
    m_pCmdProcess->start("cmd");
}

WinCMD::~WinCMD()
{
    m_pCmdProcess->write("exit\r\n");
    m_pCmdProcess->waitForFinished(3000);
    delete m_pCmdProcess;
    delete ui;
}

QString  WinCMD::showReadStr(QByteArray byte)
{
    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString strTemp = utf8->fromUnicode((gbk->toUnicode(byte)));
    QTextCursor tc = ui->textEdit_CMD_Ret->textCursor();
    tc.movePosition(QTextCursor::End);
    tc.insertText(strTemp);
    return strTemp;
}

void	WinCMD::error(QProcess::ProcessError error)
{
    qDebug() << error;
}

void	WinCMD::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "finished exitCode"   << exitCode;
    qDebug() << "finished exitStatus" << exitStatus;
}

void	WinCMD::readyReadStandardError()
{
    qDebug() << "readyReadStandardError";
    showReadStr("error:" + m_pCmdProcess->readAllStandardError());
}

void	WinCMD::readyReadStandardOutput()
{
    qDebug() << "readyReadStandardOutput";
    showReadStr("Output:" + m_pCmdProcess->readAllStandardOutput());
}

void	WinCMD::started()
{
    qDebug() << "started";
}

void	WinCMD::stateChanged(QProcess::ProcessState newState)
{
    qDebug() << "stateChanged" << newState;
}

void WinCMD::on_pushButton_cmd_clicked()
{
    m_pCmdProcess->write(ui->lineEdit->text().toLocal8Bit());
    m_pCmdProcess->write("\r\n");
    qDebug() << "on_pushButton_cmd_clicked";
}
