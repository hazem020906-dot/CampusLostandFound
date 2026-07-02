#include "UserManager.hpp"

bool UserManager::usernameExists(const QString& username) const
{
    for (const UserAccount& account : accounts)// for each account in QVector  accounts
    {
        if (account.username == username)// check if username exists
            return true;
    }
    return false;
}

bool UserManager::registerUser(const QString& username,
                                const QString& password,
                                const QString& role)
{
    if (username.isEmpty() || password.isEmpty())
        return false;

    if (usernameExists(username))// calls the userexists function here
        return false;

    UserAccount account;
    account.username = username;
    account.password = password;
    account.role     = role;

    accounts.push_back(account);// vector
    return true;
}

bool UserManager::login(const QString& username,
                         const QString& password,
                         User& outUser)
{
    for (const UserAccount& account : accounts)
    {
      if (account.username == username && account.password == password)//allowing entry to account iff username &&&& passwd are both correct
        {
            outUser.username = account.username;//outuser is a ui reference
            outUser.role     = account.role;
            return true;
        }
    }
    return false;
}
