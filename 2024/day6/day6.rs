use std::{fs, collections::{HashMap, HashSet}};

#[derive(Debug, Clone, Copy, PartialEq, Hash)]
struct Point {
    x: isize,
    y: isize,
}

impl Eq for Point {}

#[derive(Debug, Clone, Copy, PartialEq, Hash)]
struct Dir {
    x: i8,
    y: i8,
}

impl Eq for Dir {}

#[derive(Debug, Clone, Copy, PartialEq)]
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

fn simulate_guard_movement(map: &mut Map) -> HashSet<Point> {
    let mut out_of_map = false;
    let mut result = HashSet::new();

    while !out_of_map {

        if 0 <= map.guard.pos.x
            && map.guard.pos.x < (map.width as isize)
            && 0 <= map.guard.pos.y
            && map.guard.pos.y < (map.height as isize)
        {
            let guard = map.guard;
            result.insert(guard.pos);
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
    return result;
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

fn does_guard_exit (map1: Map, extra_obstacle: &Point) -> bool {
    let mut map = Map {
        map: Vec::new(),
        ..map1
    };

    for line in map1.map.into_iter() {
        map.map.push(line);
    }


    let mut out_of_map = false;
    let mut dir_at_pos : HashMap<Point, Vec<Dir>> = HashMap::new();

    while !out_of_map {
        let extra = extra_obstacle.clone();
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

                if dir_at_pos.get_mut(&new_pos).map_or(false, |l| l.contains(&guard.dir)) { return false; }

                if new_pos == extra || next == '#'  {
                    // println!("ROTATE");
                    map.guard = rotate_guard(guard);
                }
                else {
                    map.guard.pos = new_pos;
                    map.map[new_pos.y as usize].replace_range(new_pos.x as usize..new_pos.x as usize + 1, "X");
                    
                    if !dir_at_pos.contains_key(&new_pos) { dir_at_pos.insert(new_pos, Vec::new()); }
                    dir_at_pos.get_mut(&new_pos).unwrap().push(map.guard.dir);
                }
            }
            else {
                map.guard.pos = new_pos;
            }

        } else {
            out_of_map = true;
        }
    }
    return true;
}

fn challenge2(map: Map) {
    let mut num_obs = 0;


    let path = simulate_guard_movement(&mut map.clone());

    let mut v = Vec::new();

    for (i,p) in path.iter().enumerate() {
        let cl = map.clone();
        if *p != cl.guard.pos && !does_guard_exit(cl, &p) { v.push(p.clone()); }
    }

    println!("Challenge2: {}", v.len());
}

fn main() {
    let map: Map = read_input();

    challenge1(map.clone());
    challenge2(map.clone());
}
