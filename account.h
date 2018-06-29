#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QSqlQuery>
#include <QVariant>
#include<QCryptographicHash>
#include<QMessageBox>

class Account// : public QObject
{
//    Q_OBJECT
public:
    Account(QString id, int type){
        setId(id);
        setType(type);
    }
    const QString getId()const {
        return id;
    }
    void setId(QString id);

    const int getType()const {
        return accountType;
    }
    void setType(int type);

    static QString getCourseName(QString courseId){
        QSqlQuery query;
        if(query.exec("select name from course where id='" + courseId + "'")
                &&query.next()){
            return query.value(0).toString();
        }
        return "error";
    }

    static QString getTeacherName(QString teacherId){
        QSqlQuery query;
        if(query.exec("select name from teacher where id='" + teacherId + "'")
                &&query.next()){
            return query.value(0).toString();
        }
        return "error";
    }

    static QString getStudentName(QString studentId){
        QSqlQuery query;
        if(query.exec("select name from student where id='" + studentId + "'")
                &&query.next()){
            return query.value(0).toString();
        }
        return "error";
    }

    static QString getMd5(QString str);


//signals:

//public slots:

protected:
    bool cmpPassword(QString passwd);
    bool changePassword(QString newPassword);

private:
    QString id;
    int accountType;
    QString db[3] = {"student", "teacher", "admin"};
};

#endif // ACCOUNT_H
