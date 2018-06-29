#include "adminwin.h"
#include "ui_adminwin.h"

#include<QSqlQuery>
#include<QDebug>
#include<QLabel>
#include<QLayout>
#include<QStandardItemModel>
#include<QTableView>
#include<QDesktopWidget>
#include<QScrollArea>
#include<QPushButton>
#include<QLineEdit>
#include<QSpinBox>
#include<QDialogButtonBox>
#include<QHeaderView>
#include<QComboBox>

AdminWin::AdminWin(Account*account, QWidget *parent) :
    QWidget(parent),
    Account(*account),
    ui(new Ui::AdminWin)
{
    ui->setupUi(this);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QSqlQuery query;

    // set info
    if(!query.exec("select id, name, age from admin where id='" + getId() + "'")
            || !query.next()){
        qDebug()<<"query from database error"<<endl;
        return;
    }
    ui->label_id->setText(query.value(0).toString());
    ui->lineEdit_name->setText(query.value(1).toString());
    ui->spinBox_age->setValue(query.value(2).toInt());

    ui->pushButton_saveInfo->setEnabled(false);

    showScoreList();
}

AdminWin::~AdminWin()
{
    delete ui;
}

void AdminWin::showScoreList(){
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("课程号"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("课程名"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("学号"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("姓名"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("任课老师职工号"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("任课老师姓名"));
    model->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("成绩"));
    QTableView *tableView = new QTableView();
    tableView->setModel(model);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    QSqlQuery query;
    query.exec("select *  from score");

    int rowCnt=0;
    while(query.next()){
        QString cID = query.value(2).toString();
        QString cName = Account::getCourseName(cID);
        QString sID = query.value(0).toString();
        QString sName = Account::getStudentName(sID);
        QString tID = query.value(1).toString();
        QString tName = Account::getTeacherName(tID);
        int score = query.value(3).toInt();
        model->setItem(rowCnt,0, new QStandardItem(cID));
        model->setItem(rowCnt,1, new QStandardItem(cName));
        model->setItem(rowCnt,2, new QStandardItem(sID));
        model->setItem(rowCnt,3, new QStandardItem(sName));
        model->setItem(rowCnt,4, new QStandardItem(tID));
        model->setItem(rowCnt,5, new QStandardItem(tName));
        model->setItem(rowCnt,6, new QStandardItem(QString::number(score)));
        rowCnt++;
    }

    tableView->resizeColumnsToContents();
    ui->scrollArea->setWidget(tableView);
}

void AdminWin::on_pushButton_3_clicked()
{
    exit(0);
}

void AdminWin::on_pushButton_clicked()
{
    QSqlQuery query;
    QString name = ui->lineEdit_name->text();
    int age = ui->spinBox_age->value();
    query.prepare("update admin set name=? where id=?");
    query.bindValue(0, name);
    query.bindValue(1, getId());
    query.exec();
    query.prepare("update admin set age=? where id=?");
    query.bindValue(0, age);
    query.bindValue(1, getId());
    query.exec();
}

void AdminWin::on_lineEdit_name_textChanged(const QString &arg1)
{
    ui->pushButton_saveInfo->setEnabled(!arg1.isEmpty());
}

void AdminWin::on_spinBox_age_valueChanged(int arg1)
{
    ui->pushButton_saveInfo->setEnabled(!ui->lineEdit_name->text().isEmpty());
}


void AdminWin::on_lineEdit_stuID_textChanged(const QString &arg1)
{
    QSqlQuery query;
    query.exec("select * from student where id='" + arg1 + "'");
    bool flag = true;
    if(query.next()){
        ui->lineEdit_stuName->setText(query.value(1).toString());
        ui->comboBox_stuSex->setCurrentIndex(query.value(2).toString()=='M'?0:1);
        ui->spinBox_stuAge->setValue(query.value(3).toInt());
        ui->spinBox_stuGrade->setValue(query.value(4).toInt());
        ui->lineEdit_stuMajor->setText(query.value(5).toString());
        ui->lineEdit_stuClass->setText(query.value(6).toString());
    }
    else{
        flag = false;

    }
    ui->lineEdit_stuName->setEnabled(flag);
    ui->lineEdit_stuClass->setEnabled(flag);
    ui->lineEdit_stuMajor->setEnabled(flag);
    ui->spinBox_stuAge->setEnabled(flag);
    ui->spinBox_stuGrade->setEnabled(flag);
    ui->comboBox_stuSex->setEnabled(flag);
}


void AdminWin::on_pushButton_saveStuInfo_clicked()
{
    QString id = ui->lineEdit_stuID->text();
    QString name = ui->lineEdit_stuName->text();
    QString sex = ((ui->comboBox_stuSex->currentIndex())==0?"M":"F");
    int age = ui->spinBox_stuAge->value();
    int grade  = ui->spinBox_stuGrade->value();
    QString _major = ui->lineEdit_stuMajor->text();
    QString _class = ui->lineEdit_stuClass->text();
    QSqlQuery query;
    query.prepare("update student set name=?, sex=?, age=?, grade=?, major=?, class=? where id=?");
    query.bindValue(0, name);
    query.bindValue(1, sex);
    query.bindValue(2, age);
    query.bindValue(3, grade);
    query.bindValue(4, _major);
    query.bindValue(5, _class);
    query.bindValue(6, id);
    query.exec();
}

void AdminWin::on_lineEdit_teaID_textChanged(const QString &arg1)
{
    QSqlQuery query;
    query.exec("select * from teacher where id='" + arg1 + "'");
    bool flag = true;
    if(query.next()){
        ui->lineEdit_teaName->setText(query.value(2).toString());
        ui->spinBox_teaAge->setValue(query.value(3).toInt());
    }
    else{
        flag = false;
    }
    ui->lineEdit_teaName->setEnabled(flag);
    ui->spinBox_teaAge->setEnabled(flag);
}

void AdminWin::on_pushButton_saveTeaInfo_clicked()
{
    QString id = ui->lineEdit_teaID->text();
    QString name = ui->lineEdit_teaName->text();
    int age = ui->spinBox_teaAge->value();
    QSqlQuery query;
    query.prepare("update teacher set name=?, age=? where id=?");
    query.bindValue(0, name);
    query.bindValue(1, age);
    query.bindValue(2, id);
    query.exec();
}

void AdminWin::scoreRecordEdit(){
    ui->pushButton_change->setEnabled(false);
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_del->setEnabled(false);
    QString sID = ui->lineEdit_editStuID->text();
    QString tID = ui->lineEdit_editTeaID->text();
    QString cID = ui->lineEdit_editCourseID->text();
    QSqlQuery query;
    query.prepare("select score from score where student_id=? and teacher_id=? and course_id=?");
    query.bindValue(0,sID);
    query.bindValue(1,tID);
    query.bindValue(2,cID);
    query.exec();
    if(query.next()){
        ui->pushButton_change->setEnabled(true);
        ui->pushButton_del->setEnabled(true);
        ui->spinBox_editScore->setValue(query.value(0).toInt());
    }
    else{
        if(!sID.isEmpty()&&
                !tID.isEmpty()&&
                !cID.isEmpty()){
            ui->pushButton_add->setEnabled(true);
        }
    }
}

void AdminWin::on_lineEdit_editCourseID_textChanged(const QString &arg1)
{
    scoreRecordEdit();
}

void AdminWin::on_lineEdit_editTeaID_textChanged(const QString &arg1)
{
    scoreRecordEdit();
}

void AdminWin::on_lineEdit_editStuID_textChanged(const QString &arg1)
{
    scoreRecordEdit();
}

void AdminWin::on_pushButton_add_clicked()
{
    QString sID = ui->lineEdit_editStuID->text();
    QString tID = ui->lineEdit_editTeaID->text();
    QString cID = ui->lineEdit_editCourseID->text();
    int s = ui->spinBox_editScore->value();
    QSqlQuery query;
    query.exec("insert into score value(?,?,?,?)");
    query.bindValue(0, sID);
    query.bindValue(1, tID);
    query.bindValue(2, cID);
    query.bindValue(3, s);
    query.exec();
}

void AdminWin::on_pushButton_change_clicked()
{
    QString sID = ui->lineEdit_editStuID->text();
    QString tID = ui->lineEdit_editTeaID->text();
    QString cID = ui->lineEdit_editCourseID->text();
    int s = ui->spinBox_editScore->value();
    QSqlQuery query;
    query.exec("update score set score=? where student_id=? and teacher_id=? and course_id=?");
    query.bindValue(0, s);
    query.bindValue(1, sID);
    query.bindValue(2, tID);
    query.bindValue(3, cID);
    query.exec();
}

void AdminWin::on_pushButton_del_clicked()
{
    QString sID = ui->lineEdit_editStuID->text();
    QString tID = ui->lineEdit_editTeaID->text();
    QString cID = ui->lineEdit_editCourseID->text();
    QSqlQuery query;
    query.exec("delete from score where student_id=? and teacher_id=? and course_id=?");
    query.bindValue(0, sID);
    query.bindValue(1, tID);
    query.bindValue(2, cID);
    query.exec();
}

void AdminWin::on_pushButton_update_clicked()
{
    showScoreList();
}

bool AdminWin::checkPasswordInput(){
    bool flag = ui->lineEdit_passwd->text().isEmpty() ||
            ui->lineEdit_newPasswd->text().isEmpty() ||
            ui->lineEdit_passwdRepeat->text().isEmpty();
    if(flag){
        ui->pushButton_savePassword->setEnabled(false);
        return false;
    }
    if(ui->lineEdit_newPasswd->text() !=
            ui->lineEdit_passwdRepeat->text()){
        ui->label_newPasswordTips->setText(tr("2次密码不相同"));
        ui->pushButton_savePassword->setEnabled(false);
        return false;
    }
    ui->label_newPasswordTips->setText(tr(""));
    ui->pushButton_savePassword->setEnabled(true);
    return true;
}

void AdminWin::on_lineEdit_passwd_textChanged(const QString &arg1)
{
    checkPasswordInput();
}


void AdminWin::on_lineEdit_newPasswd_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void AdminWin::on_lineEdit_passwdRepeat_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void AdminWin::on_pushButton_savePassword_clicked()
{
    QString oldP = ui->lineEdit_passwd->text();
    if(!cmpPassword(oldP)){
        QMessageBox::warning(this,tr("警告"), tr("密码不正确"));
        return;
    }
    if(changePassword(ui->lineEdit_newPasswd->text())){
        QMessageBox::warning(this,tr("成功"), tr("密码修改成功"));
    }
    else{
        QMessageBox::warning(this,tr("失败"), tr("密码修改失败"));
    }
}
