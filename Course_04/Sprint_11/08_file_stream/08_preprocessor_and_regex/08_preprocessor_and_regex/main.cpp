#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

std::optional<std::filesystem::path> FindIncludeFile(const std::filesystem::path& filename, const std::filesystem::path& catalog) {
    for (const std::filesystem::path& c : std::filesystem::directory_iterator(catalog)) {
        //если это файл, то возвращаем
        //если это каталог, то входим рекурсивно
        std::filesystem::file_status status = std::filesystem::status(c);
        if (status.type() == std::filesystem::file_type::regular) {
            const std::filesystem::path file_name = filename;
            if (c.filename() == file_name.filename()) {
                return c;
            }
        }
        else {
            return FindIncludeFile(filename, c);
        }
    }
    return nullopt;
}

// напишите эту функцию
bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {

    std::ifstream in(in_file);
    std::ofstream out(out_file, std::ios::app);

    if (!in.is_open()) {
        return false;
    }

    static std::regex relatively(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
    static std::regex absolutely(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");
    std::smatch m;

     std::string line;

     unsigned int row = 0;
     while (getline(in, line)) {
         row++;

        if(std::regex_match(line, m, relatively)){
            //const std::filesystem::path p = in_file.parent_path().string() + "\\" + m[1].str();
            const std::filesystem::path p = in_file.parent_path() / m[1].str();
            if (!Preprocess(p, out_file, include_directories)) {
                bool preprocessed = false;
                for (const std::filesystem::path& directory : include_directories) {
                    const std::optional<std::filesystem::path> i = FindIncludeFile(m[1].str(), directory);
                    if (i) {
                        preprocessed = Preprocess(i.value(), out_file, include_directories);
                        break;
                    }
                }
                if (!preprocessed) {
                    std::cout << "unknown include file \"" << m[1].str() << "\" at file " << in_file << " at line " << row << std::endl;
                    return false;
                }
            }
        }
        else if(std::regex_match(line, m, absolutely)){
            for (const std::filesystem::path& directory : include_directories) {
                const std::optional<std::filesystem::path> p = FindIncludeFile(m[1].str(), directory);
                if(p){
                    if (Preprocess(p.value(), out_file, include_directories)) {
                        break;
                    }

                }
                else {
                    std::cout << "unknown include file <" << m[1].str() << "> at file " << in_file << " at line " << row << std::endl;
                    return false;
                }
            }
        }
        else{
            out<<line<<std::endl;
        }
     }
    return true;
}

string GetFileContents(string file) {
    ifstream stream(file);

    // конструируем string по двум итераторам
    return { (istreambuf_iterator<char>(stream)), istreambuf_iterator<char>() };
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
            "#include \"dir1/b.h\"\n"
            "// text between b.h and c.h\n"
            "#include \"dir1/d.h\"\n"
            "\n"
            "int SayHello() {\n"
            "    cout << \"hello, world!\" << endl;\n"
            "#   include<dummy.txt>\n"
            "}\n"sv;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
            "#include \"subdir/c.h\"\n"
            "// text from b.h after include"sv;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
            "#include <std1.h>\n"
            "// text from c.h after include\n"sv;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
            "#include \"lib/std2.h\"\n"
            "// text from d.h after include\n"sv;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"sv;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"sv;
    }

    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
        { "sources"_p / "include1"_p,"sources"_p / "include2"_p })));

    ostringstream test_out;
    test_out << "// this comment before include\n"
        "// text from b.h before include\n"
        "// text from c.h before include\n"
        "// std1\n"
        "// text from c.h after include\n"
        "// text from b.h after include\n"
        "// text between b.h and c.h\n"
        "// text from d.h before include\n"
        "// std2\n"
        "// text from d.h after include\n"
        "\n"
        "int SayHello() {\n"
        "    cout << \"hello, world!\" << endl;\n"sv;

    assert(GetFileContents("sources/a.in"s) == test_out.str());
}

int main() {
    Test();
}
