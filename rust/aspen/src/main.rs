use std::io;

#[allow(unused_variables)]
#[allow(non_snake_case)]
fn main() {
    let mut buf = String::new();
    scan_line(&mut buf);
    let N: usize = buf.trim().parse().expect("Failed to parse N");

    scan_line(&mut buf);
    let mut iter = buf.trim().split_whitespace();
    let L: i32 = iter.next().unwrap().parse().expect("Failed to parse L");
    let W: i32 = iter.next().unwrap().parse().expect("Failed to parse W");

    let mut trees: Vec<f64> = vec![];
    trees.reserve(N as usize);
    for _ in 0..N {
        scan_line(&mut buf);
        trees.push(buf.trim().parse().unwrap());
    }
    // Sort trees by position - lowest first
    trees.sort_by(|a, b| a.partial_cmp(b).unwrap());

    let targets = target_positions(N, L, W);

    // dp[i][j] : best dist for placing i tress to the left and j trees to the right
    // At a given point we consider placing the next tree to the left or right
    // Assumes trees are sorted by y-position
    let mut dp = vec![vec![f64::MAX; N / 2 + 1]; N / 2 + 1];

    dp[0][0] = 0.0;

    // Base cases
    for i in 0..N / 2 {
        let lft = targets[2 * i];
        let rgt = targets[2 * i + 1];

        dp[i + 1][0] = dp[i][0] + dist(trees[i], lft);
        dp[0][i + 1] = dp[0][i] + dist(trees[i], rgt);
    }

    // Fill dp table
    for i in 0..N / 2 {
        for j in 0..N / 2 {
            let lft = targets[2 * i];
            let rgt = targets[2 * j + 1];

            let place_left = dp[i][j + 1] + dist(trees[i + j + 1], lft);
            let place_right = dp[i + 1][j] + dist(trees[i + j + 1], rgt);

            dp[i + 1][j + 1] = place_left.min(place_right);
        }
    }

    println!("{:.10}", dp[N / 2][N / 2]);
}

fn scan_line(buf: &mut String) {
    buf.clear();
    io::stdin().read_line(buf).expect("Failed to read line");
}

#[allow(non_snake_case)]
fn target_positions(N: usize, L: i32, W: i32) -> Vec<(f64, f64)> {
    let mut positions: Vec<(f64, f64)> = vec![];
    positions.reserve(N as usize);

    let step_size: f64 = L as f64 / (N / 2 - 1) as f64;
    for i in 0..N {
        let x: f64 = if i % 2 == 0 { 0.0 } else { W as f64 };
        let y: f64 = (i / 2) as f64 * step_size as f64;

        positions.push((x, y));
    }

    positions
}

fn dist(tree: f64, to: (f64, f64)) -> f64 {
    f64::sqrt((to.0).powi(2) + (tree - to.1).powi(2))
}
