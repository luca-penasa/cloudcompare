import sys
sys.path.append("/home/luca/Code/cloudcompare-official.git/build-cloudcompare-official.git-clang-Default/python")

import cc
f = cc.BinFilter()

lpars = cc.FileIOFilter.LoadParameters()

caster = cc.ccHObjectCaster()

out = cc.ccHObject("out")

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

savepars = cc.FileIOFilter.SaveParameters()

for id, c in enumerate(clouds):
    print (c.size())
    f.saveToFile(c, str(id) +".bin", savepars)
