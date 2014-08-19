//
//  factory.h
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/11/14.
//
//

#ifndef playlistTest_factory_h
#define playlistTest_factory_h

#include "layout.h"
#include "overlay.h"
#include "column.h"
#include "row.h"


namespace Kabbou{
const int OVERLAY_LAYOUT = 1;
const int COLUMN_LAYOUT = 2;
const int ROW_LAYOUT = 3;
    
class LayoutFactory{
public:
    static FluidLayout *newLayout(int layout){
        switch (layout) {
            case OVERLAY_LAYOUT:
                return new OverlayLayout();
                break;
            case ROW_LAYOUT:
                return new RowLayout();
                break;
            case COLUMN_LAYOUT:
                return new ColumnLayout();
                break;
            default:
                return new FluidLayout();
                break;
        }
    }
};
};

#endif
