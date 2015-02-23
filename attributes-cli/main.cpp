#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <boost/smart_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;
using namespace boost::program_options;

#include "ClassToken.h"
#include "MethodToken.h"

static const regex HEADER_FILES(".*\\.(h|hpp)$");

/*!
	\brief Retrieves the all relavant header files located on the supplied file path
	\param path
	\param result
*/
void getHeaderFiles(const std::string& path, std::vector<filesystem::path>& result) {
	vector<filesystem::path> paths;
	filesystem::recursive_directory_iterator end;
	for (filesystem::recursive_directory_iterator it(path); it != end; ++it) {
		if (!boost::filesystem::is_regular_file(it->status()))
			continue;
		
		auto& filePath = it->path();
		boost::smatch what;
		if (!boost::regex_match(filePath.string(), what, HEADER_FILES))
			continue;

		result.push_back(it->path());
	}
}

std::vector<std::string> getInheritedClasses(const std::string& data, const std::string& className) {
	const string SEARCH = string(".*") + className + string("[\\s]*:([a-zA-Z0-9\\,\\<\\>\\s]*){.*");
	const regex INHERIT_REGEX(SEARCH);

	vector<string> inheritsClasses;
	boost::smatch what;
	if (boost::regex_match(data, what, INHERIT_REGEX)) {
		const string name = what[1];
		vector<string> temp;
		boost::split(temp, name, boost::is_any_of(string(",")));

		// Only use public classes
		for (auto it = temp.begin(), end = temp.end(); it != end; ++it) {
			auto potential = *it;
			if (potential.find_first_of(string("public ")) == std::string::npos)
				continue;
			static const string EMPTY("");
			boost::replace_all(potential, string("public "), EMPTY);
			boost::replace_all(potential, string("\n"), EMPTY);
			boost::replace_all(potential, string("\r"), EMPTY);
			boost::replace_all(potential, string("\t"), EMPTY);
			boost::replace_all(potential, string(" "), EMPTY);

			inheritsClasses.push_back(potential);
		}

	}
	return inheritsClasses;
}

string getTokenData(const filesystem::path& path)
{
	filesystem::ifstream stream(path);
	string data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	stream.close();

	// remove comments
	static const regex SINGLELINE_COMMENT("\\/\\/.*?\\n");
	static const regex MULTILINE_COMMENT("\\/\\*.*?\\*\\/");
	static const string EMPTY("");
	data = boost::regex_replace(data, SINGLELINE_COMMENT, EMPTY, boost::match_default | boost::format_all);
	data = boost::regex_replace(data, MULTILINE_COMMENT, EMPTY, boost::match_default | boost::format_all);

	// Remove defines
	static const regex DEFINES("#ifndef\\s+[A-Z0-9_]+\\s+#define\\s+[A-Z0-9_]+\\s*\\(.*?\\).*?\\s+#endif");
	data = boost::regex_replace(data, DEFINES, EMPTY, boost::match_default | boost::format_all);

	// Remove defines
	// #define
	static const regex DEFINES2("#define\\s+[A-Z0-9_]+\\s*\\(\\).*?\\n");
	data = boost::regex_replace(data, DEFINES2, EMPTY, boost::match_default | boost::format_all);

	// #define
	static const regex DEFINES3("#define\\s+[A-Z0-9_]+\\s*\\(\\.\\.\\.\\).*?\\n");
	data = boost::regex_replace(data, DEFINES3, EMPTY, boost::match_default | boost::format_all);

	return data;
}

string getClassBody(const string& data)
{
	string::size_type startPos = 0;
	for (string::size_type i = 0; i < data.length(); ++i) {
		if (data[i] == '{') {
			startPos = i;
			break;
		}
		else if (data[i] == ';') {
			return string();
		}
	}

	int count = 0;
	string body;
	for (string::size_type i = startPos; i < data.length(); ++i) {
		if (data[i] == '{') {
			count++;
		}
		else if (data[i] == '}') {
			count--;
		}

		body += data[i];
		if (count == 0) {
			break;
		}
	}

	return body;
}

size_t getHeadStartIndex(const string& data, const size_t searchStart)
{
	for (size_t i = searchStart; i > 0; --i) {
		if (data[i] == ';') {
			return i;
		}
	}

	return 0;
}

/*!
	\brief Retrieves the all relavant header files located on the supplied file path
	\param path
	\param result
*/
void getTokens(const filesystem::path& path, map<string, boost::shared_ptr<ClassToken>>& tokens)
{
	const auto data = getTokenData(path);

	boost::sregex_iterator it(data.begin(), data.end(), CLASS_REGEX_1);
	for (const boost::sregex_iterator e; 
		it != e; ++it) {
		const boost::smatch match = *it;

		const string val = match[1];
		const string type = match[2];
		const string name = match[3];
			
		const vector<string> inheritsClasses = getInheritedClasses(data, name);

		const auto end_pos = match.length() + match.position();
		const auto head_start_pos = getHeadStartIndex(data, match.position());
		const auto classHead = data.substr(head_start_pos, match.position() - head_start_pos);
		const auto classBody = getClassBody(data.substr(end_pos));

		if (classBody.empty()) {
			continue;
		}

		boost::shared_ptr<ClassToken> token(new ClassToken(path, name, inheritsClasses, classHead, classBody));
		token->ParseHead();
		token->ParseBody();
		tokens.insert(make_pair(name, token));
	}
}

void writeResult(const string& module, const stringstream& header, const stringstream& body, const filesystem::path& outputPath) {
	filesystem::ofstream stream(outputPath, ios::trunc);
	stream << "//" << endl << "// Generated by Attributes pre-build step program" << endl << "//" << endl << endl;
	stream << endl;
	stream << "#include \"../../attributes/attributes.h\"" << endl;
	stream << endl;
	stream << header.str() << endl;
	stream << "void LoadClasses(IMutableClassLoader* cl)" << endl;
	stream << "{" << endl;
	stream << body.str() << endl;
	stream << TAB << "cl->Build();" << endl;
	stream << "}" << endl;
	stream.close();
}

int main(int argc, char** argv) {
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print Usage Message")
		("module,m", value<string>()->composing(), "Module Name")
		("path,P", value<vector<string>>()->composing(), "Source Code Path")
		("output,O", value<string>()->composing(), "Output File");

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return 0;
	}

	auto module = vm["module"].as<string>();
	auto paths = vm["path"].as<vector<string>>();
	auto output = vm["output"].as<string>();

	vector<filesystem::path> processFiles;
	for (auto it = paths.begin(), end = paths.end(); it != end; ++it) {
		getHeaderFiles(*it, processFiles);
	}

	map<string, boost::shared_ptr<ClassToken>> classTokens;
	for (auto it = processFiles.begin(), end = processFiles.end(); it != end; ++it) {
		getTokens(*it, classTokens);
	}

	stringstream header;
	stringstream body;
	for (auto it = classTokens.begin(), end = classTokens.end(); it != end; ++it) {
		it->second->Visit(header, body);
	}

	filesystem::path outputPath(output);
	filesystem::path outputPathDirectory = outputPath.parent_path();
	if (!filesystem::exists(outputPathDirectory)) {
		filesystem::create_directories(outputPathDirectory);
	}

	writeResult(module, header, body, outputPath);

	return 0;
}