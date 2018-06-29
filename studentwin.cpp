#include "studentwin.h"
#include "ui_studentwin.h"

#include<QSqlQuery>
#include<QDebug>
#include<QLabel>
#include<QLayout>
#include<QStandardItemModel>
#include<QTableView>
#include<QDesktopWidget>

StudentWin::StudentWin(Account*account, QWidget *parent) :
    QWidget(parent),
    Account(*account),
    ui(new Ui::StudentWin)
{
    ui->setupUi(this);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    id = account->getId();

    QSqlQuery query;

    // set info
    if(!query.exec("select id, name, sex, age, grade, major, class from student where id='" + id + "'")
            || !query.next()){
        qDebug()<<"query from database error"<<endl;
        return;
    }
    ui->label_id->setText(query.value(0).toString());
    ui->label_name->setText(query.value(1).toString());
    ui->label_sex->setText(query.value(2).toString());
    ui->label_age->setText(query.value(3).toString());
    ui->label_grade->setText(query.value(4).toString());
    ui->label_major->setText(query.value(5).toString());
    ui->label_class->setText(query.value(6).toString());

    // set score
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("课程名"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("任课教师"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("成绩"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    query.exec("select * from score where student_id='" + id + "'");
    int rowCnt = 0;
    while(query.next()){
        //get a score item
        QString courseId = query.value(2).toString();
        QString score = query.value(3).toString();
        QString teacherId = query.value(1).toString();
        model->setItem(rowCnt, 0, new QStandardItem(getCourseName(courseId)));
        model->setItem(rowCnt, 1, new QStandardItem(getTeacherName(teacherId)));
        model->setItem(rowCnt, 2, new QStandardItem(score));
        ++rowCnt;
    }

}

QString StudentWin::getCourseName(QString courseId){
    QSqlQuery query;
    if(query.exec("select name from course where id='" + courseId + "'")
            &&query.next()){
        return query.value(0).toString();
    }
    return "error";
}

QString StudentWin::getTeacherName(QString teacherId){
    QSqlQuery query;
    if(query.exec("select name from teacher where id='" + teacherId + "'")
            &&query.next()){
        return query.value(0).toString();
    }
    return "error";
}

StudentWin::~StudentWin()
{
    delete ui;
}

void StudentWin::on_pushButton_exit_clicked()
{
    exit(0);
}

void StudentWin::on_tabWidget_currentChanged(int index)
{
    ui->tabWidget->adjustSize();
}

bool StudentWin::checkPasswordInput(){
    bool flag = ui->lineEdit_password->text().isEmpty() ||
            ui->lineEdit_newPassword->text().isEmpty() ||
            ui->lineEdit_passwordRepeat->text().isEmpty();
    if(flag){
        ui->pushButton_changePassword->setEnabled(false);
        return false;
    }
    if(ui->lineEdit_newPassword->text() !=
            ui->lineEdit_passwordRepeat->text()){
        ui->label_passwordTips->setText(tr("2次密码不相同"));
        ui->pushButton_changePassword->setEnabled(false);
        return false;
    }
    ui->label_passwordTips->setText(tr(""));
    ui->pushButton_changePassword->setEnabled(true);
    return true;
}

void StudentWin::on_lineEdit_password_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void StudentWin::on_lineEdit_newPassword_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void StudentWin::on_lineEdit_passwordRepeat_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void StudentWin::on_pushButton_changePassword_clicked()
{
    QString oldP = ui->lineEdit_password->text();
    if(!cmpPassword(oldP)){
        QMessageBox::warning(this,tr("警告"), tr("密码不正确"));
        return;
    }
    if(changePassword(ui->lineEdit_newPassword->text())){
        QMessageBox::warning(this,tr("成功"), tr("密码修改成功"));
    }
    else{
        QMessageBox::warning(this,tr("失败"), tr("密码修改失败"));
    }
}
