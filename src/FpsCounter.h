class FpsCounter {

public:
    FpsCounter() {
        lastTickTime = 0;
        fpsCounter = 0;
        currentFps = 0;
        timeCounter = 0;
    }

    double onFrame();

    double getFps();

private:
    double lastTickTime;

    double currentFps;
    int fpsCounter;

    double timeCounter;
};
