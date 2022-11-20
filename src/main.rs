use std::io;
use clap::{Parser, Subcommand};
use colored::*;

mod io_utils;
mod fs_utils;
// mod update;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Inits new project (xbrew init project_name console_name)
    Init {},
    /// Updates xbrew to latest version
    Update {},
}

fn main() {
   let args = Cli::parse();

   match &args.command {
       Commands::Init {} => {
        println!("{}", "Enter project name (any string)".yellow());
        let project_name = io_utils::read_input();

        println!("{}", "Enter console name (3ds, nds, switch)".yellow());
        let console_name = io_utils::read_input();
        
        // println!("[project name]: {}", project_name);
        // println!("[console name]: {}", console_name);
       }

       Commands::Update {} => {
        println!("{}", "Would you like to download latest xbrew? [Y/n]".yellow());
        let mut answer = io_utils::read_input();

        match answer {            
            Ok(_) => println!("OK"),
            Err(_) => todo!(),
        }

        // if answer == "Y" || answer == "y" {
        //     println!("{}", "Ok, downloading xbrew version: ".green());
        //     // update::update();
        // } else if answer == "N" || answer == "n" {
        //     println!("{}", "Ok, not downloading.".blue());
        //     return;
        // } else {
        //     println!("{}", "ERROR: Invalid input, please use [Y, y, N, n]");
        //     // update::update();
        //     fs_utils::create_file("file.txt".to_string());
        // }

       }
   }
}
