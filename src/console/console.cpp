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
using std::unique_ptr;
using std::vector;

int main()
{
    unique_ptr<IViewModelViewer> viewmodel = get_view_model();
    ConsoleDisplayer displayer;

    cout << endl;
    viewmodel->display(&displayer);

    return 0;
}
