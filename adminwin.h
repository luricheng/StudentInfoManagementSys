#ifndef ADMINWIN_H
#define ADMINWIN_H

#include <QWidget>
#include<account.h>

namespace Ui {
class AdminWin;
}

class AdminWin : public QWidget, public Account
{
    Q_OBJECT

public:
    explicit AdminWin(Account*account, QWidget *parent = 0);
    ~AdminWin();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_name_textChanged(const QString &arg1);

    void on_spinBox_age_valueChanged(int arg1);

    void on_lineEdit_stuID_textChanged(const QString &arg1);

    void on_pushButton_saveStuInfo_clicked();

    void on_lineEdit_teaID_textChanged(const QString &arg1);

    void on_pushButton_saveTeaInfo_clicked();

    void on_lineEdit_editCourseID_textChanged(const QString &arg1);

    void on_lineEdit_editTeaID_textChanged(const QString &arg1);

    void on_lineEdit_editStuID_textChanged(const QString &arg1);

    void on_pushButton_add_clicked();

    void on_pushButton_change_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_lineEdit_passwd_textChanged(const QString &arg1);

    void on_lineEdit_newPasswd_textChanged(const QString &arg1);

    void on_lineEdit_passwdRepeat_textChanged(const QString &arg1);

    void on_pushButton_savePassword_clicked();

private:
    Ui::AdminWin *ui;
    void scoreRecordEdit();
    void showScoreList();
    bool checkPasswordInput();
};

#endif // ADMINWIN_H
