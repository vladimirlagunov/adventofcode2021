use std::cmp::min;
use std::fs::File;
use std::io::{BufRead, BufReader};

fn main() {
//     let input = "
// 1163751742
// 1381373672
// 2136511328
// 3694931569
// 7463417111
// 1319128137
// 1359912421
// 3125421639
// 1293138521
// 2311944581
// ".trim().lines();
    let input = BufReader::new(File::open("day15_input.txt").unwrap())
        .lines()
        .map(|line| line.unwrap());

    let repeats = 5usize;

    let data: Vec<Vec<u64>> = input
        .map(|line|
            line.chars()
                .map(|c| c.to_string().parse::<u64>().unwrap())
                .collect())
        .collect();

    let first = data.first().unwrap().first().unwrap();
    let width = data.first().unwrap().len() * repeats;

    let data = (0..repeats)
        .flat_map(|increment_row| data
            .iter()
            .map(move |v| (0..repeats)
                .flat_map(|increment_cell| v
                    .iter()
                    .map(move |c| (c + increment_row as u64 + increment_cell as u64 - 1) % 9 + 1))
                .collect::<Vec<u64>>()));

    println!("{}", data
        .fold(
            vec![u64::MAX; width],
            |up, current| {
                let mut result = current.clone();
                let mut left = u64::MAX;
                println!("{}", current.iter().flat_map(|c| c.to_string().chars().collect::<Vec<char>>()).collect::<String>());
                for idx in 0..current.len() {
                    result[idx] +=
                        if left != u64::MAX {
                            min(&left, up.get(idx).unwrap())
                        } else if up.get(idx).unwrap().clone() != u64::MAX {
                            up.get(idx).unwrap()
                        } else {
                            &0u64
                        };
                    left = result[idx];
                }
                //println!("{:?}\n?\n{:?}\n=>\n{:?}\n", up, current, result);
                result
            },
        )
        .last()
        .unwrap() - first)
}