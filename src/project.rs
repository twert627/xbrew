use crate::io_utils::read_string;
use crate::fs_utils::*;

pub fn init_project() {
    println!("{}", "xbrew project init");
    println!("{}", "Please tell enter project name");
    let answer = read_string();
    println!("{:#?}", answer);


}