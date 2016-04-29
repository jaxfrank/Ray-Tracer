#pragma once

#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include <mutex>
#include <thread>

struct DisplayBuffers;
class Scene;

class Camera;
struct Triangle;
struct TracerParameters;

class RayTracer {
public:
    static void render(DisplayBuffers* buffers, Scene* scene);

    static int getNumThreads() { return numThreads; }
    static void setNumThreads(int numThreads) { RayTracer::numThreads = numThreads; }

private:
    RayTracer() {}
    ~RayTracer() {}

    struct RayIntersectionResult {
        float u;
        float v;
        float w;
    };

    static glm::vec3* screenRays;

    static const float epsilon;

    static int numThreads;

    static void traceRegion(DisplayBuffers* buffers, Scene* scene, const int startRow, const int numRows);
    static bool triangleRayIntersectionTest(const Camera* camera, const glm::vec3& direction, const Triangle* triangle, RayIntersectionResult& result);

    
};

