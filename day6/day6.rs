use std::{fs, ops::Index, thread::sleep};
use std::time;

#[derive(Debug, Clone, Copy)]
struct Point {
    x: isize,
    y: isize,
}

#[derive(Debug, Clone, Copy)]
struct Dir {
    x: i8,
    y: i8,
}

#[derive(Debug, Clone, Copy)]
struct Guard {
    pos: Point,
    dir: Dir,
    char: char,
}

#[derive(Debug, Clone)]
struct Map {
    width: usize,
    height: usize,
    map: Vec<String>,
    guard: Guard,
}

fn read_input() -> Map {
    let content: String = fs::read_to_string("input.txt").unwrap();

    let mut map: Map = Map {
        width: 0,
        height: 0,
        map: Vec::new(),
        guard: Guard {
            pos: Point { x: 0, y: 0 },
            dir: Dir { x: 0, y: 0 },
            char: 'X',
        },
    };

    let lines = content.split("\n");

    for (pos, line) in lines.enumerate() {
        if map.width == 0 {
            map.width = line.len();
        }
        if line.len() as u8 > 1 {
            map.height += 1;
        }

        if line.contains("^") {
            map.guard.pos.x = line.find("^").unwrap() as isize;
            map.guard.pos.y = pos as isize;
            map.guard.dir = Dir { x: 0, y: -1 };
            map.guard.char = '^';
        } else if line.contains("<") {
            map.guard.pos.x = line.find("<").unwrap() as isize;
            map.guard.pos.y = pos as isize;
            map.guard.dir = Dir { x: -1, y: 0 };
            map.guard.char = '<';
        } else if line.contains(">") {
            map.guard.pos.x = line.find(">").unwrap() as isize;
            map.guard.pos.y = pos as isize;
            map.guard.dir = Dir { x: 1, y: 0 };
            map.guard.char = '>';
        } else if line.contains("v") {
            map.guard.pos.x = line.find("v").unwrap() as isize;
            map.guard.pos.y = pos as isize;
            map.guard.dir = Dir { x: 0, y: 1 };
            map.guard.char = 'v';
        }
        map.map.push(line.replace(map.guard.char, "X"));
    }

    return map;
}

fn rotate_guard(guard: Guard) -> Guard {
    let pos = guard.pos;

    return match guard.char {
        '^' => Guard {
            pos,
            dir: Dir { x: 1, y: 0 },
            char: '>',
        },
        '>' => Guard {
            pos,
            dir: Dir { x: 0, y: 1 },
            char: 'v',
        },
        'v' => Guard {
            pos,
            dir: Dir { x: -1, y: 0 },
            char: '<',
        },
        _ => Guard {
            pos,
            dir: Dir { x: 0, y: -1 },
            char: '^',
        },
    };
}

fn move_point(point: Point, dir: Dir) -> Point {
    return Point {
        x: (point.x + dir.x as isize),
        y: (point.y + dir.y as isize),
    };
}

fn print_vec(vec: Vec<String>) {
    for (_, value) in vec.iter().enumerate() {
        println!("{}", value);
    }
}

fn simulate_guard_movement(map: &mut Map) {
    let mut out_of_map: bool = false;

    while !out_of_map {
        // let mut cp = map.map.clone();
        // let mut tmp = [0; 4];
        // cp[map.guard.pos.y as usize].replace_range(map.guard.pos.x as usize..map.guard.pos.x as usize+1, map.guard.char.encode_utf8(&mut tmp));
        // print_vec(cp);
        if 0 <= map.guard.pos.x
            && map.guard.pos.x < (map.width as isize)
            && 0 <= map.guard.pos.y
            && map.guard.pos.y < (map.height as isize)
        {
            let guard = map.guard;
            let new_pos = move_point(guard.pos, guard.dir);

            if 0 <= new_pos.x && new_pos.x < (map.width as isize) && 0 <= new_pos.y && new_pos.y < (map.height as isize)
            {
                let next: char = map.map[new_pos.y as usize].chars().nth(new_pos.x as usize).unwrap();

                if next == '#' {
                    // println!("ROTATE");
                    map.guard = rotate_guard(guard);
                } else {
                    map.guard.pos = new_pos;
                    map.map[new_pos.y as usize].replace_range(new_pos.x as usize..new_pos.x as usize + 1, "X");
                }
            }
            else {
                map.guard.pos = new_pos;
            }

        } else {
            out_of_map = true;
        }
    }
}

fn count_x_in_map(map: Map) -> usize {
    let mut cnt: usize = 0;
    for (_, line) in map.map.iter().enumerate() {
        for _ in line.matches("X").enumerate() {
            cnt += 1;
        }
    }
    return cnt;
}

fn challenge1(map: Map) {
    let mut map_clone: Map = Map{ 
        map: Vec::new(),
        ..map
    }; 

    for line in map.map.into_iter() {
        map_clone.map.push(line);
    }

    simulate_guard_movement(&mut map_clone);
    println!("Challenge1: {}", count_x_in_map(map_clone));
}

fn main() {
    let map: Map = read_input();

    challenge1(map);
}
