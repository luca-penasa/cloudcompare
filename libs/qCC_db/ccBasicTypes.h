//##########################################################################
//#                                                                        #
//#                            CLOUDCOMPARE                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef CC_BASIC_TYPES_HEADER
#define CC_BASIC_TYPES_HEADER

//local
#include "ccColorTypes.h"

//CCLib
#include <CCGeom.h>

//system
#include <stdlib.h>

//! Compressed normals type
/** Should be set accordingly to ccNormalCompressor::QUANTIZE_LEVEL
**/
typedef unsigned CompressedNormType;

#endif //CC_BASIC_TYPES_HEADER
