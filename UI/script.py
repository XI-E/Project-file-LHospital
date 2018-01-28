import os

f = open("list.tex", "w")

f.write("\\newcommand*{\\ListHpp}{%" + os.linesep)

a = ""
for path, subdirs, files in os.walk('code'):
    for filename in files:
        if os.path.splitext (filename)[1] in ['.hpp', '.HPP']:
            s = os.path.join(path, filename)
            a += str(s) + ',' + os.linesep

a = a[:-2] + os.linesep
        
f.write(a + '}%' + os.linesep)
a = ""

f.write("\\newcommand*{\\ListCpp}{%" + os.linesep)

for path, subdirs, files in os.walk('code'):
    for filename in files:
        if os.path.splitext (filename)[1] in ['.cpp', '.CPP']:
            s = os.path.join(path, filename)
            a += str(s) + ',' + os.linesep
            
a = a[:-2] + os.linesep
f.write(a + '}%' + os.linesep)
a = ""

f.write("\\newcommand*{\\ListOthers}{%" + os.linesep)

for path, subdirs, files in os.walk('code'):
    for filename in files:
        if os.path.splitext (filename)[1] == '.DAT':
            s = os.path.join(path, filename)
            a += str(s) + ',' + os.linesep
        
a = a[:-2] + os.linesep     
f.write(a + '}%' + os.linesep)
