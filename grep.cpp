#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

void print_usage(const char *program_name)
{
    cerr << "Uso: " << program_name << " <ruta_del_archivo> <palabra_a_buscar> <sensibilidad_a_mayusculas> <color_resaltado>" << endl;
    cerr << "       <sensibilidad_a_mayusculas>: 1 para distinguir entre mayúsculas y minúsculas, 0 para ignorar" << endl;
    cerr << "       <color_resaltado>: color para resaltar coincidencias (opciones: rojo, verde, azul, amarillo, magenta, cian)" << endl;
}

string get_color_code(const string &color)
{
    unordered_map<string, string> color_codes = {
        {"rojo", "\033[1;31m"},
        {"verde", "\033[1;32m"},
        {"azul", "\033[1;34m"},
        {"amarillo", "\033[1;33m"},
        {"magenta", "\033[1;35m"},
        {"cian", "\033[1;36m"}};

    if (color_codes.find(color) != color_codes.end())
    {
        return color_codes[color];
    }
    else
    {
        cerr << "Color no valido. Usando color rojo por defecto." << endl;
        return "\033[1;31m";
    }
}

void highlight_and_print(const string &line, const string &word, bool case_sensitive, const string &color_code)
{
    regex_constants::syntax_option_type flag = case_sensitive ? regex_constants::ECMAScript : regex_constants::icase;
    regex word_regex("(" + word + ")", flag);
    string result = regex_replace(line, word_regex, color_code + "$&\033[0m");
    cout << result << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        print_usage(argv[0]);
        return 1;
    }

    string file_path = argv[1];
    string search_word = argv[2];
    bool case_sensitive = stoi(argv[3]);
    string color = argv[4];
    string color_code = get_color_code(color);

    ifstream file(file_path);

    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo " << file_path << endl;
        return 1;
    }

    string line;
    int line_number = 0;
    int total_matches = 0;
    int lines_with_matches = 0;

    while (getline(file, line))
    {
        line_number++;
        regex_constants::syntax_option_type flag = case_sensitive ? regex_constants::ECMAScript : regex_constants::icase;
        regex word_regex(search_word, flag);
        if (regex_search(line, word_regex))
        {
            highlight_and_print(line, search_word, case_sensitive, color_code);
            total_matches++;
            lines_with_matches++;
        }
    }

    cout << "Total de coincidencias: " << total_matches << endl;
    cout << "Total de lineas con coincidencias: " << lines_with_matches << endl;
    file.close();

    return 0;
}
