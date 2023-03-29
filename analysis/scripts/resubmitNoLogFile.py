import sys

def main():
  fin      = open("test_log")
  fullpath = str(sys.argv[1])
  path     = str(sys.argv[2])
  name     = str(sys.argv[3]) 
 
  lines = []
  for line in fin:
    lines.append(int(line))

  size = len(lines)
  print size

  for i in range(0,size):
    print "bsub -q 8nh -o "+path+"/"+name+"/log/"+name+"_"+str(lines[i])+".log source "+fullpath+"/"+path+"/"+name+"/src/submit_"+str(lines[i])+".src" 
  

if __name__ == "__main__": 
  main()
