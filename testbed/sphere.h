#ifndef VEEKAY_SPHERE_H
#define VEEKAY_SPHERE_H

#include <vector>
#include <cstdint>

struct Vector {
    float x, y, z;
};
struct Vertex {
    Vector position;
    Vertex(float x, float y, float z) : position{x, y, z} {}
};

struct SpherePoint {
    float theta;    // Z angle
    float phi;      // XY angle
    float sinTheta;
    float cosTheta;
};

class SphereMesh {
    std::vector<SpherePoint> basePoints;
    std::vector<Vertex> currentVertices;
    std::vector<uint32_t> indices;
    float currentRadius;

    void updateVertices();

public:
    explicit SphereMesh(int targetVertices, float radius = 1.0f);

    void changeRadius(float newRadius);

    [[nodiscard]] std::vector<Vertex>& getVertices();
    [[nodiscard]] std::vector<uint32_t>& getIndices();
    [[nodiscard]] float getRadius() const;
};
#endif