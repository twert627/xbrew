use std::io::{self, BufRead};

pub fn read_input() -> io::Result<()> {
    let mut lines = io::stdin().lock().lines();
    let mut user_input = String::new();

    while let Some(line) = lines.next() {
        let last_input = line.unwrap();

        // stop reading
        if last_input.len() == 0 {
            break;
        }

        // add a new line once user_input starts storing user input
        if user_input.len() > 0 {
            user_input.push_str("\n");
        }

        // store user input
        user_input.push_str(&last_input);
    }

    println!("\nMulti-line user input \n{}", user_input);

    // the lock is released after it goes out of scope
    Ok(())
}
