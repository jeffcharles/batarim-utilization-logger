#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../viewmodel/viewmodel.hpp"
#include "LoggerDisplayer.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

int main()
{
    // initialize view model
    std::shared_ptr<IDisplayer> field_name_displayer(
        new LoggerFieldNameDisplayer()
    );
    shared_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel =
        get_view_model(field_name_displayer);

    typedef vector<shared_ptr<ViewModelElement>>::const_iterator ConstIter;
    
    // check if there's anything in the log file, if there isn't print the
    // headers
    cin.get();
    if(!cin) {
        for(ConstIter iter = viewmodel->begin();
            iter != viewmodel->end(); ++iter) {

            (*iter)->display();
        }

        cout << endl;
    }

    // set the view model displayer to the log data one and print the view
    // model data
    std::shared_ptr<IDisplayer> data_displayer(
        new LoggerDataDisplayer()
    );

    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {

        (*iter)->set_displayer(data_displayer);
    }

    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {

        (*iter)->display();
    }

    cout << endl;

    return 0;
}
