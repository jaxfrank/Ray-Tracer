#pragma once
class Time {
public:
    static void setDeltaTime(float deltaTime) { Time::deltaTime = deltaTime; };
    static float getDelta() { return deltaTime; };

private:
    Time();
    ~Time();

    static float deltaTime;
};

