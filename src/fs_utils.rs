use std::fs::{copy, File, self};
use std::path::{Path, PathBuf};
use std::io::{prelude::*, Error};

#[allow(dead_code)]
pub fn read_file(input: String) -> String {
    let file_name: String = input;
    let mut file = File::open(file_name).expect("Cant open file!");

    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Can't read file contents!");

    return contents;
}

#[allow(dead_code)]
pub fn copy_file(source: String, dest: String) -> Result<u64, Error> {
    // read input file
    let result: Result<u64, Error> = Ok(copy(source, dest).expect("File copy failed!"));
    println!("{}", "Succesfuly copied file!");
    return result;
}

#[allow(dead_code)]
// https://stackoverflow.com/a/60406693
pub fn copy_dir<U: AsRef<Path>, V: AsRef<Path>>(from: U, to: V) -> Result<(), Error> {
    let mut stack = Vec::new();
    stack.push(PathBuf::from(from.as_ref()));

    let output_root = PathBuf::from(to.as_ref());
    let input_root = PathBuf::from(from.as_ref()).components().count();

    while let Some(working_path) = stack.pop() {
        println!("process: {:?}", &working_path);

        // Generate a relative path
        let src: PathBuf = working_path.components().skip(input_root).collect();

        // Create a destination if missing
        let dest = if src.components().count() == 0 {
            output_root.clone()
        } else {
            output_root.join(&src)
        };
        if fs::metadata(&dest).is_err() {
            println!(" mkdir: {:?}", dest);
            fs::create_dir_all(&dest)?;
        }

        for entry in fs::read_dir(working_path)? {
            let entry = entry?;
            let path = entry.path();
            if path.is_dir() {
                stack.push(path);
            } else {
                match path.file_name() {
                    Some(filename) => {
                        let dest_path = dest.join(filename);
                        println!("  copy: {:?} -> {:?}", &path, &dest_path);
                        fs::copy(&path, &dest_path)?;
                    }
                    None => {
                        println!("failed: {:?}", path);
                    }
                }
            }
        }
    }

    Ok(())
}

#[allow(dead_code)]
pub fn create_file(name: String) -> Result<File, Error> {
    let file: Result<File, Error> = Ok(File::create(name).expect("Failed to create file!"));
    match file {
        Err(ref e) => println!("{:?}", e),
        _ => (),
    }
    println!("{}", "Succesfuly created file!");
    return file;
}

#[cfg(test)]
mod tests {
    use super::*;

//     #[test]
//     fn test_read_file() {
//         assert_eq!(read_file("README.md".to_owned()));
// }
}