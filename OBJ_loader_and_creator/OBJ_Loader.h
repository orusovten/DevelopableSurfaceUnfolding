// OBJ_Loader.h - A Single Header OBJ Model Loader

#pragma once

// Iostream - STD I/O Library
#include <iostream>

// Vector - STD Vector/Array Library
#include <vector>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

// Cmath - STD math Library
#include <cmath>

// Assert Library
#include <cassert>

#include <set>

// Namespace: OBJL
//
// Description: The namespace that holds everything that
//	is needed and used for the OBJ Model Loader
namespace objl
{
    // Structure: Vector3
	// Description: Vector in 3d-dimensional form.
	// 	Also can be used as point position or normal in this code.
	struct Vector3
	{
		// Default Constructor
		Vector3(): X(0), Y(0), Z(0) {}
		// Variable Set Constructor
		Vector3(double X_, double Y_, double Z_): X(X_), Y(Y_), Z(Z_) {}
		// Bool Equals Operator Overload
		bool operator==(const Vector3& other) const
		{
			return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector3& other) const
		{
			return !(*this == other);
		}
		// Addition Operator Overload
		Vector3 operator+(const Vector3& right) const
		{
			return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
		}
		// Subtraction Operator Overload
		Vector3 operator-(const Vector3& right) const
		{
			return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
		}
		// double Multiplication Operator Overload
		Vector3 operator*(const double& other) const
		{
			return Vector3(this->X * other, this->Y * other, this->Z * other);
		}
		// double Division Operator Overload
		Vector3 operator/(const double& other) const
		{
			return Vector3(this->X / other, this->Y / other, this->Z / other);
		}

		// Positional Variables
		double X;
		double Y;
		double Z;
	};

	// structure: Triangle
	// Description: Stores the indexes of point positions
	//  Must be 3 indexes
	struct Triangle {
		std::set<int> array;
		std::string strVersion;
	};

    // Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//  functions that is needed for OBJL
	namespace math
	{
		// Vector3 Cross Product
		Vector3 CrossV3(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}

		// Vector3 Magnitude Calculation
		double MagnitudeV3(const Vector3& in)
		{
			return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
		}

		// Vector3 DotProduct
		double DotV3(const Vector3& a, const Vector3& b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		// Angle between 2 Vector3 Objects
		double AngleBetweenV3(const Vector3& a, const Vector3& b)
		{
			double angle = DotV3(a, b);
			angle /= (MagnitudeV3(a) * MagnitudeV3(b));
			return angle = acosf(angle);
		}

		// Projection Calculation of a onto b
		Vector3 ProjV3(const Vector3& a, const Vector3& b)
		{
			Vector3 bn = b / MagnitudeV3(b);
			return bn * DotV3(a, bn);
		}
	}

    // Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// Vector3 Multiplication Operator Overload
		Vector3 operator*(const double& left, const Vector3& right)
		{
			return Vector3(right.X * left, right.Y * left, right.Z * left);
		}

		// A test to see if P1 is on the same side as P2 of a line segment ab
		bool SameSide(const Vector3& p1, const Vector3& p2, const Vector3& a, const Vector3& b)
		{
			Vector3 cp1 = math::CrossV3(b - a, p1 - a);
			Vector3 cp2 = math::CrossV3(b - a, p2 - a);

			return math::DotV3(cp1, cp2) >= 0;
		}

		// Generate a cross product normal for a triangle
		Vector3 GenTriNormal(const Vector3& t1, const Vector3& t2, const Vector3& t3)
		{
			Vector3 u = t2 - t1;
			Vector3 v = t3 - t1;
			Vector3 normal = math::CrossV3(u,v);

			return normal;
		}

		// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
		bool inTriangle(const Vector3& point, const Vector3& tri1, const Vector3& tri2, const Vector3& tri3)
		{
			// Test to see if it is within an infinite prism that the triangle outlines.
			bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
				&& SameSide(point, tri3, tri1, tri2);

			// If it isn't it will never be on the triangle
			if (!within_tri_prisim)
				return false;

			// Calulate Triangle's Normal
			Vector3 n = GenTriNormal(tri1, tri2, tri3);

			// Project the point onto this normal
			Vector3 proj = math::ProjV3(point, n);

			// If the distance from the triangle to the point is 0
			//	it lies on the triangle
			if (math::MagnitudeV3(proj) == 0)
				return true;
			else
				return false;
		}

