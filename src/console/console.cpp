#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "ConsoleDisplayer.hpp"
#include "../viewmodel/viewmodel.hpp"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::stringstream;
using std::vector;

int main()
{
    shared_ptr<IDisplayer> displayer(new ConsoleDisplayer());
    shared_ptr<vector<shared_ptr<ViewModelElement>>> viewmodel =
        get_view_model(displayer);

    cout << endl;

    typedef vector<shared_ptr<ViewModelElement>>::const_iterator ConstIter;
    for(ConstIter iter = viewmodel->begin();
        iter != viewmodel->end(); ++iter) {
        
        (*iter)->display();
        cout << endl;
    }
    return 0;
}
