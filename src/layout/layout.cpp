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