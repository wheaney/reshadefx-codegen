#include "effect_parser.hpp"
#include "effect_module.hpp"
#include "effect_codegen.hpp"
#include "effect_preprocessor.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::string includes_path;
    std::string reshade_fx_file_path;
    std::string output_file_path;

    if (argc == 4) {
        includes_path = argv[1];
        reshade_fx_file_path = argv[2];
        output_file_path = argv[3];
    } else if (argc == 3) {
        reshade_fx_file_path = argv[1];
        output_file_path = argv[2];
    } else {
        std::cerr << "Usage: reshadefx-codegen <includes_path> <reshade_fx_file_path> <output_file_path>" << std::endl;
        return 1;
    }

    reshadefx::preprocessor preprocessor;
    preprocessor.add_macro_definition("__RESHADE__", std::to_string(30000));
    preprocessor.add_macro_definition("__RESHADE_PERFORMANCE_MODE__", "1");
    preprocessor.add_macro_definition("__RENDERER__", "0x14300");
    preprocessor.add_macro_definition("BUFFER_WIDTH", std::to_string(1920));
    preprocessor.add_macro_definition("BUFFER_HEIGHT", std::to_string(1080));
    preprocessor.add_macro_definition("BUFFER_RCP_WIDTH", "(1.0 / BUFFER_WIDTH)");
    preprocessor.add_macro_definition("BUFFER_RCP_HEIGHT", "(1.0 / BUFFER_HEIGHT)");
    preprocessor.add_macro_definition("BUFFER_COLOR_DEPTH", "8");

    if (!includes_path.empty()) {
        preprocessor.add_include_path(includes_path);
    }

    if (!preprocessor.append_file(reshade_fx_file_path)) {
        std::cerr << preprocessor.errors() << std::endl;
        return 1;
    }

    reshadefx::parser parser;
    std::unique_ptr<reshadefx::codegen> codegen(reshadefx::create_codegen_glsl(true, true, false));
    if (!parser.parse(std::move(preprocessor.output()), codegen.get())) {
        std::cerr << parser.errors() << std::endl;
        return false;
    }

    reshadefx::module module;
    codegen->write_result(module);

    std::ofstream output_file(output_file_path);
    if (!output_file.is_open())
    {
        std::cerr << "Failed to open output file" << std::endl;
        return 1;
    }

    output_file << "// Automatically generated from ReShadeFX\n// See https://github.com/wheaney/reshadefx-codegen\n" << std::endl;

    output_file.write(reinterpret_cast<const char*>(module.code.data()), module.code.size());
    output_file.close();

    return 0;
}