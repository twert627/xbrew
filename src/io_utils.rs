use std::env;
use std::io;

#[path = "project.rs"]
mod project;

pub fn handle_args() {
    let help: &str = "xbrew <command> [options]\n\
                      \n\
                      Options:\n\
                      \n\
                        -h, --help  Output this message\n\
                      \n\
                      Commands:\n\
                      \n\
                        init        Start a new project\n\
                        up, update  Update xbrew to a specified or latest version";

    let args: Vec<String> = env::args().collect();
    let mut command = String::new();

    if args.len() < 2 {
        println!("At least one argument is required, showing help\n");
        println!("{}", help);
    } else {
        command = args[1].clone();
    }

    match command.as_str() {
        "help" => println!("{}", help),
        "init" => project::init_project(),
        _ => println!("{}", help),
    }
}

pub fn read_string() -> String {
    let mut line = String::new();
    let input = std::io::stdin().read_line(&mut line).unwrap();
    line = line.to_string();
    line = line.trim().to_owned();
    return line;
}
