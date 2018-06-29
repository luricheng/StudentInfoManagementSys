#include "account.h"

void Account::setId(QString id){
    this->id = id;
}

void Account::setType(int type){
    this->accountType = type;
}

bool Account::cmpPassword(QString passwd){
    passwd = getMd5(passwd);
    QSqlQuery query;
    query.exec("select password from " + db[getType() - 1] +
            " where id='" + getId() + "'");
    if(!query.next()){
        return false;
    }
    return query.value(0).toString()==passwd;
}

QString Account::getMd5(QString str){
    QByteArray byteArray;
    byteArray.append(str);
    QByteArray hashResult = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);
    QString result = hashResult.toHex();
    return result;
}

bool Account::changePassword(QString newPassword){
    QString passwordMd5 = getMd5(newPassword);

    QSqlQuery query;
    query.prepare("update " + db[getType() - 1] +
            " set password=? where id=?");
    query.bindValue(0, passwordMd5);
    query.bindValue(1, getId());
    return query.exec();
}
