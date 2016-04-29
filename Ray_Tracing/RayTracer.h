#pragma once

#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include <mutex>
#include <thread>
#include <vector>

struct DisplayBuffers;
class Scene;

class Camera;
class Renderer;
struct Triangle;
struct TracerParameters;

class RayTracer {
public:
    RayTracer(DisplayBuffers* buffers, int numThreads);
    ~RayTracer();

    void render(Scene* scene);
    
    static void enableDepthTest(bool enabled) { doDepthTest = enabled; }

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

    static bool doDepthTest;
    
    static void tracer(DisplayBuffers* buffers, const int startRow, const int numRows);
    static void traceRegion(DisplayBuffers* buffers, Scene* scene, const int startRow, const int numRows);
    static bool triangleRayIntersectionTest(const Camera* camera, const glm::vec3& direction, const Triangle* triangle, RayIntersectionResult& result);
    inline static void updateColorBuffer(DisplayBuffers* buffers, Renderer* renderer, RayIntersectionResult& result, int index);

    static bool shouldExit();
};

