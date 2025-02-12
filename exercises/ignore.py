#b=lambda x:x and"0125986"[x%7]+b(x//7)or""
#for x in open(0):print(b(int(x)))

for x in open(0):
 x=int(x)
 while x>0:print("0125986"[x%7],end="");x//=7
 print()