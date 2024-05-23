/**
 * @file newsScreen.cpp
 * @author Marcus Hagen
 */

#include "../include/newsScreen.h"

#include <string>
#include <sstream>

extern NetworkHandler networkHandler;

NewsScreen::NewsScreen() : updateFrequency(15 * 60 * 1000) {
    rssFeed = "http://feeds.bbci.co.uk/news/rss.xml";
    updateThread.start(callback(this, &NewsScreen::updateNews));
}

NewsScreen::~NewsScreen() { }

void NewsScreen::displayNewsScreen(DFRobot_RGBLCD1602 &lcd) {
    lcd.display();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Source: BBC");
    lcd.setCursor(0, 1);
    lcd.printf("%s", scrollingHeadlines.c_str());
    ThisThread::sleep_for(100ms);

    if (scrollingHeadlines.length() > 16) {
        scrollingHeadlines = scrollingHeadlines.substr(1) + scrollingHeadlines[0];
    } else {
        scrollingHeadlines = " " + scrollingHeadlines;
    }
}

void NewsScreen::updateNews() {
    while (true) {
        if (fetchNews()) {
            formatHeadlines();
        } else {
            scrollingHeadlines = "Error fetching news";
        }
        ThisThread::sleep_for(15min);
    }
}

bool NewsScreen::fetchNews() {
    if (!networkHandler.connect()) {
        return false;
    }
    TCPSocket socket;
    nsapi_error_t result = socket.open(networkHandler.getNetwork());
    if (result != 0) {
        return false;
    }
    result = socket.connect("feeds.bbci.co.uk", 80);
    if (result != 0) {
        return false;
    }

    char request[256];
    sprintf(request, "GET /news/rss.xml HTTP/1.1\r\nHost: feeds.bbci.co.uk\r\nConnection: close\r\n\r\n");
    socket.send(request, strlen(request));

    char buffer[1024];
    std::string response;
    int bytesRead;
    do {
        bytesRead = socket.recv(buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            response += buffer;
        }
    } while (bytesRead > 0);

    socket.close();

    size_t feedStart = response.find("<channel>");
    size_t feedEnd = response.find("</channel>");
    if (feedStart == std::string::npos || feedEnd == std::string::npos) {
        return false;
    }
    rssContent = response.substr(feedStart, feedEnd - feedStart + 9);
    return true;
}


void NewsScreen::formatHeadlines() {
    std::stringstream ss(rssContent);
    std::string line;
    int headlineCount = 0;
    scrollingHeadlines = "";

    while (std::getline(ss, line) && headlineCount < 3) {
        if (line.find("<title>") != std::string::npos) {
            size_t start = line.find("<title>") + 7;
            size_t end = line.find("</title>");
            if (start != std::string::npos && end != std::string::npos) {
                scrollingHeadlines += line.substr(start, end - start) + " -*- ";
                headlineCount++;
            }
        }
    }
}