		// Split a String into a string array at a given token
		inline void split(const std::string &in,
			std::vector<std::string> &out,
			const std::string& token)
		{
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string &in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string &in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get index in elements array from string index version
		template <class T>
		inline int getIndex(const std::vector<T> &elements, const std::string &index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return idx;
		}

		// Get element at given index position from string index version
		template <class T>
		inline const T & getElement(const std::vector<T> &elements, const std::string &index)
		{
			return elements[getIndex(elements, index)];
		}
	}
    
		// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader
	{
	public:
		Loader() {}
		~Loader() {}

		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found
		// or unable to be loaded return false
		void LoadFile(const std::string& Path)
		{
			// If the file is not an .obj file return false
			assert((".obj file format only", Path.substr(Path.size() - 4, 4) == ".obj"));

			std::ifstream file(Path);
			assert(("file can`t be opened, oops :(((", file.is_open()));

			strMesh.clear();
			strMaterial.clear();
			strMaterialPath.clear();
			strSmoothShading.clear();

			LoadedVertices.clear();
			LoadedTriangles.clear();

			strNormals.clear();
			strTextures.clear();

			std::string curline;
			while (std::getline(file, curline))
			{
				if (algorithm::firstToken(curline) == "g" || curline[0] == 'g') {
					assert(("no groups of objects are allowed", true));
				}
				else if (algorithm::firstToken(curline) == "o")
				{
					assert(("objects must be 0 or 1 times", strMesh.empty()));
					strMesh = curline;
				}
				// Generate a Vertex Position
				else if (algorithm::firstToken(curline) == "v")
				{
					std::vector<std::string> spos;
					algorithm::split(algorithm::tail(curline), spos, " ");
					LoadedVertices.emplace_back(
						std::stod(spos[0]),
						std::stod(spos[1]),
						std::stod(spos[2])
					);
				}
				// Generate a Vertex Texture Coordinate
				else if (algorithm::firstToken(curline) == "vt")
				{
					strTextures.push_back(curline);
				}
				// Generate a Vertex Normal;
				else if (algorithm::firstToken(curline) == "vn")
				{
					strNormals.push_back(curline);
				}
				// Generate a Face (vertices & indices)
				else if (algorithm::firstToken(curline) == "f")
				{
					std::vector<std::string> sface;
					Vector3 vVert;
					algorithm::split(algorithm::tail(curline), sface, " ");
					assert(("face must be triangle", sface.size() == 3));
					// For every given vertex do this
					Triangle triangle;
					triangle.strVersion = curline;
					for (int i = 0; i < sface.size(); ++i)
					{
						std::vector<std::string> svert;
						algorithm::split(sface[i], svert, "/");
						int index = algorithm::getIndex(LoadedVertices, svert[0]);
						triangle.array.insert(index);
					}
					LoadedTriangles.push_back(triangle);
				}
				// Get Mesh Material Name
				else if (algorithm::firstToken(curline) == "usemtl")
				{
					assert(("usemtl must be 0 or 1 times", strMaterial.empty()));
					strMaterial = curline;
				}
				// Load Smooth Shading
				else if (algorithm::firstToken(curline) == "s") {
					assert(("smooth shading must be 0 or 1 times", strSmoothShading.empty()));
					strSmoothShading = curline;
				}
				// Load Materials
				else if (algorithm::firstToken(curline) == "mtllib")
				{
					assert(("mtllib must be 0 or 1 times", strMaterialPath.empty()));
					strMaterialPath = curline;
				}
			}
			file.close();
		}

		// Loaded Mesh String in .obj file
		std::string strMesh; // should be once! Throw assert if it`s not
		// Loaded Material String in .obj file
		std::string strMaterial; // should be once! Throw assert if it`s not
		// Loaded Smooth Shading string in .obj file
		std::string strSmoothShading; // should be once! Throw assert if it`s not
		// Loaded Vertex Objects
		std::vector<Vector3> LoadedVertices;
		// Loaded Index Positions
		std::vector<Triangle> LoadedTriangles;
		// PATH to material object
		std::string strMaterialPath; // should be once! Throw assert if it`s not
		std::vector<std::string> strNormals;
		std::vector<std::string> strTextures;
	};
};
	
