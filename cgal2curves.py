import sys
f=open(sys.argv[1])
ar=f.read().split('\n')
f.close()
nx=0
ne=0
x=''
e=''
for i in range(0,len(ar)):
    v=ar[i].split(' ')
    if(v[0]):
        for j in range(0,int(v[0])):
            x=x+v[j*3+1]+' '+v[j*3+2]+' '+v[j*3+3]+'\n'
            if(j<int(v[0])-1):
                e=e+str(nx)+' '+str(nx+1)+'\n'
                ne=ne+1
            nx=nx+1
print(str(nx) + " 0 " + str(ne))
print(x+e)
