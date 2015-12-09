#include "sectionCreator.h" 
#include "sectionFactory.h"

SectionCreator::SectionCreator(std::string name)
{
    this->name = name;
    SectionCreator* p(this);
	SectionFactory::registerit(name, p);
}