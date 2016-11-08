executable = "CloudCompare.exe"
dll_dir =  "/var/run/media/luca/data/mxe.git/"

import fnmatch
import os
import subprocess
import re
import numpy as np

import os
import shutil


def find_file (srcdir, pattern):
    matches = []
    for root, dirnames, filenames in os.walk(srcdir):
        for filename in fnmatch.filter(filenames, pattern):
            matches.append(os.path.abspath(os.path.join(root, filename)))

    return matches


found = find_file("./", executable)[0]

exedir = os.path.dirname(found)


print ("exe is in dir " + str(exedir) )
#os.chdir(exedir)


def test_run(exefile):
    print ("launching exe")
    p = subprocess.Popen(["wine", exefile], stdout=subprocess.PIPE,stderr=subprocess.PIPE,stdin=subprocess.PIPE,cwd=exedir)
    #output.kill()

    import time
    time.sleep(2)
    p.terminate()
    output = p.communicate()

    print ("output received: " + str(output[1]))


    return str(output[1])


def parse_wine_output(output):
    pattern = 'err:module:import_dll Library\s(.*?.dll) \(which is needed by'
    qua = re.findall(pattern  , output,   re.DOTALL)
    return qua


done = False
while (done != True):

    out= test_run(found)

    qua = parse_wine_output(out)

    print (qua)

    dlls = np.unique(np.sort(qua))


    print ("needed dlls: " + str(dlls) )

    if len(dlls) == 0:
        done = True
        print("no ddls are needed. everything ok!   " )
        continue


    for dll in dlls:
        print (str(dll))
        qua = find_file(dll_dir, dll)

        if len(qua) == 0:
            print("-------> cannot find file " + str(dll))
            continue

        print ("copying " + str(qua[0]))
        print( "copying in  " + str(exedir))


        shutil.copy2    (qua[0], exedir)
