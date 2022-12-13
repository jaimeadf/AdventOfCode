use std::io::{BufRead, BufReader};
use std::fs::File;

#[derive(Clone)]
struct Item {
    number: Option<i32>,
    list: Option<Vec<Item>>
}

impl Item {
    fn number(number: i32) -> Self {
        Item { number: Some(number), list: None }
    }

    fn list(list: Vec<Item>) -> Self {
        Item { number: None, list: Some(list) }
    }

    fn with_wrapper(item: Self) -> Self {
        return Self::list(vec![item]);
    }

    fn parse(text: &str) -> Item {
        if !text.starts_with('[') && !text.ends_with(']') {
            return Self::number(text.parse().unwrap());
        }

        let mut list: Vec<Item> = Vec::new();
        let mut level = 0;

        let mut last_separator_index = 0;

        for (i, character) in text.chars().enumerate() {
            match character {
                '[' => level += 1,
                ']' => level -= 1,
                _ => ()
            }

            if i == text.len() - 1 || character == ',' && level == 1 {
                let value = &text[(last_separator_index + 1)..i];
                
                if !value.is_empty() {
                    list.push(Self::parse(value));
                }

                last_separator_index = i;
            }
        }

        Self::list(list)
    }

    fn is_number(&self) -> bool {
        self.number.is_some()
    }

    fn is_list(&self) -> bool {
        self.list.is_some()
    }

    fn unwrap_number(&self) -> i32 {
        self.number.unwrap()
    }

    fn unwrap_list(&self) -> &Vec<Item> {
        self.list.as_ref().unwrap()
    }
}

impl ToString for Item {
    fn to_string(&self) -> String {
        if self.is_number() {
            return self.unwrap_number().to_string();
        }

        if self.is_list() {
            let mut string = String::from('[');
            let list = self.unwrap_list();

            for (i, item) in list.iter().enumerate() {
                string.push_str(&Self::to_string(item));

                if i != list.len() - 1 {
                    string.push(',');
                }
            }

            string.push(']');

            return string;
        }

        String::from("(none)")
    }
}

#[derive(PartialEq)]
enum Order {
    Correct,
    Incorrect,
    Indeterminable
}

fn compare(left: &Item, right: &Item) -> Order {
    if left.is_number() && right.is_number() {
        let left_number = left.unwrap_number();
        let right_number = right.unwrap_number();

        if left_number == right_number {
            return Order::Indeterminable;
        }

        return if left_number < right_number { Order::Correct } else { Order::Incorrect };
    }

    if left.is_list() && right.is_list() {
        let left_list = left.unwrap_list();
        let right_list = right.unwrap_list();

        for i in 0..std::cmp::max(left_list.len(), right_list.len()) {
            if i == left_list.len() {
                return Order::Correct;
            }
 
            if i == right_list.len() {
                return Order::Incorrect;
            }

            let left_member = &left_list[i];
            let right_member = &right_list[i];

            let order = compare(left_member, right_member);

            if order != Order::Indeterminable {
                return order;
            }
        }

        return Order::Indeterminable;
    }

    if left.is_list() && right.is_number() {
        return compare(left, &Item::with_wrapper(right.clone()));
    }

    return compare(&Item::with_wrapper(left.clone()), right);
}

fn part_one(packets: &Vec<Item>) {
    let mut sum = 0;

    for i in (0..packets.len()).step_by(2) {
        let left = &packets[i];
        let right = &packets[i + 1];

        if compare(&left, &right) == Order::Correct {
            sum += i / 2 + 1;
        }
    }

    println!("Part One: {}", sum);
}

fn part_two(packets: &Vec<Item>) {
    let mut sorted_packets = packets.clone();

    sorted_packets.push(Item::parse("[[2]]"));
    sorted_packets.push(Item::parse("[[6]]"));

    for i in 0..sorted_packets.len() {
        for j in 0..(sorted_packets.len() - i - 1) {
            if compare(&sorted_packets[j], &sorted_packets[j + 1]) == Order::Incorrect {
                sorted_packets.swap(j, j + 1);
            }
        }
    }

    let mut divider_one_index = 0;
    let mut divider_two_index = 0;

    for (i, packet) in sorted_packets.iter().enumerate() {
        if packet.to_string() == "[[2]]" {
            divider_one_index = i + 1;
        }

        if packet.to_string() == "[[6]]" {
            divider_two_index = i + 1;
        }
    }
    
    println!("Part Two: {}", divider_one_index * divider_two_index);
}

fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);
    let packets: Vec<_> = reader.lines()
        .map(|line| line.unwrap())
        .filter(|line| !line.is_empty())
        .map(|line| Item::parse(&line))
        .collect();

    part_one(&packets);
    part_two(&packets);
}
