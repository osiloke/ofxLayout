#include "sectionCreator.h" 
#include "sectionFactory.h"

SectionCreator::SectionCreator(std::string name)
{
    SectionCreator* p(this);
	SectionFactory::registerit(name, p);
}