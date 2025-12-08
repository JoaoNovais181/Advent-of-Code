use std::{fs};

fn read_input() -> Vec<i32> {
    let content: String = fs::read_to_string("input.txt").unwrap();
    let mut input : Vec<i32> = Vec::new();

    for (chr) in content.chars() {
        let num : u32 = chr.to_digit(10).unwrap();
        input.push(num as i32);
    }

    return input;
}

fn encode_challenge1(input: Vec<i32>) -> Vec<i32> {
    let mut encoded = Vec::new();
    let mut id = 0;

    for (idx, num) in input.iter().enumerate() {
        let mut _num = *num;
        if idx%2 == 0 {
            while _num > 0 {
                encoded.push(id);
                _num -= 1;
            }
            id += 1;
        }
        else {
            while _num > 0 {
                encoded.push(-1);
                _num -= 1;
            }
        }
    }

    return encoded;
}

fn find_last(input: &Vec<i32>, mut lst: usize) -> usize {
    
    while input[lst] == -1 { lst -= 1; }
    
    return lst;
}

fn checksum (input: &Vec<i32>) -> i64 {
    let mut checksum : i64 = 0;

    for (idx, val) in input.iter().enumerate() {
        let mut _val = *val;
        if _val == -1 { _val = 0; }
        checksum += ((idx as i32) * _val) as i64;
    }
    return checksum;
}

fn challenge1(input: Vec<i32>) {
    let encoded_input = &mut encode_challenge1(input);
    
    let mut lst : usize = find_last(&encoded_input, encoded_input.len() - 1);
    let mut fst : usize = 0;

    // moving the file blocks
    while fst < lst {
        if encoded_input[fst] == -1 {
            encoded_input.swap(fst, lst);
            lst = find_last(&encoded_input, lst - 1);
        }
        fst += 1;
    }

    // calculating the checksum
    let _checksum = checksum(&encoded_input);

    println!("Challenge1: {}", _checksum);
}

fn encode_challenge2(input: Vec<i32>) -> Vec<(i32, i32)> {
    let mut encoded = Vec::new();
    let mut id = 0;

    for (idx, num) in input.iter().enumerate() {
        let mut _num = *num;
        if idx%2 == 0 {
            encoded.push((id, _num));
            id += 1;
        }
        else { encoded.push((-1, _num)); }
    }

    return encoded;
}

fn find_last2(input: &Vec<(i32, i32)>, mut lst: usize) -> usize {
    
    while input[lst].0 == -1 { lst -= 1; }
    
    return lst;
}

fn find_fst2(input: &Vec<(i32, i32)>, mut fst: usize) -> usize {
    
    while fst < input.len() && input[fst].0 != -1  { fst += 1; }
    
    return fst;
}

fn challenge2(input: Vec<i32>) {
    let encoded_input = &mut encode_challenge2(input);

    let mut lst = find_last2(encoded_input, encoded_input.len() - 1);
    let mut fst = find_fst2(encoded_input, 0);

    while fst < lst {
        let id = encoded_input[lst].0;
        let size = encoded_input[lst].1;

        let mut idx = fst;
        let mut found = false;
        while !found && idx < lst {
            if encoded_input[idx].1 >= size {
                found = true;

                encoded_input[lst].0 = -1;
                encoded_input[idx].0 = id;
                
                let diff = encoded_input[idx].1 - size;
                
                encoded_input[idx].1 = size;
                
                if diff > 0 {
                    encoded_input.push((-1, diff));

                    let mut i = encoded_input.len() - 1;
                    while i > idx + 1 {
                        encoded_input.swap(i, i-1);
                        i -= 1;
                    }
                }
            }
            idx = find_fst2(encoded_input, idx+1);
        }

        lst = find_last2(encoded_input, lst-1);
        fst = find_fst2(encoded_input, 0);
    }

    let mut normal_encode = Vec::new();

    for (id, size) in encoded_input {
        let mut _size = *size;

        while _size > 0 { normal_encode.push(*id); _size-=1; }
    }
    
    // calculating the checksum
    let _checksum = checksum(&normal_encode);

    println!("Challenge2: {}", _checksum);

}

fn main() {
    let input = read_input();

    challenge1(input.clone());
    challenge2(input.clone());
}
