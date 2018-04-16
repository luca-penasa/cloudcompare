class ccPointCloud: public ccHObject
{
%TypeHeaderCode
#include "../libs/qCC_db/ccPointCloud.h"
%End

public:
void clear();

ccPointCloud();

bool reserveTheRGBTable();
bool hasSensor();
const CCVector3* getPoint(unsigned index);
unsigned size() const;

virtual bool resize(unsigned newNumberOfPoints);
virtual bool reserve(unsigned newNumberOfPoints);
virtual void addPoint(const CCVector3 &P);
virtual unsigned getNumberOfScalarFields() const;


bool reserveThePointsTable(unsigned _numberOfPoints);


};
