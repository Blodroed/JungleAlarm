/**
 * @file newsScreen.h
 * @author Marcus Hagen
 */

#ifndef JUNGLEALARM_NEWSSCREEN_H
#define JUNGLEALARM_NEWSSCREEN_H

#include "mbed.h"
#include "DFRobot_RGBLCD1602.h"
#include "networkHandler.h"

class NewsScreen {
public:
    NewsScreen();
    ~NewsScreen();

    void displayNewsScreen(DFRobot_RGBLCD1602 &lcd);

private:
    Thread updateThread;
    int updateFrequency;
    std::string rssFeed;
    std::string rssContent;
    std::string scrollingHeadlines;

    bool fetchNews();
    void formatHeadlines();
};

#endif //JUNGLEALARM_NEWSSCREEN_H
