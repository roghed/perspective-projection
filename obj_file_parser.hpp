#pragma once
#include "object.hpp"
#include <armadillo>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

// reads vertices and faces data from a Wavefront .obj file
// all other elements apart from vertices and faces are ignored
class WavefrontObjFileParser
{
public:

    // opens the .obj file
    // may throw std::runtime_error if the file does not exist
    WavefrontObjFileParser(const std::filesystem::path& file_path);

    bool parse();
    void printErrors(std::ostream& output_stream = std::clog) const;
    const Object& object() const;

private:

    void reportError(std::size_t line_number, const std::string& reason);
    bool parseLine(const std::string& line, std::size_t line_number);

    std::filesystem::path   filePath_;
    std::ifstream           inStream_;
    std::vector<arma::vec3> vertexList_;
    Object                  object_;
    std::ostringstream      errorsStream_;
};
