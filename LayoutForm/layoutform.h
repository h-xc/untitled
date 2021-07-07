#ifndef LAYOUTFORM_H
#define LAYOUTFORM_H

#include <QWidget>
#include <QFormLayout>
#include <QPushButton>
#include <QCommandLinkButton>
#include <QApplication>
#include <QVBoxLayout>
namespace Ui {
class layoutForm
{
public:
    QFormLayout *formLayout;
    QPushButton *pushButton;
    QCommandLinkButton *commandLinkButton;

    void setupUi(QWidget *layoutForm)
    {
        if (layoutForm->objectName().isEmpty())
            layoutForm->setObjectName(QStringLiteral("layoutForm"));

        QVBoxLayout *Vlayout = new QVBoxLayout(layoutForm);
        formLayout = new QFormLayout(layoutForm);

        layoutForm->resize(400, 300);
        //formLayout->setObjectName(QStringLiteral("formLayout"));
        pushButton = new QPushButton(layoutForm);
        //shButton->setObjectName(QStringLiteral("pushButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, pushButton);

        commandLinkButton = new QCommandLinkButton(layoutForm);
        //commandLinkButton->setObjectName(QStringLiteral("commandLinkButton"));

        formLayout->setWidget(0, QFormLayout::FieldRole, commandLinkButton);

 Vlayout->addLayout(formLayout);
        retranslateUi(layoutForm);

        //QMetaObject::connectSlotsByName(layoutForm);
    } // setupUi

    void retranslateUi(QWidget *layoutForm)
    {
        layoutForm->setWindowTitle(QApplication::translate("layoutForm", "Form", 0));
        pushButton->setText(QApplication::translate("layoutForm", "PushButton", 0));
        commandLinkButton->setText(QApplication::translate("layoutForm", "CommandLinkButton", 0));
    } // retranslateUi
};
}

class layoutForm : public QWidget
{
    Q_OBJECT

public:
    explicit layoutForm(QWidget *parent = 0);
    ~layoutForm();

private:
    Ui::layoutForm *ui;
};

#endif // LAYOUTFORM_H
