import sys
sys.path.append("/run/media/luca/data/Code/cloudcompare.git/build-gcc-Default/python")

import pycc
from pycc.io import  BinFilter, FileIOFilter
from pycc.db import ccHObject, ccPointCloud, ccHObjectCaster

f = BinFilter()

lpars = FileIOFilter.LoadParameters()

caster = ccHObjectCaster()

out = ccHObject("out")

fname = "/data/SciDataHub/projects/golarossamodels/offcuts_alltogether_third_clean.bin"

f.loadFile(fname, out, lpars)


nchilds = out.getChildrenNumber()
print(nchilds)

clouds = []
for id in range(nchilds):
    child = out.getChild(id)
    cloud, lockedverts = caster.ToPointCloud(child)
    print(cloud)
    clouds.append(cloud)

savepars = FileIOFilter.SaveParameters()

for id, c in enumerate(clouds):
    print (c.size())
    f.saveToFile(c, str(id) +".bin", savepars)
