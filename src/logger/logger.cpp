#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../viewmodel/viewmodel.hpp"
#include "LoggerDisplayer.hpp"

using std::cerr;
using std::endl;
using std::ifstream;
using std::ios_base;
using std::ofstream;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

int main(int argc, char** argv)
{
    if(argc != 2) {
        cerr << "Usage: batarim-logger <log_file_in>" << endl;
        cerr << "You did not appear to provide the required log file argument" << endl;
        return 1;
    }

    string log_file_path = argv[1];

    ifstream log_file_in;
    ofstream log_file_out;
    log_file_in.open(log_file_path);

    // initialize view model
    unique_ptr<IViewModelViewer> viewmodel = get_view_model();
    
    // check if there's anything in the log file, if there isn't print the
    // headers
    log_file_in.get();
    bool is_content = log_file_in;
    log_file_in.close();

    log_file_out.open(log_file_path, ios_base::app);
    
    if(!is_content) {
        // output UTF-8 BOM on Windows
        // a lot of Windows applications need the BOM to correctly read
        // unicode characters in a file
        // Linux applications usually assume UTF-8 encoding when reading files
#ifdef WIN32
        char bom[] = { 0xEF, 0xBB, 0xBF, '\0' };
        log_file_out << bom;
#endif

        LoggerFieldNameDisplayer field_name_displayer(log_file_out);
        viewmodel->display(&field_name_displayer);

        log_file_out << endl;
    }

    // print the view model data
    LoggerDataDisplayer data_displayer(log_file_out);
    viewmodel->display(&data_displayer);

    log_file_out << endl;
    log_file_out.close();

    return 0;
}
