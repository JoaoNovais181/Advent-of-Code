use std::fs;
use std::collections::HashMap;

fn read_input() -> HashMap<u64, u64> {

    let mut input : HashMap<u64, u64> = HashMap::new();

    let content: String = fs::read_to_string("input.txt").unwrap();
    let nums: Vec<&str> = content.split(' ').collect(); 

    for num in nums { 
        let parsed_num = num.trim().parse::<u64>().unwrap();
        *input.entry(parsed_num).or_default() += 1;
    }

    return input;
}

fn do_blink(input: &HashMap<u64,u64>) -> HashMap<u64,u64> {
    let mut output = HashMap::new();

    for (num, num_occ) in input.iter() {
        let num_string : String = num.to_string();

        if *num == 0 { *output.entry(1).or_default() += num_occ; }
        else if num_string.len() % 2 == 0 {
            let (p1, p2) = num_string.split_at(num_string.len() / 2);
            let p1_num = p1.parse::<u64>().unwrap();
            let p2_num = p2.parse::<u64>().unwrap();

            *output.entry(p1_num).or_default() += num_occ;
            *output.entry(p2_num).or_default() += num_occ;
        }
        else { *output.entry(num * 2024).or_default() += num_occ; }
    }

    return output;
}

fn challenge1(input: &HashMap<u64,u64>) {
    let mut output : HashMap<u64,u64> = input.clone();

    for _ in 0..25 {
        output = do_blink(&output);
    }

    println!("Challenge1: {}", output.values().sum::<u64>());
}

fn challenge2(input: &HashMap<u64,u64>) {
    let mut output : HashMap<u64,u64> = input.clone();

    for _ in 0..75 {
        output = do_blink(&output);
    }

    println!("Challenge2: {}", output.values().sum::<u64>());
}

fn main() {
    let input = &read_input();

    challenge1(input);
    challenge2(input);
}
