//
//  layout.cpp
//  signage
//
//  Created by Osiloke Emoekpere on 3/20/14.
//
//

#include "layout.h"


using namespace Kabbou;

const SectionCreatorImpl<FluidLayout> FluidLayout::creator("FluidLayout");

void FluidLayout::processAction(ActionEvent &e){
    ofLogNotice(this->getType()) << e.message;
}; 