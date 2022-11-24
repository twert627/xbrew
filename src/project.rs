use std::path::Path;
use crate::io_utils::read_string;
use crate::fs_utils::*;

pub fn copy_boilerplate(project_name: String, console: String) {
    println!("DEBUG: Copying boilerplate...\n");
    // build path string using console var
    let mut path_string = String::new();
    path_string.push_str("templates/");
    path_string.push_str(&console);
    println!("{}", path_string);
    let from_string = Path::new(&path_string);
    let mut common_string = String::new();
    common_string.push_str("templates/_common");
    let to_string = Path::new(&project_name);
    println!("{}", common_string);
    // copy dir using custom path
    copy_dir(from_string, to_string);
    copy_dir(common_string, to_string);
    // sanity checking/error handling

}

pub fn init_project() {
    println!("{}", "DEBUG: xbrew project init");
    println!("{}", "Please tell enter project name");
    let project_name = read_string();
    println!("{}", project_name);
    
    println!("Please enter console name [3ds, nds, switch]");
    let console_name = read_string();

    match console_name.as_str() {
        "3ds" => copy_boilerplate(project_name.to_string(),"3ds".to_string()),
        "nds" => copy_boilerplate(project_name.to_string(), "nds".to_string()),
        "switch" => copy_boilerplate(project_name.to_string(), "switch".to_string()),
        _ => println!("{}", "xbrew forgot what the console waws, please report this bug!\n"),
    }
    println!("DEBUG: {}", console_name);

}