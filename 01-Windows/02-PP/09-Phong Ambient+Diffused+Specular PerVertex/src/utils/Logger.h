#pragma once
#include <windows.h>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <vector>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
/*
Improvements:
Logging mechanism : Add Error, Debug, Warning, Info logs;
Print vectors to log;
*/

class Logger 
{
    public:
        Logger(const std::string& filename) 
        {
            std::string dirName = "Logs/";
            CreateDirectory(dirName.c_str(), NULL);
            std::string fullPath = dirName + filename;
            file.open(fullPath, std::ios::app); // append instead of overwrite
            if (!file.is_open()) {
                MessageBox(NULL, TEXT("Creation of log file failed. Exiting..."), TEXT("File I/O Error"), MB_OK);
                exit(0);
            }
        }

        ~Logger() 
        {
            if (file.is_open()) 
            {
                file.close();
            }
        }

        void write(const std::string& message) 
        {
            if (!file.is_open()) 
            {
                throw std::runtime_error("Logger: File not open");
            }
            file << message << std::endl;
        }


        template <typename T>
        void debug(const std::string& message, const T& value = "") 
        {
            if (!file.is_open()) 
            {
                throw std::runtime_error("Logger: File not open");
            }
            file<<"Debug : "<< message << value << "\n" << std::endl;
        }

        template <typename T>
        void error(const std::string& message, const T& value = "") 
        {
            if (!file.is_open()) 
            {
                throw std::runtime_error("Logger: File not open");
            }
            file<<"Error : "<< message << value << "\n" << std::endl;
        }

        template <typename T>
        void info(const std::string& message, const T& value = "") 
        {
            if (!file.is_open()) 
            {
                throw std::runtime_error("Logger: File not open");
            }
            file<<"Info : "<< message << value << "\n" << std::endl;
        }

        template <typename T>
        void warn(const std::string& message, const T& value = "") 
        {
            if (!file.is_open()) 
            {
                throw std::runtime_error("Logger: File not open");
            }
            file<<"Warning : "<< message  << value  << std::endl;
        }

        template <typename T>
        void printVectorToLog(const std::string& label, const std::vector<T>& vec, size_t size = 3) {
            if (vec.size() % size != 0) {
                throw std::runtime_error("Vector size is not a multiple of the provided size.");
            }

            for (size_t i = 0; i < vec.size(); i += size) {
                std::stringstream ss;
                ss << label << "(";
                for (size_t j = 0; j < size; ++j) {
                    ss << vec[i + j];
                    if (j < size - 1) {
                        ss << ", ";
                    }
                }
                ss << ")";

                if (!file.is_open()) {
                    throw std::runtime_error("Logger: File not open");
                }
                file << ss.str() << std::endl;
            }
        }

        void printGLMVectorToLog(const std::string& label, const std::vector<glm::vec3>& positions) {
            if (!file.is_open()) {
                std::cerr << "Error opening log file: " << label << std::endl;
                return;
            }

            for (const glm::vec3& position : positions) {
                file <<label << "("<< position.x << " " << position.y << " " << position.z << ")"<< std::endl;
            }

            file.close();
        }

    private:
        std::ofstream file;
};
