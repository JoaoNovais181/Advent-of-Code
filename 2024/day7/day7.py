

add = lambda a,b: a+b
mult = lambda a,b: a*b
concat1 = lambda a,b: int(str(a)+str(b))

def aux(values: list[int]) -> list[int]:
    
    result = [values[0]]
    operators = [add, mult, concat1]

    for val in values[1:]:
        new_result = []
        
        for op in operators:

            new_result.extend([op(num, val) for num in result])
            
        result = new_result
        
    return result