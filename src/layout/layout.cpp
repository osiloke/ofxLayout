//
//  layout.cpp
//  signage
//
//  Created by Osiloke Emoekpere on 3/20/14.
//
//

#include "layout.h"


using namespace Kabbou;
void FluidLayout::processAction(ActionEvent &e){
    ofLogNotice(this->getType()) << e.message;
};

void FluidLayout::update(){
    std::vector<std::string>::reverse_iterator it = displayable.rbegin();
    for ( it = displayable.rbegin(); it != displayable.rend(); it++){
        FluidLayoutMember section = members.at((*it));
        section.update();
    };
}
