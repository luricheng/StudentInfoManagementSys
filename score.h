#ifndef SCORE_H
#define SCORE_H

//#include <QObject>
#include<QString>

class Score
{
public:
    explicit Score(QString teacherId, QString studentId, QString courseId, int score);
    QString getCourseName(){
        return courseName;
    }
    QString getCourseId(){
        return courseId;
    }
    QString getStudentId(){
        return studentId;
    }
    int getScore(){
        return score;
    }
    QString getStudentName(){
        return studentName;
    }
    void setScore(int s){
        score = s;
    }

private:
    QString getCourseName(QString courseId);
    QString getStudentName(QString studentId);
    QString teacherId, studentId, courseId, courseName, studentName;
    int score;
};

#endif // SCORE_H
