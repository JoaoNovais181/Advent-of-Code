use std::{collections::HashSet, fs, vec};

fn char_to_i8(chr: char) -> i8 {
    if chr.is_ascii_digit() {
        return (chr as i8) - 48;
    }
    return 0;
}

fn read_input() -> (Vec<(usize, usize)>, Vec<Vec<i8>>) {
    let content: String = fs::read_to_string("input.txt").unwrap();
    let lines: Vec<&str> = content.split('\n').collect();
    let mut map: Vec<Vec<i8>> = vec![vec![0; lines[0].len()]; lines.len()];
    let mut trailheads: Vec<(usize, usize)> = Vec::new();

    for (i, line) in lines.iter().enumerate() {
        for (j, chr) in line.chars().enumerate() {
            let num = char_to_i8(chr);
            map[i][j] = num;

            if num == 0 {
                trailheads.push((i, j));
            }
        }
    }

    return (trailheads, map);
}

fn search_trailhead(pos: (usize, usize), map: &Vec<Vec<i8>>) 
    -> (u16, HashSet<(usize, usize)>) {
    let mut unique_endings: HashSet<(usize, usize)> = HashSet::new();
    let mut num_paths = 0;

    if map[pos.0][pos.1] == 9 {
        unique_endings.insert(pos);
        return (1, unique_endings);
    }

    let dirs: [(usize, usize); 2] = [(0, 1), (1, 0)];

    let curr_val = map[pos.0][pos.1];

    for dir in dirs {
        let mut new_pos = (pos.0 + dir.0, pos.1 + dir.1);
        if new_pos.0 < map.len()
            && new_pos.1 < map[0].len()
            && map[new_pos.0][new_pos.1] - curr_val == 1
        {
            let search = search_trailhead(new_pos, map);
            num_paths += search.0;
            for p in search.1.iter() {
                unique_endings.insert(*p);
            }
        }

        let flag = (pos.0 as i16 - dir.0 as i16 >= 0) && 
        (pos.1 as i16 - dir.1 as i16 >= 0);
        if flag {
            new_pos = (pos.0 - dir.0, pos.1 - dir.1);
            if map[new_pos.0][new_pos.1] - curr_val == 1 {
                let search = search_trailhead(new_pos, map);
                num_paths += search.0;
                for p in search.1.iter() {
                    unique_endings.insert(*p);
                }
            }
        }
    }

    return (num_paths, unique_endings);
}

fn solve(input: &(Vec<(usize, usize)>, Vec<Vec<i8>>)) {
    let mut unique_endings = 0;
    let mut num_paths = 0;

    for pos in input.0.iter() {
        let search = search_trailhead(*pos, &input.1);
        unique_endings += search.1.len();
        num_paths += search.0;
    }

    println!("Challenge1: {}\nChallenge2: {}", unique_endings, num_paths);
}


fn main() {
    let input = &read_input();

    solve(input);
}
