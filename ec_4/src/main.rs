struct Config {
    filename: String,
    query: String,
    threads: u64,
}
impl Config {
    fn new(info: Vec<String>) -> Self {
        let filename = info[1].clone();
        let query = info[2].clone();
        let threads = info[3].parse::<u64>().expect("Invalid thread count");
        Self {
            filename,
            query,
            threads,
        }
    }
}
use std::env;
use std::fs::File;
use std::fs::metadata;
use std::io::prelude::*;
use std::time::SystemTime;
fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 4 {
        panic!("./program filename query thead_count")
    }
    let config = Config::new(args);
    let size = fileContents(config.filename.clone());
    let chunk_size = size / config.threads;
    let mut ranges = vec![];
    for i in 0..config.threads {
        let start = i * chunk_size;
        let end = if i == config.threads - 1 {
            size
        } else {
            (i + 1) * chunk_size
        };
        ranges.push((start, end));
    }
    let start_time = SystemTime::now();
    find_match(ranges, config.filename.clone(), config.query);
    let finish_time = SystemTime::now()
        .duration_since(start_time)
        .expect("Time went backwards")
        .as_nanos();
    println!(
        "{} processed in {} nanoseconds",
        config.filename, finish_time
    );
}

fn fileContents(filename: String) -> u64 {
    let size = metadata(&filename).unwrap().len();
    size
}
use std::io::{Read, Seek, SeekFrom};
use std::thread;
fn find_match(ranges: Vec<(u64, u64)>, filename: String, query: String) {
    let mut handles = vec![];

    for (start, end) in ranges {
        let filename = filename.clone();
        let query = query.clone();
        let handle = thread::spawn(move || {
            let mut file = File::open(filename).expect("Could not open file");
            let length = end - start;
            file.seek(SeekFrom::Start(start)).expect("Seek failed");
            let mut buffer = vec![0; length as usize];
            file.read_exact(&mut buffer).expect("Read failed");

            if let Ok(text) = String::from_utf8(buffer) {
                for (i, line) in text.lines().enumerate() {
                    if line.contains(&query) {
                        println!("{i}: {line}");
                    }
                }
            }
        });
        handles.push(handle);
    }

    // Wait for all threads to finish
    for handle in handles {
        handle.join().expect("Thread panicked");
    }
}
