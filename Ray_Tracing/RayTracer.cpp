#include "RayTracer.h"

#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>

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

bool RayTracer::doDepthTest = false;

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
        #ifdef ANIMATE_RAY_TRACE
        Window::globalWindow->displayBuffers();
        #endif
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

    // Rotate the rays?
    glm::vec3 negZ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 forward = glm::normalize(scene->getCamera()->getForward());
    float cosa = glm::dot(negZ, forward);
    glm::clamp(cosa,-1.0f,1.0f);
    glm::vec3 axis = glm::cross(negZ, forward);
    float angle = glm::acos(cosa);
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1), angle, axis);
    
    std::vector<Renderer*> renderers = scene->getRenderers();
    for(int rendererIndex = 0; rendererIndex < renderers.size(); rendererIndex++) {
        Renderer* renderer = renderers[rendererIndex];
        for(int i = 0; i < buffers->width; i++) {
            for(int j = startRow; j < startRow + numRows; j++) {
                float x = (2 * (i + 0.5f) / buffers->width - 1) * aspectRatio * scale;
                float y = (1 - 2 * (j + 0.5f) / buffers->height) * scale;
                glm::vec4 dir4(x, y, -1.0f, 0.0f);
                glm::normalize(dir4);
                dir4 = rotateMatrix * dir4;
                glm::vec3 dir = glm::normalize(glm::vec3(dir4.x, dir4.y, dir4.z));

                RayIntersectionResult result;
                for(int triangleIndex = 0; triangleIndex < renderer->getTriangleCount(); triangleIndex++) {
                    Triangle* triangle = renderer->getTriangle(triangleIndex);
                    if(triangleRayIntersectionTest(scene->getCamera(), dir, triangle, result)) {
                        int index = i + j * buffers->width;
                        if(doDepthTest) {
                            glm::vec3 intersectPosition = result.u*triangle->a + result.v*triangle->b + result.w*triangle->c;
                            float distance = glm::length(intersectPosition - scene->getCamera()->getPosition());

                            if(distance < buffers->depthBuffer[index]) {
                                buffers->depthBuffer[index] = distance;
                                updateColorBuffer(buffers, renderer, result, index);
                            }
                        } else {
                            updateColorBuffer(buffers, renderer, result, index);
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

inline void RayTracer::updateColorBuffer(DisplayBuffers* buffers, Renderer* renderer, RayIntersectionResult& result, int index) {
    glm::vec4 color = renderer->getColor(index, result.u, result.v, result.w);
    for(int k = 0; k < 4; k++) {
        buffers->colorBuffer[index*4 + k] = (sf::Uint8)(color[k] * 255.0f);
    }
}

void RayTracer::threadComplete() {
    std::lock_guard<std::mutex> lock(threadCompleteMutex);
    completedThreads++;
}

bool RayTracer::shouldExit() {
    std::lock_guard<std::mutex> lock(exitMutex);
    return exit;
}