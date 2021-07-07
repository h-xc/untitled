#include "socket.h"
#include "ui_socket.h"

Socket::Socket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Socket)
{
    ui->setupUi(this);
    mp_TCPServer = new QTcpServer();
    on_comboBox_socket_type_currentIndexChanged(0);
}

Socket::~Socket()
{
    delete ui;
}

void Socket::on_pushButton_start_clicked()
{

}

void Socket::on_comboBox_socket_type_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        m_socketType = TCPserver;
        ui->pushButton_start->setText("打开");
        break;
    case 1:
        m_socketType = TCPclient;
        ui->pushButton_start->setText("连接");
        break;
    case 2:
        m_socketType = UDP;
        ui->pushButton_start->setText("打开");
        break;
    }
}
void Socket::UIsetVisible(bool visible)
{
    ui->lineEdit_local_IP->setVisible(visible);
    ui->lineEdit_local_port->setVisible(visible);
    ui->lineEdit_remote_IP->setVisible(visible);
    ui->lineEdit_remote_port->setVisible(visible);
}
void Socket::on_pushButton_start_pressed()
{
    QHostAddress localIPaddr = QHostAddress(ui->lineEdit_local_IP->text());
    int localPort = ui->lineEdit_local_port->text().toInt();	//
    QHostAddress remoteIPaddr(ui->lineEdit_remote_IP->text());	//
    int remotePort = ui->lineEdit_remote_port->text().toInt();	//

    switch(m_socketType)
    {
    case TCPserver:
        if(!mp_TCPServer->listen(localIPaddr, localPort))
        {
            return;
        }
        connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
        break;
    case TCPclient:

        break;
    case UDP:

        break;
    }
    ui->lineEdit_remote_port->setVisible(false);
}

void Socket::ServerNewConnection()
{
    m_TCPSocketList.append(mp_TCPServer->nextPendingConnection());
}

void Socket::on_pushButton_start_released()
{
    switch(m_socketType)
    {
    case TCPserver:
        for(int pos = 0;pos < m_TCPSocketList.count();pos++)
        {
            mp_TCPServer->close();
            delete mp_TCPServer;
        }
        mp_TCPServer->close();
        break;
    case TCPclient:
        break;
    case UDP:

        break;
    }
}
