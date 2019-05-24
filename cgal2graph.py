import sys
f=open(sys.argv[1])
ar=f.read().split('\n')
f.close()
x=''
ve=[]
ed=[]
for i in range(0,len(ar)):
    v=ar[i].split(' ')
    if(v[0]):
        n=int(v[0])
        a=v[1]+' '+v[2]+' '+v[3]+' '
        b=v[(n-1)*3+1]+' '+v[(n-1)*3+2]+' '+v[(n-1)*3+3]
        try:
            ia=ve.index(a);
        except:
            ve.append(a);
            ia=ve.index(a);
        try:
            ib=ve.index(b);
        except:
            ve.append(b);
            ib=ve.index(b);
        ed.append(str(ia)+' '+str(ib))
print(str(len(ve))+' 0 '+str(len(ed)))
print('\n'.join(ve))
print('\n'.join(ed))
