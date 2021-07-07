#ifndef SOCKET_H
#define SOCKET_H

#include <QWidget>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class Socket;
}

class Socket : public QWidget
{
    Q_OBJECT

public:
    explicit Socket(QWidget *parent = 0);
    ~Socket();

    void UIsetVisible(bool visible);

    QTcpServer* mp_TCPServer;
    enum SocketType
    {
        TCPserver,
        TCPclient,
        UDP
    };
    SocketType m_socketType;

    QList<QTcpSocket*> m_TCPSocketList;

private slots:
    void on_pushButton_start_clicked();
    void on_comboBox_socket_type_currentIndexChanged(int index);

    void on_pushButton_start_pressed();

    void on_pushButton_start_released();
    void ServerNewConnection();

private:
    Ui::Socket *ui;
};

#endif // SOCKET_H
