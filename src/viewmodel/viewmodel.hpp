#ifndef GUARD_viewmodel_h
#define GUARD_viewmodel_h

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "DLLDefines.hpp"
#include "IDisplayer.hpp"

class ViewModelElement
{
    public:
        std::string name;
        virtual void display(IDisplayer* displayer) = 0;
};

class VIEWMODEL_EXPORT IViewModelViewer
{
    public:
        virtual void display(IDisplayer* displayer) = 0;
};

class ViewModelInternalNode : public ViewModelElement
{
    public:
        std::vector<std::unique_ptr<ViewModelElement>> children;

        ViewModelInternalNode(
            const std::string name,
            std::vector<std::unique_ptr<ViewModelElement>>
                children
        ): children(move(children))
        {
            this->name = name;
        }

        virtual void display(IDisplayer* displayer)
        {
            displayer->display_internal(this);
        }
};

template <class T>
class ViewModelExternalNode : public ViewModelElement
{
    public:
        T data;
    
        ViewModelExternalNode(
            const std::string name,
            const T data
        ): data(data)
        {
            this->name = name;
        }

        virtual void display(IDisplayer* displayer)
        {
            displayer->display_external(name, to_string_());
        }

    private:
        std::unique_ptr<std::string> to_string_()
        {
            std::stringstream ss;
            ss << data;
            return std::unique_ptr<std::string>(new std::string(ss.str()));
        }
};

class ViewModel : public IViewModelViewer
{
    public:
        virtual void display(IDisplayer* displayer)
        {
            typedef
                std::vector<std::unique_ptr<ViewModelInternalNode>>::const_iterator
                Iter;
            for(Iter iter = elements_.begin();
                iter != elements_.end(); ++iter) {
                
                (*iter)->display(displayer);
            }
        }

        void push_back(std::unique_ptr<ViewModelInternalNode> element)
        {
            elements_.push_back(move(element));
        }

    private:
        std::vector<std::unique_ptr<ViewModelInternalNode>> elements_;
};

std::unique_ptr<IViewModelViewer> VIEWMODEL_EXPORT get_view_model();

#endif
