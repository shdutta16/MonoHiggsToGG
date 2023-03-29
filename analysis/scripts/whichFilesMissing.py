import sys, os

def AllGlobal(fname):
  fin  = open(fname,"r")
  fout = open(fname+"_log","w")

  path = str(sys.argv[1])
  name = str(sys.argv[2])
  indir = []
  indir = os.listdir(path+"/"+name+"/src/")

  total = 0
  for files in indir:
    if files.endswith("src"):
      total += 1

  lines = []
  for line in fin:
    lines.append(int(line))

  lines.sort()
  size = len(lines)
  n    = size-1
 
  if lines[0]!=0:
    fout.write(str(0)+"\n")

  x=lines[n]
  while x < total:
    fout.write(str(x)+"\n")
    x+=1;
 
  for i in range(0,n):
    diff = lines[i+1] - lines[i] - 1
    if diff==0:
       continue
    else:
       for x in range(0,diff):
         missing = lines[i]+x+1
         fout.write(str(missing)+"\n")

if __name__ == "__main__":
  AllGlobal("test")   
