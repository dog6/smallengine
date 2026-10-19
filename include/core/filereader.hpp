#pragma once

#include <vector>
#include <string>
#include <fstream>


namespace sg::Core {

		class FileReader {

        private: 
			static std::vector<std::string> split(std::string_view str, char delim);
			static int safe_stoi(const std::string& s);
			static std::vector<int> split_ints(const std::string& s, const std::string& delimiter);
		public:
			static std::string ReadFile(const std::string& filePath);
			static bool WriteFile(const std::string& filepath, const std::string& content);
		};

}

