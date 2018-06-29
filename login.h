#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <account.h>
#include <QLineEdit>
#include <QCryptographicHash>
#include <QPushButton>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDesktopWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0, QSqlDatabase *dataBase = nullptr);
    ~Login();

private slots:
    void on_okPushButton_clicked();

    void on_idLineEdit_textChanged(const QString &arg1);

    void on_passwordLineEdit_textChanged(const QString &arg1);

    void on_cancelPushButton_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase *dataBase;
};

#endif // LOGIN_H
