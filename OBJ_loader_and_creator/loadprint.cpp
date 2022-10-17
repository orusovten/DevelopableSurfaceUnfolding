// Example 1: Load and Print
//
// Load the data from the "name".obj then print it`s unfolded version into a file
//	called "name_unfold".obj

// Iostream - STD I/O Library
#include <iostream>

// fStream - STD File I/O Library
#include <fstream>

// OBJ_Loader - .obj Loader
#include "OBJ_Load.h"

// Main function
int main(int argc, char* argv[])
{
	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	Loader.LoadFile("cone.obj");

	// Create/Open .obj file with unfolded version
	std::ofstream file("cone_unfold.obj");

	if (!Loader.strMaterialPath.empty()) {
		file << Loader.strMaterialPath << '\n';
	}
	if (!Loader.strMesh.empty()) {
		file << Loader.strMesh << '\n';
	}
	for (const objl::Vector3& vertex : Loader.LoadedVertices) {
		file << "v " << vertex.X << " " << vertex.Y << " " << vertex.Z << '\n';
	}
	for (const std::string& text : Loader.strTextures) {
		file << text << '\n';
	}
	for (const std::string& text : Loader.strNormals) {
		file << text << '\n';
	}
	file << Loader.strMaterial << '\n';
	if (!Loader.strSmoothShading.empty()) {
		file << Loader.strSmoothShading << '\n';
	}
	for (const objl::Triangle& triangle : Loader.LoadedTriangles) {
		file << triangle.strVersion << '\n';
	}
  
  // Close the file
	file.close();


	// Exit the program
	return 0;
}
