#include "OBJParser.h"


OBJParser::OBJParser(){
	maxX = minX = maxY = minY = maxZ = minZ = 0.0f;
}
OBJParser::~OBJParser(){}
using namespace std;



void OBJParser::loadFile(const char* filename){ 
	ifstream myFile(filename);
	if (!myFile.is_open()) {
		cout << "Can't find file " << filename << endl;
		return;
	}
	char dataline[1000];
	char datacopy[1000];
	char* firstToken;
	int numVertices, numNormals, numTexCoords, numFaces;
	numVertices = numNormals = numTexCoords = numFaces = 0;
	std::cout << "Parsing " << filename << endl;
	// PASS 1
	while (!myFile.eof()) {
		myFile.getline(dataline, 1000);
		strcpy(datacopy, dataline);
		
		if (strlen(dataline)>0) {
			firstToken = strtok(dataline, " ");
		}
		else {
			continue;
		}
		// Comment in the OBJ file
		if (dataline[0] == '#'){
			std::cout << dataline << endl;
		}
		else if (strcmp(firstToken, "v") == 0) {
			addVertices(datacopy);
			numVertices++;
		}
		else if (strcmp(firstToken, "vt") == 0) {
			//printf ("This line is a texture coordinate\n");
			numTexCoords++;
		}
		else if (strcmp(firstToken, "vn") == 0) {
			//printf ("This line is a vertex normal\n");
			numNormals++;
		}
		else if (strcmp(firstToken, "f") == 0) {
			//printf ("This line is an index face\n");
			addIndices(datacopy);
			numFaces++;
		}
		else {
			printf("I am awesome.\n");
		}
	}
	//printVertices();

	std::cout << "Verts: " << numVertices << endl;
	std::cout << "Normals: " << numNormals << endl;
	std::cout << "TexCoords: " << numTexCoords << endl;
	std::cout << "Faces: " << numFaces << "::" << faces.size() << endl;

	float xScale = (maxX - minX);
	float yScale = (maxY - minY);
	float zScale = (maxZ - minZ);
	scaleFactor = xScale > yScale ? xScale : yScale;
	scaleFactor = scaleFactor > zScale ? scaleFactor : zScale;
	scaleFactor = 1.0f / scaleFactor;

	std::cout << "X scale is " << xScale <<":"<<minX<<":"<<maxX << endl;
	std::cout << "Y scale is " << yScale << ":" << minY << ":" << maxY << endl;
	std::cout << "Z scale is " << zScale << ":" << minZ << ":" << maxZ << endl;
	std::cout << "Scale fa is " << scaleFactor << endl;
	xOffset = ((maxX+minX) / 2) / xScale;
	yOffset = ((maxY+minY) / 2) / yScale;
	zOffset = ((maxZ+minZ) / 2) / zScale;
	cout << "xO " << xOffset << endl;
	cout << "yO " << yOffset << endl;
	cout << "zO " << zOffset << endl;

	// Start PASS 2 here
}

void OBJParser::addIndices(char data[]){
	char* vTok = strtok(data, " ");
	char* x = strtok(NULL, " ");
	char* y = strtok(NULL, " ");
	char* z = strtok(NULL, " ");
	float realX = (float)atof(x)-1;
	float realY = (float)atof(y)-1;
	float realZ = (float)atof(z)-1;
	faces.push_back(realX);
	faces.push_back(realY);
	faces.push_back(realZ);
}

void OBJParser::addVertices(char data[]){
	char* vTok = strtok(data, " ");
	char* x = strtok(NULL, " ");
	char* y = strtok(NULL, " ");
	char* z = strtok(NULL, " ");
	float realX = (float)atof(x);
	if (realX > maxX) maxX = realX;
	if (realX < minX) minX = realX;
	
	float realY = (float)atof(y);
	if (realY > maxY) maxY = realY;
	if (realY < minY) minY = realY;

	float realZ = (float)atof(z);
	if (realZ > maxZ) maxZ = realZ;
	if (realZ < minZ) minZ = realZ;

	vertices.push_back(realX);
	vertices.push_back(realY);
	vertices.push_back(realZ);
}

void OBJParser::printVertices(){
	for (int i = 0; i < vertices.size(); i++) {
		std::cout << vertices[i] << " ";
		if (i % 3 == 0) {
			std::cout << endl;
		}
	}
	std::cout << endl;
}