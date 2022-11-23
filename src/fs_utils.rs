use std::fs::{copy, File};
use std::io::{prelude::*, Error};

pub fn read_file(input: String) -> String {
    let file_name: String = input;
    let mut file = File::open(file_name).expect("Cant open file!");

    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Can't read file contents!");

    return contents;
}

pub fn copy_file(source: String, dest: String) -> Result<u64, Error> {
    // read input file
    let result: Result<u64, Error> = Ok(copy(source, dest).expect("File copy failed!"));
    println!("{}", "Succesfuly copied file!");
    return result;
}

pub fn create_file(name: String) -> Result<File, Error> {
    let file: Result<File, Error> = Ok(File::create(name).expect("Failed to create file!"));
    match file {
        Err(ref e) => println!("{:?}", e),
        _ => (),
    }
    println!("{}", "Succesfuly created file!");
    return file;
}
