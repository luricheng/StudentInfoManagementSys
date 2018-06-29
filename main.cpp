#include <QApplication>
#include <account.h>
#include <login.h>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // connect database
    QSqlDatabase dataBase = QSqlDatabase::addDatabase("QMYSQL");
    dataBase.setHostName("localhost");
    dataBase.setUserName("root");
    dataBase.setPassword("88");
    dataBase.setDatabaseName("StudentInfo");
    if(!dataBase.open()){
        qDebug()<<"open database failed"<<endl;
        QMessageBox::warning(nullptr,"警告", "连接数据库失败");
        return 3306;
    }
    qDebug()<<"open database success"<<endl;

    Login login(nullptr, &dataBase);
    login.show();

    return a.exec();
}
