#pragma once

#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include <mutex>
#include <thread>
#include <vector>

struct DisplayBuffers;
class Scene;

class Camera;
struct Triangle;
struct TracerParameters;

class RayTracer {
public:
    RayTracer(DisplayBuffers* buffers, int numThreads);
    ~RayTracer();

    void render(Scene* scene);

private:
    struct RayIntersectionResult {
        float u;
        float v;
        float w;
    };

    static const float epsilon;

    std::vector<std::thread*> tracers;

    static std::mutex threadCompleteMutex;
    static int completedThreads;
    static void threadComplete();
    static std::mutex beginWorkingMutex;
    static bool beginWorking;
    static Scene* scene;
    static std::mutex exitMutex;
    static bool exit;
    
    static void tracer(DisplayBuffers* buffers, const int startRow, const int numRows);
    static void traceRegion(DisplayBuffers* buffers, Scene* scene, const int startRow, const int numRows);
    static bool triangleRayIntersectionTest(const Camera* camera, const glm::vec3& direction, const Triangle* triangle, RayIntersectionResult& result);

    static bool shouldExit();
};

