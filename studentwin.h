#ifndef STUDENTWIN_H
#define STUDENTWIN_H

#include <QWidget>
#include<account.h>
namespace Ui {
class StudentWin;
}

class StudentWin : public QWidget, public Account
{
    Q_OBJECT

public:
    explicit StudentWin(Account* account, QWidget *parent = 0);
    ~StudentWin();

private slots:
    void on_pushButton_exit_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_lineEdit_newPassword_textChanged(const QString &arg1);

    void on_lineEdit_passwordRepeat_textChanged(const QString &arg1);

    void on_pushButton_changePassword_clicked();

private:
    Ui::StudentWin *ui;
    QString getCourseName(QString courseId);
    QString getTeacherName(QString teacherId);
    QString id, name, age, sex, _major, _class;
    int grade;
    bool checkPasswordInput();
};

#endif // STUDENTWIN_H
