import pandas as pd 

df = pd.read_excel (r'database.xls')
list = []
for i in range (0,len(df)):
    list.append(int(df.values[i]))
print(list)