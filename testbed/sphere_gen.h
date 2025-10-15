#ifndef VEEKAY_SPHERE_GEN_H
#define VEEKAY_SPHERE_GEN_H

#include <cstdint>
#include <cmath>
#include <vector>

struct Vector {
    float x, y, z;
};

struct Vertex {
    Vector position;
    // NOTE: You can add more attributes
};


constexpr std::pair<std::vector<Vertex>, std::vector<uint32_t>> sphereGen(const float radius = 1, const int latitudes = 8, const int longitudes = 13) {

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    constexpr auto PI = M_PI;
    const float deltaLatitude = PI / latitudes;
    const float deltaLongitude = 2 * PI / longitudes;

    for (int i = 0; i <= latitudes; ++i) {
        float latitudeAngle = PI / 2 - i * deltaLatitude;
        float xy = radius * cosf(latitudeAngle);
        float z = radius * sinf(latitudeAngle);

        for (int j = 0; j <= longitudes; ++j) {
            float longitudeAngle = j * deltaLongitude;
            float x = xy * cosf(longitudeAngle);
            float y = xy * sinf(longitudeAngle);
            vertices.push_back({x, y, z});
        }
    }

    for (int i = 0; i < latitudes; ++i) {
        for (int j = 0; j < longitudes; ++j) {
            uint32_t first = i * (longitudes + 1) + j;
            uint32_t second = first + longitudes + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }

    return std::make_pair(vertices, indices);
}
#endif
