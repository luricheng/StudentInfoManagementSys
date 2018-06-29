#include "score.h"
#include<QSqlQuery>
#include<QVariant>

Score::Score(QString teacherId, QString studentId, QString courseId, int score):
    teacherId(teacherId),
    studentId(studentId),
    courseId(courseId),
    score(score)
{
    courseName = getCourseName(courseId);
    studentName = getStudentName(studentId);
}

QString Score::getCourseName(QString courseId){
    QSqlQuery query;
    if(query.exec("select name from course where id='" + courseId + "'")
            &&query.next()){
        return query.value(0).toString();
    }
    return "error";
}

QString Score::getStudentName(QString studentId){
    QSqlQuery query;
    if(query.exec("select name from student where id='" + studentId + "'")
            &&query.next()){
        return query.value(0).toString();
    }
    return "error";
}

//Score& Score::operator=(const Score&other){
//    teacherId = other.teacherId;
//    studentId = other.studentId;
//    courseId = other.courseId;
//    courseName = other.courseName;
//    score = other.score;
//    return *this;
//}
