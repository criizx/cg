#include <cmath>

#include "sphere.h"

constexpr std::pair<int, int> calculateSphereParameters(int targetVertices) {
	if (targetVertices < 12) {
		return {4, 2};
	}

	int best_sector = 4;
	int best_stack = 3;
	int best_diff = std::abs((best_sector + 1) * (best_stack + 1) - targetVertices);
	float best_quality = 0.0f;

	for (int sector = 4; sector <= 128; ++sector) {
		for (int stack = 3; stack <= 64; ++stack) {
			float ratio = static_cast<float>(sector) / stack;
			if (ratio < 0.3f || ratio > 3.0f) {
				continue;
			}
			int vertices_count = (sector + 1) * (stack + 1);
			int diff = std::abs(vertices_count - targetVertices);
			float quality = 1.0f / (diff + 1) * (1.0f - std::abs(ratio - 1.0f) * 0.5f);

			if (quality > best_quality) {
				best_quality = quality;
				best_sector = sector;
				best_stack = stack;
			}
		}
	}
	return {best_sector, best_stack};
}

std::vector<uint32_t> generateSphereIndices(const int sectorCount,const int stackCount) {
	std::vector<uint32_t> indices;
	int k1, k2;

	for(int i = 0; i < stackCount; ++i) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if(i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if(i != (stackCount-1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	return indices;
}

std::vector<SpherePoint> generateSphereBasePoints(const int sectorCount, const int stackCount) {
	std::vector<SpherePoint> basePoints;
	constexpr double PI = M_PI;

	const float sectorStep = 2 * PI / sectorCount;
	const float stackStep = PI / stackCount;

	for (int i = 0; i <= stackCount; ++i) {
		const float stackAngle = PI / 2 - i * stackStep;
		const float sinTheta = std::sin(stackAngle);
		const float cosTheta = std::cos(stackAngle);

		for (int j = 0; j <= sectorCount; ++j) {
			const float sectorAngle = j * sectorStep;
			basePoints.push_back({stackAngle, sectorAngle, sinTheta, cosTheta});
		}
	}
	return basePoints;
}

void SphereMesh::updateVertices() {
	currentVertices.clear();
	currentVertices.reserve(basePoints.size());

	for (const auto& point : basePoints) {
		float x = currentRadius * point.sinTheta * std::cos(point.phi);
		float y = currentRadius * point.sinTheta * std::sin(point.phi);
		float z = currentRadius * point.cosTheta;
		currentVertices.emplace_back(x, y, z);
	}

}


void SphereMesh::changeRadius(const float newRadius) {
	if (std::abs(currentRadius - newRadius) < 1e-6f) {
		return;
	}

	currentRadius = newRadius;
	updateVertices();
}

SphereMesh::SphereMesh(const int targetVertices, const float radius) : currentRadius(radius) {
	auto [sectors, stacks] = calculateSphereParameters(targetVertices);

	basePoints = generateSphereBasePoints(sectors, stacks);

	indices = generateSphereIndices(sectors, stacks);
	currentRadius = radius;

	changeRadius(radius);
}

std::vector<Vertex>& SphereMesh::getVertices() {
	return currentVertices;
}

std::vector<uint32_t>& SphereMesh::getIndices() {
	return indices;
}

float SphereMesh::getRadius() const {
	return currentRadius;
}
