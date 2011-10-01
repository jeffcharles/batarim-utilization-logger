#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../viewmodel/viewmodel.hpp"
#include "LoggerDisplayer.hpp"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

int main()
{
    std::shared_ptr<IDisplayer> field_name_displayer(
        new LoggerFieldNameDisplayer()
    );
    shared_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel =
        get_view_model(field_name_displayer);

    typedef vector<shared_ptr<ViewModelElement>>::const_iterator ConstIter;
    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {

        (*iter)->display();
    }

    cout << endl;

    std::shared_ptr<IDisplayer> data_displayer(
        new LoggerDataDisplayer()
    );
    shared_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel2 =
        get_view_model(data_displayer);
    
    for(ConstIter iter = viewmodel2->begin();
        iter != viewmodel2->end(); ++iter) {

        (*iter)->display();
    }

    cout << endl;

    return 0;
}
