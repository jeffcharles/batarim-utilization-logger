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
    std::shared_ptr<IDisplayer> field_name_displayer(
        new LoggerFieldNameDisplayer(log_file_out)
    );
    shared_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel =
        get_view_model(field_name_displayer);

    typedef vector<shared_ptr<ViewModelElement>>::const_iterator ConstIter;
    
    // check if there's anything in the log file, if there isn't print the
    // headers
    log_file_in.get();
    bool is_content = log_file_in;
    log_file_in.close();

    log_file_out.open(log_file_path, ios_base::app);
    
    if(!is_content) {
        for(ConstIter iter = viewmodel->begin();
            iter != viewmodel->end(); ++iter) {

            (*iter)->display();
        }

        log_file_out << endl;
    }

    // set the view model displayer to the log data one and print the view
    // model data
    std::shared_ptr<IDisplayer> data_displayer(
        new LoggerDataDisplayer(log_file_out)
    );

    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {

        (*iter)->set_displayer(data_displayer);
    }

    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {

        (*iter)->display();
    }

    log_file_out << endl;
    log_file_out.close();

    return 0;
}
