#include <gtest/gtest.h>
#include "UserManager.hpp"
#include "User.hpp"

TEST(UserManagerTest, RegisterNewUserSucceeds)
{
    UserManager manager;
    bool success = manager.registerUser("jana", "password123", "Student");
    EXPECT_TRUE(success);
}

TEST(UserManagerTest, RegisterDuplicateUsernameFails)
{
    UserManager manager;
    manager.registerUser("jana", "password123", "Student");
    bool success = manager.registerUser("jana", "differentPassword", "Student");
    EXPECT_FALSE(success);
}

TEST(UserManagerTest, RegisterWithEmptyFieldsFails)
{
    UserManager manager;
    EXPECT_FALSE(manager.registerUser("", "password123", "Student"));
    EXPECT_FALSE(manager.registerUser("jana", "", "Student"));
}

TEST(UserManagerTest, UsernameExistsReflectsRegisteredUsers)
{
    UserManager manager;
    EXPECT_FALSE(manager.usernameExists("jana"));
    manager.registerUser("jana", "password123", "Student");
    EXPECT_TRUE(manager.usernameExists("jana"));
}

TEST(UserManagerTest, LoginWithCorrectCredentialsSucceeds)
{
    UserManager manager;
    manager.registerUser("jana", "password123", "Student");

    User outUser;
    bool success = manager.login("jana", "password123", outUser);

    EXPECT_TRUE(success);
    EXPECT_EQ(outUser.username, "jana");
    EXPECT_EQ(outUser.role, "Student");
}

TEST(UserManagerTest, LoginWithWrongPasswordFails)
{
    UserManager manager;
    manager.registerUser("jana", "password123", "Student");

    User outUser;
    bool success = manager.login("jana", "wrongPassword", outUser);

    EXPECT_FALSE(success);
}

TEST(UserManagerTest, LoginWithUnknownUsernameFails)
{
    UserManager manager;
    User outUser;
    bool success = manager.login("ghost", "password123", outUser);

    EXPECT_FALSE(success);
}
