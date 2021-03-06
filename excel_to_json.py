import pandas as pd 
import hashlib

def transofrm():
    df = pd.read_excel (r'database.xls')
    list = []
    DATABASE_SIZE = 25 
    for i in range (0, DATABASE_SIZE):
        list.append(int(df.values[i]))
    list_strings = [str(int) for int in list]
    return list_strings 

def hash_sha256(list): 
    hashed_list = []
    for i in range(0,len(list)):
        l = hashlib.sha256(list[i].encode())
        hashed_list.append(l.hexdigest())
    return hashed_list

if __name__ == "__main__":
    list = transofrm()
    hashed_list = hash_sha256(list)
    hashed_list2 = []
    print(f"Original list: \n {list} \n")
    print(f"Hashed list: \n {hashed_list} \n")