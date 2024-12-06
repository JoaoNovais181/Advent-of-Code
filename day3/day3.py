import re

def readInput() -> str:
    with open("input.txt") as f:
        return f.read().replace("\n", "")
    
def challenge1(input: str):
    regex = re.compile(r"mul\((\d{1,3}),(\d{1,3})\)")
    matches = regex.findall(input)

    result = 0
    for m in matches:
        result += int(m[0]) * int(m[1])
            
    print(f"Challenge2 1: {result}")

def challenge2(input: str):
    
    regex = re.compile(r"(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))")
    matches = regex.findall(input)
    
    result = 0
    do = True
    for m in matches:
        if m == "do()":
            do = True
        elif m == "don't()":
            do = False
        elif do:
            m = re.match(r"mul\((\d{1,3}),(\d{1,3})\)", m)
            result += int(m[1]) * int(m[2])
    
    print(f"Challenge2 2: {result}")

input = readInput()
challenge1(input)
challenge2(input)