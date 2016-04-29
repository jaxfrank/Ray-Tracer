#include "RayTracer.h"

#include <thread>
#include <vector>
#include <chrono>

#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "Triangle.h"
#include "Renderer.h"

std::chrono::microseconds sleepInterval(10);

const float RayTracer::epsilon = 0.000001f;

std::mutex RayTracer::threadCompleteMutex;
int RayTracer::completedThreads = 0;

std::mutex RayTracer::beginWorkingMutex;
bool RayTracer::beginWorking;

std::mutex RayTracer::exitMutex;
bool RayTracer::exit = false;

Scene* RayTracer::scene;

RayTracer::RayTracer(DisplayBuffers* buffers, int numThreads) {
    for(int i = 0; i < numThreads; i++) {
        int startRow = i*(buffers->height / numThreads);
        int numRows = (buffers->height / numThreads);
        tracers.push_back(new std::thread(RayTracer::tracer, buffers, startRow, numRows));
    }
}

RayTracer::~RayTracer() {
    exitMutex.lock();
    exit = true;
    exitMutex.unlock();

    while(tracers.size() > 0) {
        tracers.back()->join();
        delete tracers.back();
        tracers.pop_back();
    }
}

void RayTracer::render(Scene* scene) {
    //sf::Clock timer;
    this->scene = scene;
    beginWorkingMutex.lock();
    beginWorking = true;
    beginWorkingMutex.unlock();

    while(true) {
        threadCompleteMutex.lock();
        bool finished = completedThreads == tracers.size();
        threadCompleteMutex.unlock();
        if(finished) break;
        std::this_thread::sleep_for(sleepInterval);
    }
    completedThreads = 0;
    beginWorkingMutex.lock();
    beginWorking = false;
    beginWorkingMutex.unlock();
    //std::cout << timer.getElapsedTime().asSeconds() << std::endl;
}

void RayTracer::tracer(DisplayBuffers* buffers, const int startRow, const int numRows) {
    bool prevBeginWorkng = false;
    while(true) {
        beginWorkingMutex.lock();
        bool begin = beginWorking;
        beginWorkingMutex.unlock();
        if(begin && !prevBeginWorkng) {
            traceRegion(buffers, scene, startRow, numRows);
            threadComplete();
        }
        prevBeginWorkng = begin;
        if(shouldExit()) {
            return;
        }
        std::this_thread::sleep_for(sleepInterval);
    }
}

void RayTracer::traceRegion(DisplayBuffers* buffers, Scene* scene, const int startRow, const int numRows) {
    float scale = scene->getCamera()->getScale();
    float aspectRatio = (float)buffers->width / (float)buffers->height;

    std::vector<Renderer*> renderers = scene->getRenderers();

    /* Rotate the rays?
    glm::vec3 from_vector;
    glm::vec3 to_vector;
    glm::normalize(from_vector);
    glm::normalize(to_vector);
    float cosa = glm::dot(from_vector,to_vector);
    glm::clamp(cosa,-1.0f,1.0f);
    glm::vec3 axis = glm::cross(from_vector,to_vector);
    float angle = glm::degrees(glm::acos(cosa));
    glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1),angle,axis);
    */

    for(int rendererIndex = 0; rendererIndex < renderers.size(); rendererIndex++) {
        Renderer* renderer = renderers[rendererIndex];
        for(int i = 0; i < buffers->width; i++) {
            for(int j = startRow; j < startRow + numRows; j++) {
                float x = (2 * (i + 0.5f) / buffers->width - 1) * aspectRatio * scale;
                float y = (1 - 2 * (j + 0.5f) / buffers->height) * scale;
                glm::vec3 dir(x, y, -1.0f);
                glm::normalize(dir);

                RayIntersectionResult result;

                for(int triangleIndex = 0; triangleIndex < renderer->getTriangleCount(); triangleIndex++) {
                    //Triangle& triangle = ;
                    if(triangleRayIntersectionTest(scene->getCamera(), dir, renderer->getTriangle(triangleIndex), result)) {
                        //TODO: Add check to see if this new interesection is closer to the camera than the value stored in the depth buffer
                        int index = (i + j * buffers->width) * 4;
                        /*triangle.a.color * result.u + triangle.b.color * result.v + triangle.c.color * result.w*/
                        glm::vec4 color = renderer->getColor(triangleIndex, result.u, result.v, result.w);
                        for(int k = 0; k < 4; k++) {
                            buffers->colorBuffer[index + k] = (sf::Uint8)(color[k] * 255.0f);
                        }
                    }
                }
            }
        }
    }
}

bool RayTracer::triangleRayIntersectionTest(const Camera* camera, const glm::vec3& direction, const Triangle* triangle, RayIntersectionResult& result) {
    glm::vec3 edgeAB = triangle->b - triangle->a;
    glm::vec3 edgeAC = triangle->c - triangle->a;
    glm::vec3 p = glm::cross(direction, edgeAC);
    float determinant = glm::dot(edgeAB, p);
    if(determinant > -epsilon && determinant < epsilon) return false;
    float inverseDeterminant = 1.0f / determinant;

    glm::vec3 T = camera->getPosition() - triangle->a;
    float u = glm::dot(T, p) * inverseDeterminant;

    if(u < 0.0f || u > 1.0f) return false;

    glm::vec3 q = glm::cross(T, edgeAB);
    float v = glm::dot(direction, q) * inverseDeterminant;

    if(v < 0.0f || u + v > 1.0f) return false;

    float t = glm::dot(edgeAC, q) * inverseDeterminant;
    if(t > epsilon) {
        result.u = u;
        result.v = v;
        result.w = 1 - u - v;
        return true;
    }

    return false;
}

void RayTracer::threadComplete() {
    std::lock_guard<std::mutex> lock(threadCompleteMutex);

    completedThreads++;
}

bool RayTracer::shouldExit() {
    std::lock_guard<std::mutex> lock(exitMutex);
    return exit;
}