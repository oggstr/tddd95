from math import gcd
for f in open(0):
 f=f.strip()
 if len(f)==1:break
 n=len(f)-5
 f=int(f[2:-3])
 p,q=0,0
 for i in range(1,n+1):
  r=f-f//10**i
  t=10**n-10**(n-i)
  r,t=r//gcd(r,t),t//gcd(r,t)
  if not q or t<q:
   p,q=r,t
 print(f"{p}/{q}")