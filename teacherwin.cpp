#include "teacherwin.h"
#include "ui_teacherwin.h"

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

teacherWin::teacherWin(Account*account, QWidget *parent) :
    QWidget(parent),
    Account(*account),
    ui(new Ui::teacherWin)
{
    ui->setupUi(this);

    id = account->getId();

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);


    QSqlQuery query;

    // set info
    if(!query.exec("select id, name, age from teacher where id='" + id + "'")
            || !query.next()){
        qDebug()<<"query from database error"<<endl;
        return;
    }
    ui->label_id->setText(query.value(0).toString());
    ui->label_name->setText(query.value(1).toString());
    ui->label_age->setText(query.value(2).toString());

    // init scoreMap
    initScoreMap();

    // set course
    for(auto it = scoreMap.begin();it!=scoreMap.end();++it){
        ui->comboBox->addItem(it.key());
    }
}

void teacherWin::initScoreMap(){
    QSqlQuery query;
    query.exec("select * from score where teacher_id='" + id + "'");
    while(query.next()){
        //get a score item
        QString studentId = query.value(0).toString();
        QString teacherId = query.value(1).toString();
        QString courseId = query.value(2).toString();
        QString score = query.value(3).toString();
        Score scoreItem(teacherId,studentId, courseId, score.toInt());
        QString courseName = scoreItem.getCourseName();
        if(scoreMap.find(courseName)==scoreMap.end()){
            scoreMap[courseName] = new QLinkedList<Score>();
        }
        QLinkedList<Score>&list = *scoreMap[courseName];
        list.append(scoreItem);
    }
}

teacherWin::~teacherWin()
{
    delete ui;
}

void teacherWin::on_pushButton_exit_clicked()
{
    exit(0);
}

void teacherWin::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug()<<"switch course to: "<<arg1<<endl;

    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("课程号"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("课程名"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("学号"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("姓名"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("成绩"));
    QTableView *tableView = new QTableView();
    tableView->setModel(model);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    QLinkedList<Score>&ls = *scoreMap[arg1];

    int rowCnt = 0;
    for(Score&s : ls){
        model->setItem(rowCnt, 0, new QStandardItem(s.getCourseId()));
        model->setItem(rowCnt, 1, new QStandardItem(s.getCourseName()));
        model->setItem(rowCnt, 2, new QStandardItem(s.getStudentId()));
        model->setItem(rowCnt, 3, new QStandardItem(s.getStudentName()));
        model->setItem(rowCnt, 4, new QStandardItem(QString::number(s.getScore())));
        rowCnt+=1;
    }
    tableView->resizeColumnsToContents();
    ui->scrollArea->setWidget(tableView);
}



void teacherWin::on_pushButton_submit_clicked()
{
    QString sId = ui->lineEdit_student_id->text();
    QString cId = ui->lineEdit_course_id->text();
    int score = ui->spinBox_score->value();
    QSqlQuery query;
    query.prepare("select * from score where student_id=? and course_id=?");
    query.bindValue(0, sId);
    query.bindValue(1, cId);
    if(query.exec()&&query.next()){ // exist
        query.prepare("update score set score=? where student_id=? and course_id=?");
        query.bindValue(0, score);
        query.bindValue(1,sId);
        query.bindValue(2, cId);
    }
    else{
        query.prepare("insert into score value(?,?,?,?)");
        query.bindValue(0, sId);
        query.bindValue(1, this->id);
        query.bindValue(2, cId);
        query.bindValue(3, score);
    }
    bool ans = query.exec();
    qDebug()<<"update "<< (ans?"success":"failed") <<endl;
    ui->lineEdit_course_id->setText("");
    ui->lineEdit_student_id->setText("");
}

void teacherWin::on_pushButton_exit_2_clicked()
{
    exit(0);
}

void teacherWin::on_lineEdit_course_id_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() ||
            ui->lineEdit_student_id->text().isEmpty()){
        ui->pushButton_submit->setEnabled(false);
    }
    else{
        ui->pushButton_submit->setEnabled(true);
    }
}

void teacherWin::on_lineEdit_student_id_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() ||
            ui->lineEdit_course_id->text().isEmpty()){
        ui->pushButton_submit->setEnabled(false);
    }
    else{
        ui->pushButton_submit->setEnabled(true);
    }
}

void teacherWin::on_pushButton_update_clicked()
{
    scoreMap.clear();
    initScoreMap();
    on_comboBox_currentTextChanged(ui->comboBox->currentText());
}

bool teacherWin::checkPasswordInput(){
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

void teacherWin::on_lineEdit_password_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void teacherWin::on_lineEdit_newPassword_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void teacherWin::on_lineEdit_passwordRepeat_textChanged(const QString &arg1)
{
    checkPasswordInput();
}

void teacherWin::on_pushButton_changePassword_clicked()
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
