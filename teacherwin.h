#ifndef TEACHERWIN_H
#define TEACHERWIN_H

#include <QWidget>
#include<score.h>
#include<QMap>
#include<QLinkedList>
#include<QGridLayout>
#include<QPushButton>
#include<account.h>

class Score;

namespace Ui {
class teacherWin;
}

class teacherWin : public QWidget, public Account
{
    Q_OBJECT

public:
    explicit teacherWin(Account*account, QWidget *parent = 0);
    ~teacherWin();

private slots:
    void on_pushButton_exit_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_lineEdit_student_id_textChanged(const QString &arg1);

    void on_pushButton_submit_clicked();

    void on_pushButton_exit_2_clicked();

    void on_lineEdit_course_id_textChanged(const QString &arg1);

    void on_pushButton_update_clicked();

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_lineEdit_newPassword_textChanged(const QString &arg1);

    void on_lineEdit_passwordRepeat_textChanged(const QString &arg1);

    void on_pushButton_changePassword_clicked();

private:
    Ui::teacherWin *ui;
    QString id;
    QMap<QString, QLinkedList<Score>* >scoreMap;
    void initScoreMap();
    QMap<QPushButton*, std::pair<Score*, int> >btnMap;
    bool checkPasswordInput();
};

#endif // TEACHERWIN_H
