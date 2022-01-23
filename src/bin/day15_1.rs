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

    let data: Vec<Vec<u64>> = input
        .map(|line|
            line.chars()
                .map(|c| c.to_string().parse::<u64>().unwrap())
                .collect())
        .collect();

    println!("{}", data.iter()
        .fold(
            vec![u64::MAX; data.first().unwrap().len()],
            |up, current| {
                let mut result = current.clone();
                let mut left = u64::MAX;
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
        .unwrap() - data.first().unwrap().first().unwrap())
}