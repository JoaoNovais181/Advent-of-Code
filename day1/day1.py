def quicksort(arr: list[int]) -> list[int]:
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quicksort(left) + middle + quicksort(right)

def binarySearch(arr: list[int], target: int) -> int:
    left = 0
    right = len(arr) - 1
    while left <= right:
        mid = left + (right - left) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1

arr1 = []
arr2 = []

with open("input.txt") as f:
    data = f.read().splitlines()
    
    for line in data:
        num1, num2 = map(int, line.split("   "))
        arr1.append(num1)
        arr2.append(num2)

def challange1():
    global arr1, arr2
        
    arr1 = quicksort(arr1)
    arr2 = quicksort(arr2)
    
    sum = 0
    for i in range(len(arr1)):
        sum += abs(arr1[i] - arr2[i])
        
    print(sum)
    
def challange2():
    global arr1, arr2
    
    arr2 = quicksort(arr2)
    similarityScore = 0
    similarityScores = {}
    
    for num in arr1:
        
        if num in similarityScores:
            similarityScore += similarityScores[num]
            continue
        
        index = binarySearch(arr2, num)
        
        if index == -1:
            similarityScores[num] = 0
        else:
            numOccur = 1
            if index < len(arr2):
                for i in range(index+1, len(arr2)):
                    if arr2[i] == num:
                        numOccur += 1
                    else:
                        break
            if index > 0:
                for i in range(index-1, 0, -1):
                    if arr2[i] == num:
                        numOccur += 1
                    else:
                        break
                    
            similarityScores[num] = numOccur * num
            
        similarityScore += similarityScores[num]
        
    print(similarityScore)
    

# challange1()
challange2()