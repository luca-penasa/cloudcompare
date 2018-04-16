class ccHObjectCaster
{
%TypeHeaderCode
#include "../libs/qCC_db/ccHObjectCaster.h"
%End

public:

static ccPointCloud* ToPointCloud(ccHObject* obj, bool* isLockedVertices);



};
