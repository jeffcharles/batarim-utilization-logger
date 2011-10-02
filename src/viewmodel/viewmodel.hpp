#ifndef GUARD_viewmodel_h
#define GUARD_viewmodel_h

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "IDisplayer.hpp"

class ViewModelElement
{
    public:
        std::string name;
        virtual void display() = 0;
        virtual void set_displayer(std::shared_ptr<IDisplayer> displayer)
        {
            displayer_ = displayer;
        }

    protected:
        std::shared_ptr<IDisplayer> displayer_;
};

class ViewModelInternalNode : public ViewModelElement
{
    public:
        std::vector<std::shared_ptr<ViewModelElement>> children;

        ViewModelInternalNode(
            const std::string name,
            const std::vector<std::shared_ptr<ViewModelElement>>
                children,
            std::shared_ptr<IDisplayer> displayer
        ): children(children)
        {
            this->name = name;
            displayer_ = displayer;
        }

        virtual void display() { displayer_->display_internal(this); }

        virtual void set_displayer(std::shared_ptr<IDisplayer> displayer)
        {
            displayer_ = displayer;

            typedef
                std::vector<std::shared_ptr<ViewModelElement>>::const_iterator
                ConstIter;
            for(ConstIter iter = children.begin();
                iter != children.end(); ++iter) {
                
                (*iter)->set_displayer(displayer);
            }
        }
};

template <class T>
class ViewModelExternalNode : public ViewModelElement
{
    public:
        T data;
    
        ViewModelExternalNode(
            const std::string name,
            const T data,
            std::shared_ptr<IDisplayer> displayer
        ): data(data)
        {
            this->name = name;
            displayer_ = displayer;
        }

        virtual void display()
        {
            displayer_->display_external(name, to_string_());
        }

    private:
        std::shared_ptr<std::string> to_string_()
        {
            std::stringstream ss;
            ss << data;
            return std::shared_ptr<std::string>(new std::string(ss.str()));
        }
};

std::shared_ptr<std::vector<std::shared_ptr<ViewModelElement>>>
get_view_model(
    std::shared_ptr<IDisplayer> displayer
);

#endif
