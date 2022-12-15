use std::hash::Hash;
use std::io::{BufReader, BufRead};
use std::fs::File;
use std::collections::HashSet;
use regex::{self, Regex};
use std::cmp;

#[derive(Copy, Clone, Eq, PartialEq, Hash, Debug)]
struct Position {
    x: i32,
    y: i32
}

#[derive(Debug)]
struct Range {
    start: i32,
    end: i32
}

impl Range {
    fn new(start: i32, end: i32) -> Self {
        Self { start, end }
    }

    fn count(&self) -> i32 {
        return self.end - self.start + 1;
    }

    fn contains(&self, value: i32) -> bool {
        value >= self.start && value <= self.end
    }

    fn overlaps(&self, other: &Self) -> bool {
        other.start >= self.start && other.start <= self.end
        || self.start >= other.start && self.start <= other.end
    }

    fn merge(&self, other: &Self) -> Result<Self, ()> {
        if self.overlaps(other) {
            return Ok(Self::new(
                cmp::min(self.start, other.start),
                cmp::max(self.end, other.end)
            ));
        }

        Err(())
    }
}

#[derive(Debug)]
struct Sensor {
    position: Position,
    beacon_position: Position,
    distance: i32
}

impl Sensor {
    fn new(position: Position, beacon_position: Position) -> Self {
        Self {
            distance: (position.x - beacon_position.x).abs() + (position.y - beacon_position.y).abs(),
            position,
            beacon_position
        }
    }
}

fn get_sensor_ranges_at_row(sensors: &[Sensor], y: i32) -> Vec<Range> {
    let mut sensor_ranges = Vec::new();
    let mut merged_ranges = Vec::new();

    for sensor in sensors {
        let remaining_distance = sensor.distance - (sensor.position.y - y).abs();

        if remaining_distance > 0 {
            sensor_ranges.push(Range::new(
                sensor.position.x - remaining_distance,
                sensor.position.x + remaining_distance
            ));
        }
    }

    sensor_ranges.sort_by(|a, b| a.start.cmp(&b.start));

    for sensor_range in sensor_ranges {
        if merged_ranges.is_empty() {
            merged_ranges.push(sensor_range);
            continue;
        }

        let last_merged_range = merged_ranges.last().unwrap();

        match sensor_range.merge(last_merged_range) {
            Ok(merged_range) => {
                merged_ranges.pop();
                merged_ranges.push(merged_range);
            },
            Err(()) => merged_ranges.push(sensor_range)
        }
    }

    return merged_ranges;
}

fn part_one(sensors: &[Sensor], known_positions: &HashSet<Position>, search_y: i32) {
    let mut empty_count = 0;

    for range in get_sensor_ranges_at_row(sensors, search_y) {
        empty_count += range.count();

        for position in known_positions.iter() {
            if position.y == search_y && range.contains(position.x) {
                empty_count -= 1;
            }
        }
    }

    println!("Part One: {}", empty_count);
}

fn part_two(sensors: &[Sensor], known_positions: &HashSet<Position>, search_size: i32) {
    let mut distress_beacon_position = Position { x: 0, y: 0 };

    for y in 0..search_size {
        let ranges = get_sensor_ranges_at_row(sensors, y);

        if ranges.len() >= 2 {
            let range_a = ranges.first().unwrap();

            if range_a.end >= 0 && range_a.end <= search_size {
                let position = Position { x: range_a.end + 1, y };

                if !known_positions.contains(&position) {
                    distress_beacon_position = position;
                    break;
                }
            }
        }
    }

    let tuning_frequency = (distress_beacon_position.x as i64) * 4000000 + distress_beacon_position.y as i64;

    println!("Part Two: {:?} {}", distress_beacon_position, tuning_frequency);
}

fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);

    let regex = Regex::new(r"x=(-?\d+), y=(-?\d+)").unwrap();

    let mut known_positions = HashSet::new();
    let sensors: Vec<_> = reader.lines()
        .map(|line| {
            let line = line.unwrap();
            let mut matches = regex.captures_iter(&line);

            let sensor_match = matches.next().unwrap();
            let beacon_match = matches.next().unwrap();

            let sensor_position = Position {
                x: sensor_match[1].parse().unwrap(),
                y: sensor_match[2].parse().unwrap()
            };

            let beacon_position = Position {
                x: beacon_match[1].parse().unwrap(),
                y: beacon_match[2].parse().unwrap()
            };

            known_positions.insert(sensor_position.clone());
            known_positions.insert(beacon_position.clone());

            Sensor::new(sensor_position, beacon_position)
        })
        .collect();

    part_one(&sensors, &known_positions, if sensors.len() == 14 { 10 } else { 2000000 });
    part_two(&sensors, &known_positions, if sensors.len() == 14 { 20 } else { 4000000 });
}
