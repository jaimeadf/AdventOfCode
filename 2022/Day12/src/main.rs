use std::cmp::Ordering;
use std::fs::{File};
use std::io::{BufReader, BufRead};
use std::collections::BinaryHeap;

#[derive(Copy, Clone, Eq, PartialEq)]
struct State {
    cost: usize,
    position: (usize, usize),
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
        other.cost.cmp(&self.cost)
            .then_with(|| self.position.cmp(&other.position))
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

fn resolve_height(heatmap: &Vec<Vec<u8>>, x: usize, y: usize) -> u8 {
    let value = heatmap[y][x];

    match value {
        b'S' => b'a',
        b'E' => b'z',
        _ => value
    }
}

fn find_position(heatmap: &Vec<Vec<u8>>, value: u8) -> Option<(usize, usize)> {
    for (y, row) in heatmap.iter().enumerate() {
        for (x, position_value) in row.iter().enumerate() {
            if value == *position_value {
                return Some((x, y));
            }
        }
    }

    None
}

fn djikstra(heatmap: &Vec<Vec<u8>>, (start_x, start_y): (usize, usize), (end_x, end_y): (usize, usize)) -> Option<usize> {
    let mut distance: Vec<Vec<usize>> = Vec::new();
    let mut visited: Vec<Vec<bool>> = Vec::new();
    let mut heap = BinaryHeap::new();

    for y in 0..heatmap.len() {
        distance.push((0..heatmap[y].len()).map(|_| usize::MAX).collect());
        visited.push((0..heatmap[y].len()).map(|_| false).collect());
    }

    distance[start_y][start_x] = 0;
    heap.push(State { cost: 0, position: (start_x, start_y) });

    while let Some(State { cost, position }) = heap.pop() {
        let (x, y) = position;

        if x == end_x && y == end_y {
            return Some(cost);
        }

        visited[y][x] = true;

        let current_height = resolve_height(heatmap, x, y);
        let mut neighbors = Vec::new();

        if x > 0 {
            neighbors.push((x - 1, y));
        }

        if x < heatmap[y].len() - 1 {
            neighbors.push((x + 1, y));
        }

        if y > 0 {
            neighbors.push((x, y - 1));
        }

        if y < heatmap.len() - 1 {
            neighbors.push((x, y + 1));
        }

        for (neighbor_x, neighbor_y) in neighbors {
            let neighbor_height = resolve_height(heatmap, neighbor_x, neighbor_y);

            if neighbor_height > current_height && neighbor_height - current_height > 1 {
                continue;
            }

            if visited[neighbor_y][neighbor_x] {
                continue;
            }

            let total_distance = cost + 1;

            if total_distance < distance[neighbor_y][neighbor_x] {
                distance[neighbor_y][neighbor_x] = total_distance;
                heap.push(State { cost: total_distance, position: (neighbor_x, neighbor_y) });
            }
        }
    }

    None
}

fn part_one(heatmap: &Vec<Vec<u8>>) {
    let start_position: (usize, usize) = find_position(heatmap, b'S').unwrap();
    let end_position: (usize, usize) = find_position(heatmap, b'E').unwrap();

    println!("Part One: {}", djikstra(&heatmap, start_position, end_position).unwrap());
}

fn part_two(heatmap: &Vec<Vec<u8>>) {
    let end_position: (usize, usize) = find_position(heatmap, b'E').unwrap();
    let mut shortest_distance = usize::MAX;

    for y in 0..heatmap.len() {
        for x in 0..heatmap[y].len() {
            if resolve_height(heatmap, x, y) == b'a' {
                let distance = djikstra(heatmap, (x, y), end_position);

                if distance.is_some() {
                    shortest_distance = std::cmp::min(shortest_distance, distance.unwrap());   
                }
            }
        }
    }

    println!("Part Two: {}", shortest_distance);
}

fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);

    let mut heatmap = Vec::new();

    for line in reader.lines() {
        let mut row = Vec::new();

        for value in line.unwrap().bytes() {
            row.push(value);
        }

        heatmap.push(row);
    }

    part_one(&heatmap);
    part_two(&heatmap);
}
