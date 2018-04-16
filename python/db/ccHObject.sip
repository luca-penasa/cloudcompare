class ccHObject
{
%TypeHeaderCode
#include "../libs/qCC_db/ccHObject.h"
%End

public:

ccHObject(QString name);
QIcon getIcon();

unsigned getChildrenNumber() const;
ccHObject* getChild(unsigned childPos) const;

ccHObject* getFirstChild() const;
    ccHObject* getLastChild() const;


};
