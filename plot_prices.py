import matplotlib.pyplot as plt
from math import exp 
import sys

# get file 
filein=sys.argv[1] 
fl = open(filein,"r")

# data from file  
data = fl.readlines() 

#size of frame 
plt.figure(figsize=(12,10)) 

# column labels are in the header of the input file
labels = data[0].strip().split() 

plt.ylabel("Call Price", size=30) 
plt.xlabel("Strike", size=30) 
plt.xticks(size=25)    
plt.yticks(size=25)

data_dict = {} 

for thing in labels:
    if thing == '#':
        continue
    # create lists for T=1,10 for each "label" 
    data_dict[thing] = [[] for _ in xrange(11)]

for item in data[1:]:
    vals = item.strip().split() 

    # get eMax `
    T = int(vals[labels.index('expiry')]) 

    # put the data into it's proper structure
    for thing in labels:
        data_dict[thing][T].append(float(vals[labels.index(thing)]))



line=[]
leg=[]
# i'm only plotting the odd expiry times, so I only need colors for those. 
color = ['green','','red','','blue','','black','','orange',''] 

# plot curves for every odd expiry time... 
for T in range(1,10,2):
    k,=plt.plot(data_dict["strike"][T],data_dict["price"][T]\
             ,color[T],linestyle='-',marker="o",markersize=10,lw=3) 

    # relevant to legend 
    line.append(k)
    leg.append('T$_{exp}$='+str(T)) 

legend = plt.legend(line,leg,loc='best')

for txt in legend.get_texts():
    txt.set_fontsize(25)
    
frame = legend.get_frame()
frame.set_linewidth(2.5)

outname = "call_pricing.pdf"
plt.savefig(outname)    


plt.show()
    
