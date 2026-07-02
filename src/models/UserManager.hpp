#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP
#include <QVector>
#include <QString>
#include "User.hpp"


struct UserAccount {
    QString username;
    QString password;
    QString role;
};

class UserManager
{
private:
    QVector<UserAccount> accounts;

public:
    // a bunch of functions declarations for user registration
   

 bool registerUser(const QString& username,  //thsi function should register the user asking them for info and returning false if empty fields or already taken
                      const QString& password,
                      const QString& role);

    bool login(const QString& username,   //this one is for logging in to an account  false if username not found or password wrong and fills outuser with info of user on success

               const QString& password,
               User& outUser);

    bool usernameExists(const QString& username) const; //helper function to check if username already exists helps the registeruser function
};

#endif
