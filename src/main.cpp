#include <QApplication>
#include "ui/LoginWindow.hpp"

// Temporary demo launch for Jana's 4 features.
// Not wired into LoginWindow/dashboards yet - opened directly here for now.
#include "ui/FeedbackWindow.hpp"
#include "ui/MessagingWindow.hpp"
#include "ui/StatisticsWindow.hpp"
#include "ui/RewardWindow.hpp"
#include "models/User.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.show();

    User demoUser;
    demoUser.username = "demoUser";
    demoUser.role     = "Student";

    FeedbackWindow* feedbackWindow = new FeedbackWindow(demoUser);
    feedbackWindow->show();

    MessagingWindow* messagingWindow = new MessagingWindow(demoUser);
    messagingWindow->show();

    StatisticsWindow* statisticsWindow = new StatisticsWindow();
    statisticsWindow->show();

    RewardWindow* rewardWindow = new RewardWindow(demoUser);
    rewardWindow->show();

    return app.exec();
}
