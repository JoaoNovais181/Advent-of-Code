

def readData():
    with open("input.txt", "r") as f:
        data = f.read().splitlines()
        
        cleanData = []
        
        for line in data:
            line = list(map(int,line.split(" ")))
            cleanData.append(line)
        
        return cleanData
    
def challange1():
    
    def checkSafety(row) -> int:
        ascending = row[1] > row[0]
        for i in range(len(row)-1):
            if row[i] == row[i+1]:
                return 0
            diff = row[i+1] - row[i]
            
            if ascending and (diff <= 0 or diff > 3):
                return 0
            elif not ascending and (diff >= 0 or diff < -3):
                return 0
        return 1            
    
    data = readData()
    
    numSafe = 0
    
    for line in data:
        numSafe += checkSafety(line)
    
    print(f"Challange 1: {numSafe}")
    
def challange2():
    
    def checkSafety(row) -> int:
        ascending = row[1] > row[0]
        for i in range(len(row)-1):
            if row[i] == row[i+1]:
                return 0
            diff = row[i+1] - row[i]
            
            if ascending and (diff <= 0 or diff > 3):
                return 0
            elif not ascending and (diff >= 0 or diff < -3):
                return 0
        return 1
    
    def checkSafetyDampener(row) -> int:
        
        l = [checkSafety(row) == 1]
        
        for i in range(len(row)):
            rowNew = [x for idx, x in enumerate(row) if idx != i]
            
            l.append(checkSafety(rowNew) == 1)
            
        return any(l)
    
    data = readData()
    
    numSafe = 0
    
    for line in data:
        numSafe += checkSafetyDampener(line)
    
    print(f"Challange 2: {numSafe}")
    
challange1()
challange2()