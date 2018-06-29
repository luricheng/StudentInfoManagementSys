#include "login.h"
#include "ui_login.h"

#include <studentwin.h>
#include <teacherwin.h>
#include <adminwin.h>

class StudentWin;
class teacherWin;
class AdminWin;

Login::Login(QWidget *parent, QSqlDatabase *dataBase) :
    QWidget(parent),
    ui(new Ui::Login),
    dataBase(dataBase)
{
    ui->setupUi(this);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

}

Login::~Login()
{
    delete ui;
}

void Login::on_okPushButton_clicked()
{
    // get id and password
    const QString &id = ui->idLineEdit->text();
    const QString &password = ui->passwordLineEdit->text();

    // get password-md5
    QByteArray byteArray;
    byteArray.append(password);
    QByteArray hashResult = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);
    QString passwordMd5 = hashResult.toHex();

    qDebug()<<passwordMd5<<endl;

    // Check id and password
    QSqlQuery query;
    int accountType = 0;
    if(ui->studentRadioButton->isChecked()){
        query.prepare("select id from student where id=? and password=?");
        accountType = 1;
    }
    else{
        if(ui->teacherRadioButton->isChecked()){
            query.prepare("select id from teacher where id=? and password=?");
            accountType = 2;
        }
        else{
            query.prepare("select id from admin where id=? and password=?");
            accountType = 3;
        }
    }
    query.bindValue(0,id);
    query.bindValue(1,passwordMd5);
    query.exec();
    if(!query.next()){
        QMessageBox::warning(this, tr("警告"), tr("账号或密码错误!"));
        return;
    }

    // login success
    qDebug()<<"login success"<<endl;

    //set account
    Account account(id,accountType);

    bool loginSuccess = false;
    if(accountType==1){
        StudentWin *swin = new StudentWin(&account);
        swin->show();
        loginSuccess = true;
    }
    if(accountType==2){
        teacherWin *twin = new teacherWin(&account);
        twin->show();
        loginSuccess = true;
    }
    if(accountType==3){
        AdminWin *awin = new AdminWin(&account);
        awin->show();
        loginSuccess = true;
    }

    this->destroy();
}

// set enabled when both id and password are not empty
void Login::on_idLineEdit_textChanged(const QString &arg1)
{
    ui->okPushButton->setEnabled(!(arg1.isEmpty()||ui->passwordLineEdit->text().isEmpty()));
}

void Login::on_passwordLineEdit_textChanged(const QString &arg1)
{
    ui->okPushButton->setEnabled(!(arg1.isEmpty()||ui->idLineEdit->text().isEmpty()));
}

void Login::on_cancelPushButton_clicked()
{
    exit(0);
}
