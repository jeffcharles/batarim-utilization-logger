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
        
        typedef std::vector<std::unique_ptr<ViewModelElement>>::const_iterator
            const_iterator;

        ViewModelInternalNode(
            const std::string name,
            std::vector<std::unique_ptr<ViewModelElement>>
                children
        ): children_(move(children))
        {
            this->name = name;
        }

        virtual void display(IDisplayer* displayer)
        {
            displayer->display_internal(this);
        }

        const_iterator begin() const
        {
            return children_.begin();
        }

        const_iterator end() const
        {
            return children_.end();
        }

    private:
        
        std::vector<std::unique_ptr<ViewModelElement>> children_;
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
