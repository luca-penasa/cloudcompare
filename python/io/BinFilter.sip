class BinFilter: public FileIOFilter
{
%TypeHeaderCode
#include "../libs/qCC_io/BinFilter.h"
%End

public:
    CC_FILE_ERROR loadFile(const QString& filename, ccHObject& container, FileIOFilter::LoadParameters& parameters);

    CC_FILE_ERROR saveToFile(ccHObject* entity, const QString& filename, const SaveParameters& parameters);
};
