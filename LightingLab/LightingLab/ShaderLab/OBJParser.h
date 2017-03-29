#ifndef OBJ_PARSER
#define OBJ_PARSER
#include <vector>
#include <iostream>
#include <fstream>


class OBJParser {
public:
	OBJParser();
	~OBJParser();
	void loadFile(const char* file);

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<int> faces;

	float scaleFactor;
	float xOffset;
	float yOffset;
	float zOffset;

private:
	float minX, maxX, minY, maxY, minZ, maxZ;
	void addVertices(char data[]);
	void addIndices(char data[]);
	void printVertices();
};
#endif