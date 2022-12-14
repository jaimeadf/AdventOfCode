use std::cmp;
use std::fs::File;
use std::hash::Hash;
use std::io::{BufRead, BufReader};
use std::collections::HashMap;

#[derive(Clone, Eq, PartialEq, Hash)]
struct Position {
    x: i32,
    y: i32
}

impl Position {
    fn parse(text: &str) -> Self {
        let result: Vec<_> = text.split(',').collect();

        Position {
            x: result[0].parse().unwrap(),
            y: result[1].parse().unwrap()
        }
    }
}

fn find_bottom_y(tiles: &HashMap<Position, char>) -> i32 {
    let mut bottom_y = 0;    

    for (position, &tile) in tiles {
        if tile == '#' {
            bottom_y = cmp::max(bottom_y, position.y);
        }
    }

    bottom_y
}

fn find_next_position(tiles: &HashMap<Position, char>, sand: &Position, floor_y: i32) -> Option<Position> {
    if sand.y < floor_y - 1 {
        for relative_x in [0, -1, 1] {
            let position = Position { x: sand.x + relative_x, y: sand.y + 1 };

            if !tiles.contains_key(&position) {
                return Some(position);
            }
        }
    }

    None
}

fn emulate<F>(
    tiles: &mut HashMap<Position, char>,
    source_position: &Position,
    floor_y: i32, 
    should_stop: F
) -> usize where F : Fn(&Position) -> bool{
    let mut count = 0;

    loop {
        let mut sand = source_position.clone();

        while let Some(next_position) = find_next_position(tiles, &sand, floor_y) {
            sand = next_position;
        }

        if should_stop(&sand) {
            break;
        }

        tiles.insert(sand, 'o');
        count += 1;
    }

    count
}


fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);

    let mut tiles = HashMap::new();

    for line in reader.lines() {
        let points: Vec<_> = line.unwrap().split(" -> ")
            .map(|text| Position::parse(text))
            .collect();

        for i in 1..points.len() {
            let point_a = &points[i - 1];
            let point_b = &points[i];

            if point_a.y == point_b.y {
                for x in cmp::min(point_a.x, point_b.x)..=cmp::max(point_a.x, point_b.x) {
                    tiles.insert(Position { x, y: point_a.y }, '#');
                }
            }

            if point_a.x == point_b.x {
                for y in cmp::min(point_a.y, point_b.y)..=cmp::max(point_a.y, point_b.y) {
                    tiles.insert(Position { x: point_a.x, y }, '#');
                }
            }
        }
    }

    let source_position = Position { x: 500, y: 0 };
    let bottom_y = find_bottom_y(&tiles);

    let part_one = emulate(&mut tiles.clone(), &source_position, bottom_y + 1, |sand| sand.y >= bottom_y);
    let part_two = emulate(&mut tiles.clone(), &source_position, bottom_y + 2, |sand| *sand == source_position) + 1;

    println!("Part One: {}", part_one);
    println!("Part Two: {}", part_two);
}
