#include <algorithm>
class ScreenHandler {

public:
    ScreenHandler(int currentScreenNumber, int maxScreenNumber, int currentSubScreenNumber, int maxSubScreenNumber);

    int getCurrentScreenNumber();

    int getMaxScreenNumber();

    void changeScreenRight();

    void changeScreenLeft();

    void changeSubScreen();

    int getCurrentSubScreenNumber();

private:
    int maxScreenNumber;
    int currentScreenNumber;

    int currentSubScreenNumber;
    int maxSubScreenNumber;

    

    

};