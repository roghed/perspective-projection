#include "obj_file_parser.hpp"
#include "object.hpp"
#include "polygon.hpp"
#include <armadillo>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <ios>
#include <regex>

namespace fs = std::filesystem;

WavefrontObjFileParser::WavefrontObjFileParser(const fs::path &file_path)
{
    if (fs::exists(file_path) && fs::is_regular_file(file_path)) 
    {
        inStream_.exceptions(std::ios::badbit);
        inStream_.open(file_path);
    } 
    else 
    {
        std::string msg = "File `" + file_path.string() + 
            "` does not exist";
        throw std::runtime_error(msg);
    }
}

bool WavefrontObjFileParser::parse()
{
    bool error_state = false;
    
    for (int line_number = 1; inStream_; ++line_number)
    {
        std::string line;
        (void) std::getline(inStream_, line);
        if (!line.empty())
        {
            error_state |= parseLine(line, line_number);
        }
    }

    return error_state;
}

void WavefrontObjFileParser::printErrors(std::ostream &output_stream) const
{
    output_stream << errorsStream_.str() << std::flush;
}

const Object &WavefrontObjFileParser::object() const
{
    for (int i = 0; i < object_.nPolygons(); ++i)
    {
        std::cout << "Face " << i << ":" << std::endl;
        const auto& f = object_.getPolygon(i);
        for (int j = 0; j < f.nVertices(); ++j)
        {
            const auto& v = f.getVertex(j);
            std::cout << '\t' << "(";
            std::cout << v[0] << ", ";
            std::cout << v[1] << ", ";
            std::cout << v[2] << ")";
            std::cout << std::endl;
        }
    }
    
    return object_;
}

void WavefrontObjFileParser::reportError(std::size_t line_number, const std::string& reason)
{
    errorsStream_ << "Errot at " << filePath_.string() << ':' << line_number << ' ';
    errorsStream_ << reason << std::endl;
}

bool WavefrontObjFileParser::parseLine(const std::string &line, std::size_t line_number)
{
    static const std::regex FACE_DECL_REG(R"(f(\s+(\d+)([\\\/]\d*){0,2}){3,})");
    static const std::regex FACE_VERTEX_INDICES_REG(R"((\d+)(?:[\/\\](\d*))?(?:[\/\\](\d*))?)");
    static const std::regex VERTEX_DECL_REG(R"(^v\s([+-]?([0-9]*[.])?[0-9]+)\s([+-]?([0-9]*[.])?[0-9]+)\s([+-]?([0-9]*[.])?[0-9]+)(\s([+-]?([0-9]*[.])?[0-9]+))?$)");
    
    if (std::smatch r; std::regex_match(line, r, VERTEX_DECL_REG))
    {
        double x, y, z;
        std::optional<double> w;

        try
        {
            x = std::stod(r.str(1));
            y = std::stod(r.str(3));
            z = std::stod(r.str(5));

            if (!r.str(8).empty())
                w = std::stod(r.str(8));
        }
        catch (std::out_of_range&)
        {
            reportError(line_number, "Vertex coordinates out of range");
            return false;
        }

        arma::vec3 vertex;

        if (w.has_value())
        {
            if (w.value() == 0)
            {
                reportError(line_number, "W coordinate cannot be zero");
                return false;
            }
            
            vertex = {x / w.value(), y / w.value(), z / w.value()};
        }
        else
        {
            vertex = {x, y, z};
        }

        vertexList_.emplace_back(vertex);
    }
    else if (std::regex_match(line, FACE_DECL_REG))
    {
        Polygon face;
        
        auto beg = std::sregex_iterator(line.cbegin(), line.cend(), FACE_VERTEX_INDICES_REG);
        auto end = std::sregex_iterator();
        for (auto it = beg; it != end; ++it)
        {
            std::size_t v_index;
            
            try
            {
                v_index = std::stoull(it->str(1));
            }
            catch (std::out_of_range&)
            {
                reportError(line_number, "Vertex index `" + it->str(1) + "` is too large");
                return false;
            }

            if (!(0 < v_index && v_index <= vertexList_.size()))
            {
                reportError(line_number, "Vertex with index `" + std::to_string(v_index) + "` does not exist");
                return false;
            }

            face.addVertex(vertexList_[v_index - 1]);
        }

        object_.addPolygon(face);
    }

    return true;
}
